/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : I2C                */
/* Layer     : MCAL   		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MI2C_private.h>
#include <MI2C_interface.h>


STD_error_t MI2C_stderrInit(u32 ARG_u32Bitrate, u32 ARG_u32CPUFrequency)
{
	STD_error_t L_stderrState = E_OK;

	if(ARG_u32Bitrate <= 400000 && ARG_u32CPUFrequency <= 50000000)
	{
		/*Turn OFF Peripheral*/
		MI2C1->CR1 = 0;

		/*Set CPU Frequency and Calculate values for required bitrate*/
		MI2C1->CR2 = ARG_u32CPUFrequency/1000000;
		MI2C1->CCR = (ARG_u32CPUFrequency) / (2*ARG_u32Bitrate);

		/*Max Rise Time (Hardcoded for 8MHz Clock for now)*/
		MI2C1->TRISE = 0x09;
	}
	else
	{
		L_stderrState = E_NOK;
	}

	return L_stderrState;
}

void MI2C_voidEnable(void)
{
	/*Enable I2C Peripheral*/
	SET_BIT(MI2C1->CR1, MI2C_CR1_PE);
}

void MI2C_voidDisable(void)
{
	/*Disable I2C Peripheral*/
	CLEAR_BIT(MI2C1->CR1, MI2C_CR1_PE);
}

void MI2C_voidEnableACK(void)
{
	SET_BIT(MI2C1->CR1, MI2C_CR1_ACK);
}

void MI2C_voidDisableACK(void)
{
	CLEAR_BIT(MI2C1->CR1, MI2C_CR1_ACK);
}

void MI2C_voidSendStartCondition(void)
{
	/*Start generation*/
	SET_BIT(MI2C1->CR1, MI2C_CR1_START);
}


void MI2C_voidSendStopCondition(void)
{
	/*Start generation*/
	SET_BIT(MI2C1->CR1, MI2C_CR1_STOP);
}

void MI2C_voidTransmitSlaveAddress(u8 ARG_u8SlaveAddress, u8 ARG_u8ReadWrite)
{
	/*Send Slave Address*/
	MI2C1->DR = (ARG_u8SlaveAddress & 0xFE) | (ARG_u8ReadWrite & 0x01);
}


void MI2C_voidTransmitByte(u8 ARG_u8Byte)
{
	/*Transmit a byte over the bus*/
	MI2C1->DR = ARG_u8Byte;
}


u8 MI2C_u8ReceiveByte(void)
{
	u8 L_u8Data = 0;

	L_u8Data = MI2C1->DR;
	
	return L_u8Data;
}


void MI2C_voidFlagPolling(u8 ARG_u8Flag)
{
	if(ARG_u8Flag == MI2C_BUSY)
	{
		while(!GET_BIT(MI2C1->SR2, MI2C_SR2_BUSY));
	}
	else
	{
		while(!GET_BIT(MI2C1->SR1, ARG_u8Flag));	
	}
}

void MI2C_voidClearFlags(void)
{
	u8 L_u8DummyRead = 0;
	L_u8DummyRead = MI2C1->SR1 | MI2C1->SR2;
}
