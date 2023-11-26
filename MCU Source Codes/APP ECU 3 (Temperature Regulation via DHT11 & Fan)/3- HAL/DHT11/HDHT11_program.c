/*
 * HDHT11_program.c
 *
 *  Created on: Nov 4, 2023
 *      Author: fady3
 */


#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MRCC_interface.h>
#include <MGPIO_interface.h>
#include <MSTK_interface.h>

#include <HDHT11_private.h>
#include <HDHT11_interface.h>


STD_error_t HDHT11_stderrInit(u8 ARG_u8SensorPin)
{
	STD_error_t L_stderrState = E_OK;

	if(ARG_u8SensorPin >= MGPIO_PIN_A0 && ARG_u8SensorPin <= MGPIO_PIN_A15 && ARG_u8SensorPin != MGPIO_PIN_A13 && ARG_u8SensorPin != MGPIO_PIN_A14)
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	}
	else if(ARG_u8SensorPin >= MGPIO_PIN_B0 && ARG_u8SensorPin <= MGPIO_PIN_B15 && ARG_u8SensorPin != MGPIO_PIN_B2)
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	}
	else if(ARG_u8SensorPin >= MGPIO_PIN_C13 && ARG_u8SensorPin <= MGPIO_PIN_C15)
	{
		MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOC);
	}
	else
	{
		L_stderrState = E_NOK;
	}

	return L_stderrState;
}

HDHT11_status HDHT11_hdht11statusReadData(HDHT11_data *ARG_phdhtdataData, u8 ARG_u8SensorPin)
{
	/*Sensor State Variable*/
	HDHT11_status L_hdht11statusState = HDHT11_WAITING;

	/*Set Timeout to zero because this is a new data request*/
	HDHT11_u8Timeout = 0;

	/*Request data from DHT*/
	HDHT11_voidRequest(ARG_u8SensorPin);

	/*Check for response*/
	HDHT11_voidResponse(ARG_u8SensorPin);

	/*Check for timeout in the initialization process*/
	if(!HDHT11_u8Timeout)
	{
		/*
		 * Start receiving the 5 bytes in respective order:
		 * 1- Humidity Integer
		 * 2- Humidity Decimal
		 * 3- Temperature Integer
		 * 4- Temperature Decimal
		 * 5- Data Checksum
		 */
		for (int i=0; i<5 && L_hdht11statusState != HDHT11_TIMEOUT; i++)
		{
			/*If a timeout occurs in receiving any of the bytes set state to timeout to prevent further time wasting*/
			if(HDHT11_u8Timeout)
			{
				L_hdht11statusState = HDHT11_TIMEOUT;
			}
			else
			{
				ARG_phdhtdataData->dataArray[i] = HDHT11_u8receiveData(ARG_u8SensorPin);
			}
		}

		/*If no timeouts occurred then set state to Data ready*/
		if(L_hdht11statusState != HDHT11_TIMEOUT)
		{
			L_hdht11statusState = HDHT11_DATA_READY;
		}
		else
		{
			/*MISRA-C*/
		}
	}
	else
	{
		L_hdht11statusState = HDHT11_TIMEOUT;
	}

	/*Return a pointer to the receieved Data*/
	return L_hdht11statusState;
}


/*MCU send start pulse or request */
static void HDHT11_voidRequest(u8 DHT11_pin)
{
	/*Set the pin mode to output*/
	MGPIO_stderrSetPinMode(DHT11_pin, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);

	/*Pull data line low*/
	MGPIO_stderrSetPinState(DHT11_pin, MGPIO_LOW);

	/* wait for 20ms */
	MSTK_voidBusyWait(20000);

	/*Pull data line high*/
	MGPIO_stderrSetPinState(DHT11_pin,MGPIO_HIGH);
}

/*Receive response from DHT11*/
static void HDHT11_voidResponse(u8 DHT11_pin)
{
	u8 L_u8PinState = 1;

	/*Set the pin mode to input*/
	MGPIO_stderrSetPinMode(DHT11_pin, MGPIO_MODE_INPUT_FLOATING);

	/*Receive Response Sequence (Wait for 40us + 10us tolerance for the response to start)*/
	MSTK_stderrStartSingle(50, HDHT11_voidTimeout);
	while(L_u8PinState == MGPIO_HIGH && !HDHT11_u8Timeout)
	{
		MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);
	}
	MSTK_voidStop();

	/*Wait for 80us + 10us tolerance for the first part of the DHT Response*/
	MSTK_stderrStartSingle(90, HDHT11_voidTimeout);
	while(L_u8PinState == MGPIO_LOW && !HDHT11_u8Timeout)
	{
		MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);
	}
	MSTK_voidStop();

	/*Wait for 80us + 10us tolerance for the second part of the DHT Response*/
	MSTK_stderrStartSingle(90, HDHT11_voidTimeout);
	while(L_u8PinState == MGPIO_HIGH && !HDHT11_u8Timeout)
	{
		MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);
	}
	MSTK_voidStop();

}

/*Receive data from DHT11*/
static u8 HDHT11_u8receiveData(u8 DHT11_pin)
{
	u8 L_u8Byte=0;
	u8 L_u8PinState = 0;

	/*Loop for Eight Bits*/
	for (int i=0; i<8 && !HDHT11_u8Timeout; i++)
	{

		/*Wait for 50us + 10us tolerance for the transmission process to start*/
		MSTK_stderrStartSingle(60, HDHT11_voidTimeout);
		while(L_u8PinState == 0 && !HDHT11_u8Timeout)
		{
			MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);
		}
		MSTK_voidStop();

		/*Wait for 30us then check the voltage level*/
		MSTK_voidBusyWait(30);

		/*Read the pin state*/
		MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);

		/*If high pulse is greater than 30us then its a 1 bit*/
		if(L_u8PinState == MGPIO_HIGH)
		{
			L_u8Byte = (L_u8Byte<<1)|(0x01);
		}
		else
		{
			/*Otherwise it's a 0 bit*/
			L_u8Byte = (L_u8Byte<<1);
		}

		/*Wait for the remaining high pulse time to pass in order to start another bit receiving*/
		MSTK_stderrStartSingle(60, HDHT11_voidTimeout);
		while(L_u8PinState == MGPIO_HIGH && !HDHT11_u8Timeout)
		{
			MGPIO_stderrGetPinState(&L_u8PinState, DHT11_pin);
		}
		MSTK_voidStop();

	}

	/*Return the received byte*/
	return L_u8Byte;
}

void HDHT11_voidTimeout(void)
{
	HDHT11_u8Timeout = 1;
}




