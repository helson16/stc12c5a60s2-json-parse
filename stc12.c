#include <main.h>
#include "RelayD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Send_485_len 32
#define SENSOR_DEFINE 0 
#define ENABLE_ISP 0x82  
#define SECTOR_SIZE 256
#define SECTOR_BASE_ADDRESS 0x0000
#define ENABLE_ISP 0x82
#define JSON_STRING_SIZE 200
#define NULL ((void *)0)

uint8 crc_Check = 0;
uint8 getCount = 0;
uint8 usart2_rx_over = 0;
u16 sensor_time = 0;
uint8 Flag = 0, lenth = 0;
uint8 Send_485[Send_485_len];
struct _Relay Relay;

u8 type = 1;
uint8 length = 0;
uint8 Rx_i = 0;
uint16 crcData;	
volatile int json_ready = 0;

unsigned char xdata json_buffer[JSON_STRING_SIZE];
int json_index = 0;
int bracket_count = 0; 
unsigned char json_complete = 0;
unsigned char complet_flag = 0;
int brace_count = 0;

typedef struct {
    int net;
    int node;
    int stime;
    int sen_count;
    int sen_type[8];
    int sen_code[8];
} ConfigParams;
ConfigParams config;

unsigned char received_char;

void flash_write_data(unsigned char* char_data, unsigned int length);
void flash_read_data(unsigned char* char_data, unsigned int length);
void flash_write_read_struct(ConfigParams config_params_write);
void flash_read(void);
void IAP_Disable(void);
char putchar(char ch);
void configure_string_parsing(char *json_string);

void flash_write_data(unsigned char* char_data, unsigned int length);
void flash_read_data(unsigned char* char_data, unsigned int length);
void flash_write_data_con(ConfigParams* source_data);
void flash_read_data_con(ConfigParams* target_data);

u8 Modbus_485_uart2_TX(void);
u8 Modbus_485_package(u8 *buf, u8 slave_addr, u8 function_code, u8 start_H_addr, u8 start_L_addr, u8 number_H_addr, u8 number_L_addr);
int my_strtol(const char *nptr, char **endptr, int base);
char *mystrtok_r(char *str, const char *delim, char **saveptr);

static void __Relay_1(unsigned char Flag);
static void __Relay_2(unsigned char Flag);
static void __Relay_3(unsigned char Flag);
static void __Relay_4(unsigned char Flag);

typedef void (*__cbRelayFun)(unsigned char);
static __cbRelayFun const _cbRelayFun[4] = {
    __Relay_1,
    __Relay_2,
    __Relay_3,
    __Relay_4,
};

void Single_Relay(u8 ch, u8 sw);
char putchar(char ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
    return ch;
}

void IAP_Disable()      
{                       
    IAP_CONTR = 0;      
    IAP_CMD   = 0;      
    IAP_TRIG = 0;       
    IAP_ADDRH = 0;      
    IAP_ADDRL = 0;      
}
  

unsigned char Byte_Read(unsigned int add)
{
    IAP_DATA = 0x00;             
    IAP_CONTR = ENABLE_ISP;      
    IAP_CMD = 0x01;              
  
    IAP_ADDRH = (unsigned char)(add>>8);    
    IAP_ADDRL = (unsigned char)(add&0xff);   
  
    EA = 0;
    IAP_TRIG = 0x5a;   
    IAP_TRIG = 0xa5;   
    _nop_();
    EA = 1;
    IAP_Disable(); 
                    
    return (IAP_DATA);
}
  
void Byte_Program(unsigned int add,unsigned char ch)
{
    IAP_CONTR = ENABLE_ISP;         
    IAP_CMD = 0x02;                
  
  
    IAP_ADDRH = (unsigned char)(add>>8);    
    IAP_ADDRL = (unsigned char)(add&0xff);  
  
    IAP_DATA = ch;                 
    EA = 0;
    IAP_TRIG = 0x5a;   
    IAP_TRIG = 0xa5;  
    _nop_();
    EA = 1;
    IAP_Disable(); 
}
  
void Sector_Erase(unsigned int add)      
{
    IAP_CONTR = ENABLE_ISP;         
    IAP_CMD = 0x03;                
  
    IAP_ADDRH = (unsigned char)(add>>8);   
    IAP_ADDRL = (unsigned char)(add&0xff);   
  
    EA = 0;
    IAP_TRIG = 0x5a;   
    IAP_TRIG = 0xa5; 
    _nop_();
    EA = 1;
    IAP_Disable();     
}

void main() {
    rece2Count = 0;
    receCount = 0;
    port_init();
    Timer0_Init();
    uart1_init();
    uart2_init();
    RS485E = 0;
    flash_read();
    
    // Ensure a delay after initialization
    Delay_ms(100);

    while (1) {
        if (json_ready && json_complete) {
            json_ready = 0;
            configure_string_parsing(json_buffer);
            memset(json_buffer, 0, JSON_STRING_SIZE);
        }
    }
}

void configure_string_parsing(char *json_string) {
    char json_copy[JSON_STRING_SIZE];
    char *token, *saveptr;
    int execute_flash_write = 0;

    strcpy(json_copy, json_string);
    memset(&config, 0, sizeof(config));

    token = mystrtok_r(json_copy, ",\":{}", &saveptr);
    while (token != NULL) {
        if (strcmp(token, "lora") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            if (strcmp(token, "sensor") == 0) {
                printf("lora: sensor\n");
            } else if (strcmp(token, "set") == 0) {
                printf("lora: set\n");
                execute_flash_write = 1;
            }
        } else if (strcmp(token, "n") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            config.net = my_strtol(token, NULL, 10);
        } else if (strcmp(token, "d") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            config.node = my_strtol(token, NULL, 10);
        } else if (strcmp(token, "t") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            config.stime = (unsigned int)my_strtol(token, NULL, 10);
        } else if (strcmp(token, "s") == 0) {
            token = mystrtok_r(NULL, "[", &saveptr);
            while (token && token[0] != ']') {
                token = mystrtok_r(NULL, ",\":{}", &saveptr);
                if (strcmp(token, "c") == 0) {
                    token = mystrtok_r(NULL, ",\":{}", &saveptr);
                    config.sen_code[config.sen_count] = my_strtol(token, NULL, 10);
                } else if (strcmp(token, "p") == 0) {
                    token = mystrtok_r(NULL, ",\":{}", &saveptr);
                    config.sen_type[config.sen_count] = my_strtol(token, NULL, 10);
                    config.sen_count++;
                }
            }
        } else if (strcmp(token, "node") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            printf("node: %s\n", token);
        } else if (strcmp(token, "net") == 0) {
            token = mystrtok_r(NULL, ",\":{}", &saveptr);
            printf("net: %s\n", token);
        }
        token = mystrtok_r(NULL, ",\":{}", &saveptr);
    }

    if (execute_flash_write) {
        flash_write_read_struct(config);
    }
}

void Uart_Isr() interrupt 4 using 1 {
    if (RI) {
        RI = 0;
        received_char = SBUF;
        if (!json_complete && json_index < JSON_STRING_SIZE - 1) {
            json_buffer[json_index++] = received_char;
            if (received_char == '{') {
                brace_count++;
            } else if (received_char == '}') {
                brace_count--;
                if (brace_count == 0 && json_index > 1) {
                    json_buffer[json_index] = '\0';
                    json_complete = 1;
                    json_ready = 1;
                    json_index = 0;
                }
            }
        } else {
            json_complete = 0;
            json_ready = 0;
            json_index = 0;
            brace_count = 0;
            memset(json_buffer, 0, sizeof(json_buffer));
        }
    }
}

void flash_write_data(unsigned char* char_data, unsigned int length) {
    unsigned int address = SECTOR_BASE_ADDRESS;
    unsigned char sector_data[SECTOR_SIZE];
    int i;

    for (i = 0; i < SECTOR_SIZE; i++) {
        sector_data[i] = Byte_Read(i);
    }

    Sector_Erase(0);

    for (i = 0; i < length; i++) {
        Byte_Program(i, char_data[i]);
    }
}

void flash_read_data(unsigned char* char_data, unsigned int length) {
    unsigned int address = SECTOR_BASE_ADDRESS;
    unsigned char sector_data[SECTOR_SIZE];
    int i;

    for (i = 0; i < SECTOR_SIZE; i++) {
        sector_data[i] = Byte_Read(i);
    }

    for (i = 0; i < length; i++) {
        if (i < SECTOR_SIZE) {
            char_data[i] = sector_data[i];
        } else {
            char_data[i] = 0;
        }
    }
    char_data[length] = '\0';
}

void flash_write_data_con(ConfigParams* source_data) {
    unsigned char* char_data = (unsigned char*)source_data;
    flash_write_data(char_data, sizeof(ConfigParams));
}

void flash_read_data_con(ConfigParams* target_data) {
    unsigned char char_data[sizeof(ConfigParams)];
    flash_read_data(char_data, sizeof(ConfigParams));
    memcpy(target_data, char_data, sizeof(ConfigParams));
}

void flash_write_read_struct(ConfigParams config_params_write) {
    ConfigParams config_params_read;
    int i;

    flash_write_data_con(&config_params_write);
    flash_read_data_con(&config_params_read);

    if (memcmp(&config_params_write, &config_params_read, sizeof(ConfigParams)) == 0) {
        printf("System parameters configured successfully.\n");
        printf("Lora_ID: %d\n", config_params_read.net);
        printf("Node_ID: %d\n", config_params_read.node);
        printf("Time_NO: %d\n", config_params_read.stime);
        printf("Sens_NO: %d\n", config_params_read.sen_count);

        if (config_params_read.sen_count >= 1 && config_params_read.sen_count <= 32) {
            for (i = 0; i < config_params_read.sen_count; i++) {
                printf("SensorNumber:%d (", i + 1);
                printf("type:%d ", config_params_read.sen_type[i]);
                printf("code:%d )\n", config_params_read.sen_code[i]);
            }
        }
    } else {
        printf("System parameters configured Failed!\n");
    }

    memset(&config_params_read, 0, sizeof(ConfigParams));
    memset(&config_params_write, 0, sizeof(config_params_write));
}

void flash_read(void) {
    int i;

    flash_read_data_con(&config);

    if (config.net >= 1 && config.net <= 255) {
        printf("System configuration parameters:\n");
        printf("Lora_ID: %d\n", config.net);
        printf("Node_ID: %d\n", config.node);
        printf("Time_NO: %d\n", config.stime);
        printf("Sens_NO: %d\n", config.sen_count);

        if (config.sen_count >= 1 && config.sen_count <= 32) {
            for (i = 0; i < config.sen_count; i++) {
                printf("SensorNumber:%d (", i + 1);
                printf("type:%d ", config.sen_type[i]);
                printf("code:%d )\n", config.sen_code[i]);
            }
        }
    } else {
        printf("Please configure parameters!.\n");
    }
}

int my_strtol(const char *nptr, char **endptr, int base) {
    long result = 0;
    int sign = 1;

    while (*nptr == ' ')
        nptr++;

    if (*nptr == '-') {
        sign = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }

    if (base == 0) {
        if (*nptr == '0') {
            nptr++;
            if (*nptr == 'x' || *nptr == 'X') {
                nptr++;
                base = 16;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    while (*nptr != '\0') {
        int digit;

        if ('0' <= *nptr && *nptr <= '9') {
            digit = *nptr - '0';
        } else if ('a' <= *nptr && *nptr <= 'z') {
            digit = *nptr - 'a' + 10;
        } else if ('A' <= *nptr && *nptr <= 'Z') {
            digit = *nptr - 'A' + 10;
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        result = result * base + digit;
        nptr++;
    }

    if (endptr != NULL) {
        *endptr = (char *)nptr;
    }

    return result * sign;
}

char *mystrtok_r(char *str, const char *delim, char **saveptr) {
    if (!str)
        str = *saveptr;
    str += strspn(str, delim);
    if (!*str)
        return NULL;
    *saveptr = str + strcspn(str, delim);
    if (**saveptr)
        *(*saveptr)++ = '\0';
    else
        *saveptr = NULL;
    return str;
}

static void __Relay_1(unsigned char Flag) {
    if (Flag) P26 = 0;
    else P26 = 1;
}
static void __Relay_2(unsigned char Flag) {
    if (Flag) P25 = 0;
    else P25 = 1;
}
static void __Relay_3(unsigned char Flag) {
    if (Flag) P22 = 0;
    else P22 = 1;
}
static void __Relay_4(unsigned char Flag) {
    if (Flag) P23 = 0;
    else P23 = 1;    
}

void Single_Relay(u8 ch, u8 sw) {
    Relay.Ctrl = ch;
    switch(Relay.Ctrl) {
        case 1:
            Relay.cbAct = _cbRelayFun[0];
            if(sw) Relay.cbAct(1);
            else Relay.cbAct(0);
            Relay.Ctrl = 0;
            break;
        case 2:
            Relay.cbAct = _cbRelayFun[1];
            if(sw) Relay.cbAct(1);
            else Relay.cbAct(0);
            Relay.Ctrl = 0;
            break;
        case 3:
            Relay.cbAct = _cbRelayFun[2];
            if(sw) Relay.cbAct(1);
            else Relay.cbAct(0);
            Relay.Ctrl = 0;
            break;
        case 4:
            Relay.cbAct = _cbRelayFun[3];
            if(sw) Relay.cbAct(1);
            else Relay.cbAct(0);
            Relay.Ctrl = 0;
            break;
        default:
            break;
    }
}
