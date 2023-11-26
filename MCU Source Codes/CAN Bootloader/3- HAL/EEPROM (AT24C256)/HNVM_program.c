/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : EEPROM             */
/* Layer     : HAL  		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MRCC_interface.h>
#include <MGPIO_interface.h>
#include <MI2C_interface.h>
#include <MUART_interface.h>

#include <HNVM_private.h>
#include <HNVM_interface.h>
#include <HNVM_config.h>


STD_error_t HNVM_stderrInit(void)
{
	STD_error_t L_stderrState = E_OK;

	/*I2C RCC Initialization*/
	L_stderrState = MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_I2C1);

	/*I2C Initialization*/
	L_stderrState = MI2C_stderrInit(HNVM_BIT_RATE, HNVM_PERIPHERAL_FREQ);

	/*EEPROM Address Pins initialization*/
	if(   (MNVM_ADDRESS_PIN_A0 >= MGPIO_PIN_A0 && MNVM_ADDRESS_PIN_A0 <= MGPIO_PIN_A15)
			|| (MNVM_ADDRESS_PIN_A1 >= MGPIO_PIN_A0 && MNVM_ADDRESS_PIN_A1 <= MGPIO_PIN_A15)
			|| (MNVM_ADDRESS_PIN_A2 >= MGPIO_PIN_A0 && MNVM_ADDRESS_PIN_A2 <= MGPIO_PIN_A15))
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	}
	else if(   (MNVM_ADDRESS_PIN_A0 >= MGPIO_PIN_B0 && MNVM_ADDRESS_PIN_A0 <= MGPIO_PIN_B15)
			|| (MNVM_ADDRESS_PIN_A1 >= MGPIO_PIN_B0 && MNVM_ADDRESS_PIN_A1 <= MGPIO_PIN_B15)
			|| (MNVM_ADDRESS_PIN_A2 >= MGPIO_PIN_B0 && MNVM_ADDRESS_PIN_A2 <= MGPIO_PIN_B15))
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	}
	else if(   (MNVM_ADDRESS_PIN_A0 >= MGPIO_PIN_C0 && MNVM_ADDRESS_PIN_A0 <= MGPIO_PIN_C15)
			|| (MNVM_ADDRESS_PIN_A1 >= MGPIO_PIN_C0 && MNVM_ADDRESS_PIN_A1 <= MGPIO_PIN_C15)
			|| (MNVM_ADDRESS_PIN_A2 >= MGPIO_PIN_C0 && MNVM_ADDRESS_PIN_A2 <= MGPIO_PIN_C15))
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOC);
	}
	else
	{
		L_stderrState = E_NOK;
	}

	/*I2C GPIO Initialization*/
	L_stderrState = MGPIO_stderrSetPinMode(MGPIO_PIN_B6, MGPIO_MODE_AF_OPENDRAIN_10MHZ);
	L_stderrState = MGPIO_stderrSetPinMode(MGPIO_PIN_B7, MGPIO_MODE_AF_OPENDRAIN_10MHZ);

	L_stderrState = MGPIO_stderrSetPinMode(MNVM_ADDRESS_PIN_A0, MGPIO_MODE_OUTPUT_PUSHPULL_10MHZ);
	L_stderrState = MGPIO_stderrSetPinMode(MNVM_ADDRESS_PIN_A1, MGPIO_MODE_OUTPUT_PUSHPULL_10MHZ);
	L_stderrState = MGPIO_stderrSetPinMode(MNVM_ADDRESS_PIN_A2, MGPIO_MODE_OUTPUT_PUSHPULL_10MHZ);


	/*Enable I2C Peripheral*/
	MI2C_voidEnable();

	return L_stderrState;
}

static void HNVM_voidSetAddressPins(u32 ARG_u32Address)
{
	MGPIO_stderrSetPinState(MNVM_ADDRESS_PIN_A0, GET_BIT(ARG_u32Address, 15));
	MGPIO_stderrSetPinState(MNVM_ADDRESS_PIN_A1, GET_BIT(ARG_u32Address, 16));
	MGPIO_stderrSetPinState(MNVM_ADDRESS_PIN_A2, GET_BIT(ARG_u32Address, 17));
}

void HNVM_voidWriteByte(u32 ARG_u32Address, u8 ARG_u8Data)
{
	u8 L_u8SlaveAddress = 0;

	/*Set Address Pins*/
	HNVM_voidSetAddressPins(ARG_u32Address);

	/*Calculate Slave Address to be sent with extensions*/
	L_u8SlaveAddress = HNVM_SLAVE_ADDRESS | ((ARG_u32Address & 0x00038000) >> 14);

	/*Send Start Condition*/
	MI2C_voidSendStartCondition();
	MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

	/*Transmit Slave Address + Write*/
	MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_WRITE);
	MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
	MI2C_voidClearFlags();

	/*Transmit Data Address*/
	/*Transmit High Address Byte*/
	MI2C_voidTransmitByte((ARG_u32Address >> 8) & 0xFF);
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

	/*Transmit Low Address Byte*/
	MI2C_voidTransmitByte(ARG_u32Address & 0xFF);
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

	/*Transmit The Data*/
	MI2C_voidTransmitByte(ARG_u8Data);
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

	/*Send Stop Condition*/
	MI2C_voidSendStopCondition();
}

STD_error_t HNVM_stderrBurstWrite(u32 ARG_u32StartAddress, u8 *ARG_pu8Data, u8 ARG_u8ByteCount)
{
	STD_error_t L_stderrState = E_OK;
	u8 L_u8SlaveAddress = 0;
	u8 L_u8Iterator = 0;

	if(ARG_u8ByteCount <= HNVM_MAX_BURST_WRITE_BYTE_COUNT)
	{
		/*Set Address Pins*/
		HNVM_voidSetAddressPins(ARG_u32StartAddress);

		/*Calculate Slave Address to be sent with extensions*/
		L_u8SlaveAddress = HNVM_SLAVE_ADDRESS | ((ARG_u32StartAddress & 0x00038000) >> 14);

		/*Send Start Condition*/
		MI2C_voidSendStartCondition();
		MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

		/*Transmit Slave Address + Write*/
		MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_WRITE);
		MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
		MI2C_voidClearFlags();

		/*Transmit Data Address*/
		/*Transmit High Address Byte*/
		MI2C_voidTransmitByte((ARG_u32StartAddress >> 8) & 0xFF);
		MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

		/*Transmit Low Address Byte*/
		MI2C_voidTransmitByte(ARG_u32StartAddress & 0xFF);
		MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);


		/*Transmit The Data*/

		for(L_u8Iterator = 0; L_u8Iterator < ARG_u8ByteCount; L_u8Iterator++)
		{
			MI2C_voidTransmitByte(ARG_pu8Data[L_u8Iterator]);
			MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);
		}

		/*Send Stop Condition*/
		MI2C_voidSendStopCondition();
	}
	else
	{
		L_stderrState = E_NOK;
	}

	return L_stderrState;
}



u8 HNVM_u8ReadByte(u32 ARG_u32Address)
{
	u8 L_u8SlaveAddress = 0;
	u8 L_u8Data = 0;

	/*Set Address Pins*/
	HNVM_voidSetAddressPins(ARG_u32Address);

	/*Calculate Slave Address to be sent with extensions*/
	L_u8SlaveAddress = HNVM_SLAVE_ADDRESS | ((ARG_u32Address & 0x00038000) >> 14);

	/*Send Start Condition*/
	MI2C_voidSendStartCondition();
	MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

	/*Transmit Slave Address + Write*/
	MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_WRITE);
	MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
	MI2C_voidClearFlags();

	/*Transmit Data Address*/
	/*Transmit High Address Byte*/
	MI2C_voidTransmitByte((ARG_u32Address >> 8) & 0xFF);
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

	/*Transmit Low Address Byte*/
	MI2C_voidTransmitByte(ARG_u32Address & 0xFF);
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

	/*Send Start Condition*/
	MI2C_voidSendStartCondition();
	MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

	/*Transmit Slave Address + Read*/
	MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_READ);
	MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
	MI2C_voidClearFlags();

	/*Receieve The Data*/
	MI2C_voidFlagPolling(MI2C_DATA_BUFFER_NOT_EMPTY);
	L_u8Data = MI2C_u8ReceiveByte();

	/*Send Stop Condition*/
	MI2C_voidSendStopCondition();

	return L_u8Data;
}


STD_error_t HNVM_stderrBurstRead(u32 ARG_u32StartAddress, u8 *ARG_pu8Data, u32 ARG_u32ByteCount)
{
	STD_error_t L_stderrState = E_OK;
	u8 L_u8SlaveAddress = 0;
	u32 L_u32Iterator = 0;

	if(ARG_u32ByteCount <= HNVM_MAX_BURST_READ_BYTE_COUNT)
	{
		/*Enable Acknowledgment after byte receiving*/
		MI2C_voidEnableACK();

		/*Set Address Pins*/
		HNVM_voidSetAddressPins(ARG_u32StartAddress);

		/*Calculate Slave Address to be sent with extensions*/
		L_u8SlaveAddress = HNVM_SLAVE_ADDRESS | ((ARG_u32StartAddress & 0x00038000) >> 14);

		/*Send Start Condition*/
		MI2C_voidSendStartCondition();
		MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

		/*Transmit Slave Address + Write*/
		MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_WRITE);
		MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
		MI2C_voidClearFlags();

		/*Transmit Data Address*/
		/*Transmit High Address Byte*/
		MI2C_voidTransmitByte((ARG_u32StartAddress >> 8) & 0xFF);
		MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

		/*Transmit Low Address Byte*/
		MI2C_voidTransmitByte(ARG_u32StartAddress & 0xFF);
		MI2C_voidFlagPolling(MI2C_DATA_BUFFER_EMPTY);

		/*Send Start Condition*/
		MI2C_voidSendStartCondition();
		MI2C_voidFlagPolling(MI2C_START_CONDITION_SENT);

		/*Transmit Slave Address + Read*/
		MI2C_voidTransmitSlaveAddress(L_u8SlaveAddress, MI2C_READ);
		MI2C_voidFlagPolling(MI2C_SLAVE_ADDR_SENT);
		MI2C_voidClearFlags();

		/*Receive The Data*/
		for(L_u32Iterator = 0; L_u32Iterator < ARG_u32ByteCount; L_u32Iterator++)
		{
			/*Receive The Data*/
			MI2C_voidFlagPolling(MI2C_DATA_BUFFER_NOT_EMPTY);
			ARG_pu8Data[L_u32Iterator] = MI2C_u8ReceiveByte();

			if(L_u32Iterator == ARG_u32ByteCount-2)
			{
				MI2C_voidDisableACK();
			}
			else
			{
				/*MISRA-C*/
			}
		}

		/*Send Stop Condition*/
		MI2C_voidSendStopCondition();
	}
	else
	{
		L_stderrState = E_NOK;
	}

	return L_stderrState;
}
