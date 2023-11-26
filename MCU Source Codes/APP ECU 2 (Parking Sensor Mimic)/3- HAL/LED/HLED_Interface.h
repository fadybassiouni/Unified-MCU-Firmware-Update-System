/************************************/
/*	Author	  :	Elamir Galal		*/
/*	SWC		  : LED					*/
/*  Layer	  : HAL					*/
/*	Version	  :	1.0		  			*/
/*	Date	  :						*/
/*	Last Edit :						*/
/************************************/



#ifndef _HLED_INTERFACE_H_
#define _HLED_INTERFACE_H_


/*************	Ports 	 *************/
#define HLED_PORTA			0
#define HLED_PORTB			1
#define HLED_PORTC			2
/*************	Pin Numbers  *************/
#define HLED_PIN_A0                         0
#define HLED_PIN_A1                         1
#define HLED_PIN_A2                         2
#define HLED_PIN_A3                         3
#define HLED_PIN_A4                         4
#define HLED_PIN_A5                         5
#define HLED_PIN_A6                         6
#define HLED_PIN_A7                         7
#define HLED_PIN_A8                         8
#define HLED_PIN_A9                         9
#define HLED_PIN_A10                        10
#define HLED_PIN_A11                        11
#define HLED_PIN_A12                        12
#define HLED_PIN_A13                        13
#define HLED_PIN_A14                        14
#define HLED_PIN_A15                        15
#define HLED_PIN_B0                         16
#define HLED_PIN_B1                         17
#define HLED_PIN_B2                         18
#define HLED_PIN_B3                         19
#define HLED_PIN_B4                         20
#define HLED_PIN_B5                         21
#define HLED_PIN_B6                         22
#define HLED_PIN_B7                         23
#define HLED_PIN_B8                         24
#define HLED_PIN_B9                         25
#define HLED_PIN_B10                        26
#define HLED_PIN_B11                        27
#define HLED_PIN_B12                        28
#define HLED_PIN_B13                        29
#define HLED_PIN_B14                        30
#define HLED_PIN_B15                        31
#define HLED_PIN_C0                         32
#define HLED_PIN_C1                         33
#define HLED_PIN_C2                         34
#define HLED_PIN_C3                         35
#define HLED_PIN_C4                         36
#define HLED_PIN_C5                         37
#define HLED_PIN_C6                         38
#define HLED_PIN_C7                         39
#define HLED_PIN_C8                         40
#define HLED_PIN_C9                         41
#define HLED_PIN_C10                        42
#define HLED_PIN_C11                        43
#define HLED_PIN_C12                        44
#define HLED_PIN_C13                        45
#define HLED_PIN_C14                        46
#define HLED_PIN_C15                        47

void HLED_voidLedInit(u8 ARG_u8Pin);
void HLED_voidLedOn(u8 ARG_u8Pin);
void HLED_voidLedOff(u8 ARG_u8Pin);
void HLED_voidLedToggle(u8 ARG_u8Pin);








#endif
