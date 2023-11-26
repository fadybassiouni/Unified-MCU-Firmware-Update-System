/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : GPIO               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 16,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MGPIO_INTERFACE_H_
#define _MGPIO_INTERFACE_H_

/*Pin IDs*/
#define MGPIO_PIN_A0                         0
#define MGPIO_PIN_A1                         1
#define MGPIO_PIN_A2                         2
#define MGPIO_PIN_A3                         3
#define MGPIO_PIN_A4                         4
#define MGPIO_PIN_A5                         5
#define MGPIO_PIN_A6                         6
#define MGPIO_PIN_A7                         7
#define MGPIO_PIN_A8                         8
#define MGPIO_PIN_A9                         9
#define MGPIO_PIN_A10                        10
#define MGPIO_PIN_A11                        11
#define MGPIO_PIN_A12                        12
#define MGPIO_PIN_A13                        13
#define MGPIO_PIN_A14                        14
#define MGPIO_PIN_A15                        15
#define MGPIO_PIN_B0                         16
#define MGPIO_PIN_B1                         17
#define MGPIO_PIN_B2                         18
#define MGPIO_PIN_B3                         19
#define MGPIO_PIN_B4                         20
#define MGPIO_PIN_B5                         21
#define MGPIO_PIN_B6                         22
#define MGPIO_PIN_B7                         23
#define MGPIO_PIN_B8                         24
#define MGPIO_PIN_B9                         25
#define MGPIO_PIN_B10                        26
#define MGPIO_PIN_B11                        27
#define MGPIO_PIN_B12                        28
#define MGPIO_PIN_B13                        29
#define MGPIO_PIN_B14                        30
#define MGPIO_PIN_B15                        31
#define MGPIO_PIN_C0                         32
#define MGPIO_PIN_C1                         33
#define MGPIO_PIN_C2                         34
#define MGPIO_PIN_C3                         35
#define MGPIO_PIN_C4                         36
#define MGPIO_PIN_C5                         37
#define MGPIO_PIN_C6                         38
#define MGPIO_PIN_C7                         39
#define MGPIO_PIN_C8                         40
#define MGPIO_PIN_C9                         41
#define MGPIO_PIN_C10                        42
#define MGPIO_PIN_C11                        43
#define MGPIO_PIN_C12                        44
#define MGPIO_PIN_C13                        45
#define MGPIO_PIN_C14                        46
#define MGPIO_PIN_C15                        47

/*Pin Modes Definitions*/
#define MGPIO_MODE_INPUT                     0
#define MGPIO_MODE_OUTPUT_PUSHPULL           1
#define MGPIO_MODE_ALTERNATE_FUNTION         2
#define MGPIO_MODE_ANALOG                    3
#define MGPIO_MODE_INVALID                   4
#define MGPIO_MODE_OUTPUT_OPEN_DRAIN         5


/****************************************************************
**********************GPIO Pin Mode Options**********************
*****************************************************************
* GPIO Mode                        PxODRx   CNFx[1:0] MODEx[1:0]*
* 1-  Analog Input                             00         00    *
* 2-  Floating Input                           01         00    *
* 3-  Pulled Down Input              0         10         00    *
* 4-  Pulled Up Input                1         10         00    *
* 5-  10 MHz GP Push Pull  Output              00         01    *
* 6-  2  MHz GP Push Pull  Output              00         10    *
* 7-  50 MHz GP Push Pull  Output              00         11    *
* 8-  10 MHz GP Open Drain Output              01         01    *
* 9-  2  MHz GP Open Drain Output              01         10    *
* 10- 50 MHz GP Open Drain Output              01         11    *
* 11- 10 MHz AF Push Pull  Output              10         01    *
* 12- 2  MHz AF Push Pull  Output              10         10    *
* 13- 50 MHz AF Push Pull  Output              10         11    *
* 14- 10 MHz AF Open Drain Output              11         01    *
* 15- 2  MHz AF Open Drain Output              11         10    *
* 16- 10 MHz AF Open Drain Output              11         11    *
*****************************************************************/
#define MGPIO_MODE_INPUT_ANALOG              0x0
#define MGPIO_MODE_INPUT_FLOATING			 0x4
#define MGPIO_MODE_INPUT_PULLDOWN            0x8
#define MGPIO_MODE_INPUT_PULLUP              0xC

#define MGPIO_MODE_OUTPUT_PUSHPULL_10MHZ     0x1
#define MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ      0x2
#define MGPIO_MODE_OUTPUT_PUSHPULL_50MHZ     0x3

#define MGPIO_MODE_OUTPUT_OPENDRAIN_10MHZ    0x5
#define MGPIO_MODE_OUTPUT_OPENDRAIN_2MHZ     0x6
#define MGPIO_MODE_OUTPUT_OPENDRAIN_50MHZ    0x7

#define MGPIO_MODE_AF_PUSHPULL_10MHZ         0x9
#define MGPIO_MODE_AF_PUSHPULL_2MHZ          0xA
#define MGPIO_MODE_AF_PUSHPULL_50MHZ         0xB

#define MGPIO_MODE_AF_OPENDRAIN_10MHZ        0xD
#define MGPIO_MODE_AF_OPENDRAIN_2MHZ         0xE
#define MGPIO_MODE_AF_OPENDRAIN_50MHZ        0xF
STD_error_t MGPIO_stderrSetPinMode(u8 ARG_u8PinID, u8 ARG_u8PinMode);


/******************************************
**************GPIO Pin States**************
*******************************************
* 1- LOW  -> 0                            *
* 2- HIGH -> 1                            *
*******************************************/
STD_error_t MGPIO_stderrGetPinState(u8 *ARG_pu8PinState, u8 ARG_u8PinID);


/*Pin State Definitions*/
#define MGPIO_LOW                            0
#define MGPIO_HIGH                           1

/******************************************
**********GPIO Pin State Options***********
*******************************************
* 1- MGPIO_LOW  -> 0                      *
* 2- MGPIO_HIGH -> 1                      *
*******************************************/
STD_error_t MGPIO_stderrSetPinState(u8 ARG_u8PinID, u8 ARG_u8PinState);


/*Atomic Pin State Definitions*/
#define MGPIO_ATOMIC_LOW                     0
#define MGPIO_ATOMIC_HIGH                    1

/******************************************
**********GPIO Pin State Options***********
*******************************************
* 1- MGPIO_ATOMIC_LOW  -> 0               *
* 2- MGPIO_ATOMIC_HIGH -> 1               *
*******************************************/
STD_error_t MGPIO_stderrAtomicSetPinState(u8 ARG_u8PinID, u8 ARG_u8PinState);


#define MGPIO_PORTA                          0
#define MGPIO_PORTB                          1
#define MGPIO_PORTC                          2

/******************************************
**********GPIO Port Lock Options***********
*******************************************
* 1- MGPIO_PORTA                          *
* 2- MGPIO_PORTB                          *
* 3- MGPIO_PORTC                          *
*******************************************/
/*Function to Lock the configuration of a specified Port*/
STD_error_t MGPIO_stderrLockPortConfig(u8 ARG_u8PortID,u8 ARG_u8LockConfig);


#endif
