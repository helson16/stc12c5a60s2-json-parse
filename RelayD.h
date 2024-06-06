#ifndef __RELAYD_H
#define __RELAYD_H

#include <STC12C5A.h>
#include <intrins.h>   
#include <math.h>     
#include <stdio.h>   
#include <string.h>

struct _Relay{
	unsigned char Ctrl;
	void (*cbAct)(unsigned char Flag);

};
extern struct _Relay Relay;
void ControlRelay (void);
#endif
