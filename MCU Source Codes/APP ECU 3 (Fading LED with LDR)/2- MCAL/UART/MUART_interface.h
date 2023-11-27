/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : UART               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 23,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MUART_INTERFACE_H_
#define _MUART_INTERFACE_H_


#define MUART_UART1                0
#define MUART_UART2                1
#define MUART_UART3                2

#define MUART_DATABITS_8           0
#define MUART_DATABITS_9           1
							      
#define MUART_PARITY_NONE          0
#define MUART_DATABITS_EVEN        1
#define MUART_DATABITS_ODD         3
							      
#define MUART_STOPBITS_1           0
#define MUART_STOPBITS_0_5         1
#define MUART_STOPBITS_2           2
#define MUART_STOPBITS_1_5         3
 							      
#define MUART_INTTERUPT_RXC         5
#define MUART_INTTERUPT_TXC         6
#define MUART_INTTERUPT_DR_EMPTY    7


STD_error_t MUART_stderrInit(u8 ARG_u8UARTID, u32 ARG_u32BaudRate , u8 ARG_u8DataBits , u8 ARG_u8Parity , u8 ARG_u8StopBits);

void MUART_voidEnableTx(u8 ARG_u8UARTID);
void MUART_voidDisableTx(u8 ARG_u8UARTID);

void MUART_voidEnableRx(u8 ARG_u8UARTID);
void MUART_voidDisableRx(u8 ARG_u8UARTID);

void MUART_voidRxFlagPolling(u8 ARG_u8UARTID);
void MUART_voidTxFlagPolling(u8 ARG_u8UARTID);
void MUART_voidUDREFlagPolling(u8 ARG_u8UARTID);

void MUART_voidSendChar(u8 ARG_u8UARTID, char ARG_charChar);
void MUART_voidSendString(u8 ARG_u8UARTID, const char* ARG_ccharpString);

char MUART_voidRecieveCharBlocking(u8 ARG_u8UARTID);
char MUART_voidRecieveCharNonBlocking(u8 ARG_u8UARTID);
void MUART_voidRecieveStringUntil(u8 ARG_u8UARTID, char *ARG_charString , char ARG_charTerminator);

STD_error_t MUART_stderrEnableInterrupt(u8 ARG_u8UARTID, u8 ARG_u8InterruptSource);
STD_error_t MUART_stderrDisableInterrupt(u8 ARG_u8UARTID, u8 ARG_u8InterruptSource);
STD_error_t MUART_stderrSetCallback(void (*ARG_pvoidfUserFunction)(void), u8 ARG_u8UARTID, u8 ARG_u8InterruptSource);

#endif
