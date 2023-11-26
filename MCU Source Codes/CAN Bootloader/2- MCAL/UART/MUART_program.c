/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : UART               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 23,2023  */
/* Last Edit : N/A                */
/**********************************/

/* Library Includes */
#include <stddef.h>
#include "LSTD_types.h"
#include "LBIT_math.h"

/* SWC Includes */
#include "MUART_private.h"
#include "MUART_interface.h"


static volatile UART_t *MUART_uart_tInterfaces[3] = {MUART1, MUART2, MUART3};

void (*MUART_pvoidfUserFunctionRxC[3])(void) = {NULL_POINTER};
void (*MUART_pvoidfUserFunctionTxC[3])(void) = {NULL_POINTER};
void (*MUART_pvoidfUserFunctionUDREmpty[3])(void) = {NULL_POINTER};


STD_error_t MUART_stderrInit(u8 ARG_u8UARTID, u32 ARG_u32BaudRate , u8 ARG_u8DataBits , u8 ARG_u8Parity , u8 ARG_u8StopBits){

	STD_error_t L_stderrError = E_OK;

	MUART_uart_tInterfaces[ARG_u8UARTID]->BRR = 0x00000045;
	//MUART_uart_tInterfaces[ARG_u8UARTID]->BRR = 0x00000341;
	MUART_uart_tInterfaces[ARG_u8UARTID]->CR1 = (ARG_u8DataBits<<MUART_CR1_M) | ((ARG_u8Parity& 0x01)<<MUART_CR1_PCE) | ((ARG_u8Parity>>0x01)<<MUART_CR1_PS) | (1<<MUART_CR1_UE);
	MUART_uart_tInterfaces[ARG_u8UARTID]->CR2 = (ARG_u8StopBits<<MUART_CR2_STOP0);

	MUART_uart_tInterfaces[ARG_u8UARTID]->SR = 0;
	return L_stderrError;
}

void MUART_voidEnableTx(u8 ARG_u8UARTID)
{	
	SET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1, MUART_CR1_TE);
}

void MUART_voidDisableTx(u8 ARG_u8UARTID)
{	
	CLEAR_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1, MUART_CR1_TE);
}

void MUART_voidEnableRx(u8 ARG_u8UARTID)
{	
	SET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1, MUART_CR1_RE);
}

void MUART_voidDisableRx(u8 ARG_u8UARTID)
{	
	CLEAR_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1, MUART_CR1_RE);
}

void MUART_voidTxFlagPolling(u8 ARG_u8UARTID)
{	
	while(!GET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->SR, MUART_SR_TC));
}

void MUART_voidRxFlagPolling(u8 ARG_u8UARTID)
{	
	while(!GET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->SR, MUART_SR_RXNE));
}

void MUART_voidUDREFlagPolling(u8 ARG_u8UARTID)
{	
	while(!GET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->SR, MUART_SR_TXE));
}

void MUART_voidSendChar(u8 ARG_u8UARTID, char ARG_charChar)
{	
	MUART_voidTxFlagPolling(ARG_u8UARTID);
	MUART_uart_tInterfaces[ARG_u8UARTID]->DR = ARG_charChar;
}

void MUART_voidSendString(u8 ARG_u8UARTID, const char* ARG_ccharpString)
{	
	u32 L_u32i=0;
	do{
		MUART_voidSendChar(ARG_u8UARTID, ARG_ccharpString[L_u32i]);
		L_u32i++;
	}while(ARG_ccharpString[L_u32i]!='\0');
}

char MUART_voidRecieveCharBlocking(u8 ARG_u8UARTID)
{	
	MUART_voidRxFlagPolling(ARG_u8UARTID);
	return MUART_uart_tInterfaces[ARG_u8UARTID]->DR;
}

char MUART_voidRecieveCharNonBlocking(u8 ARG_u8UARTID)
{
	char L_charUDR=0;

	if(GET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->SR, MUART_SR_RXNE))
	{
		L_charUDR = MUART_uart_tInterfaces[ARG_u8UARTID]->DR;
	}
	else
	{
		L_charUDR = 0;
	}

	return L_charUDR;
}

void MUART_voidRecieveStringUntil(u8 ARG_u8UARTID, char *ARG_charString , char ARG_charTerminator)
{	
	u32 L_u32i=0;

	do
	{
		ARG_charString[L_u32i] = MUART_voidRecieveCharBlocking(ARG_u8UARTID);
		L_u32i++;

	}while(ARG_charString[L_u32i-1]!=ARG_charTerminator);

	ARG_charString[L_u32i-1] = '\0';	
}

STD_error_t MUART_stderrEnableInterrupt(u8 ARG_u8UARTID, u8 ARG_u8InterruptSource)
{	
	STD_error_t L_stderrError = E_OK;
	
	if((ARG_u8InterruptSource>=MUART_INTTERUPT_RXC) && (ARG_u8InterruptSource <=MUART_INTTERUPT_DR_EMPTY))
	{
		SET_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1 , ARG_u8InterruptSource);
	}
	else
	{
		L_stderrError = E_NOK;	
	}
	return L_stderrError;
}

STD_error_t MUART_stderrDisableInterrupt(u8 ARG_u8UARTID, u8 ARG_u8InterruptSource)
{
	STD_error_t L_stderrError = E_OK;
	
	if((ARG_u8InterruptSource>=MUART_INTTERUPT_RXC) && (ARG_u8InterruptSource <=MUART_INTTERUPT_DR_EMPTY))
	{
		CLEAR_BIT(MUART_uart_tInterfaces[ARG_u8UARTID]->CR1 , ARG_u8InterruptSource);
	}
	else
	{	
		L_stderrError = E_NOK;	
	}
	return L_stderrError;
}

STD_error_t MUART_stderrSetCallback(void (*ARG_pvoidfUserFunction)(void), u8 ARG_u8UARTID, u8 ARG_u8InterruptSource)
{	
	STD_error_t L_stderrError = E_OK;
	
	switch(ARG_u8InterruptSource)
	{	
		case MUART_INTTERUPT_RXC       : MUART_pvoidfUserFunctionRxC[ARG_u8UARTID] = ARG_pvoidfUserFunction ; break;
		case MUART_INTTERUPT_TXC       : MUART_pvoidfUserFunctionTxC[ARG_u8UARTID] = ARG_pvoidfUserFunction ; break;
		case MUART_INTTERUPT_DR_EMPTY  : MUART_pvoidfUserFunctionUDREmpty[ARG_u8UARTID] = ARG_pvoidfUserFunction ; break;
	}
	
	return L_stderrError;
}

void USART1_IRQHandler(void)
{
	if(MUART_pvoidfUserFunctionRxC[MUART_UART1] != NULL_POINTER)
	{
		MUART_pvoidfUserFunctionRxC[MUART_UART1]();
	}
	else
	{
		/*MISRA*/
	}

}

void USART2_IRQHandler(void)
{
	if(MUART_pvoidfUserFunctionRxC[MUART_UART2] != NULL_POINTER)
	{
		MUART_pvoidfUserFunctionRxC[MUART_UART2]();
		CLEAR_BIT(MUART_uart_tInterfaces[MUART_UART2]->SR, MUART_SR_RXNE);
	}
	else
	{
		/*MISRA*/
	}

}

