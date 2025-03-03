# stc12c5a60s2-json-parse
MCU: STC12c5a60s2, function is to send json string configuration parameters through serial port 1, send/flash write/read,Relay control and RS485 sensor reading

## Project Description

This project is designed for the MCU: STC12c5a60s2. The main functionalities of this project include:
- Sending JSON string configuration parameters through serial port 1.
- Sending/flashing write/read operations.
- Relay control.
- RS485 sensor reading.

## Code Structure

The project consists of the following source files:

- `init.c`: Contains initialization functions for the MCU, including delay functions, UART initialization, port initialization, and timer initialization.
- `main.h`: Header file that includes necessary libraries, defines data types, and declares global variables and functions used across the project.
- `modbus.c`: Implements Modbus communication protocol functions, including CRC calculation, UART data sending, and relay control.
- `RelayD.h`: Header file for relay control, defines the relay structure and declares the relay control function.
- `rs485.c`: Contains functions for RS485 communication, including data sending and receiving.
- `STARTUP.A51`: Assembly file for MCU startup, includes memory initialization and stack setup.
- `stc12.c`: Main source file that includes the main function, JSON string parsing, flash memory read/write functions, and relay control functions.
- `STC12C5A.h`: Header file for the STC12c5a60s2 MCU, defines special function registers (SFRs) and bit addresses.

## Setup Instructions

To set up and run the project, follow these steps:

1. Install the Keil uVision IDE for compiling and debugging the project.
2. Open the project in Keil uVision.
3. Compile the project by clicking on the "Build" button.
4. Connect the STC12c5a60s2 MCU to your computer using a USB-to-serial adapter.
5. Flash the compiled firmware to the MCU using the STC-ISP tool.
6. Open a serial terminal (e.g., PuTTY, Tera Term) and connect to the appropriate COM port with the following settings:
   - Baud rate: 9600
   - Data bits: 8
   - Parity: None
   - Stop bits: 1
   - Flow control: None

## Usage Instructions

### Sending JSON String Configuration Parameters

1. Open the serial terminal and connect to the MCU.
2. Send a JSON string with the desired configuration parameters through the serial terminal.
3. The MCU will parse the JSON string and configure the system accordingly.

### Relay Control

1. Use the serial terminal to send commands for controlling the relays.
2. The relays can be turned on or off based on the received commands.

### RS485 Sensor Reading

1. Connect the RS485 sensors to the MCU.
2. Use the serial terminal to send commands for reading sensor data.
3. The MCU will read the sensor data and send it back through the serial terminal.

## Troubleshooting

### Common Issues and Solutions

1. **Issue**: The MCU is not responding to commands.
   - **Solution**: Check the serial connection and ensure the correct COM port and settings are used.

2. **Issue**: The relays are not functioning as expected.
   - **Solution**: Verify the relay connections and ensure the correct commands are sent through the serial terminal.

3. **Issue**: RS485 sensor data is not being read correctly.
   - **Solution**: Check the RS485 sensor connections and ensure the correct commands are sent through the serial terminal.

4. **Issue**: JSON string configuration parameters are not being applied.
   - **Solution**: Ensure the JSON string is correctly formatted and sent through the serial terminal.

For further assistance, refer to the project documentation or contact the project maintainer.
