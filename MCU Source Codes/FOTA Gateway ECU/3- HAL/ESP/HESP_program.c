/*
 * HESP_program.c
 *
 *  Created on: Sep 26, 2023
 *      Author: fady3
 */
#include <stdio.h>
#include <LBIT_math.h>
#include <LSTD_types.h>


#include <MSTK_interface.h>
#include <MNVIC_interface.h>
#include <MRCC_interface.h>
#include <MGPIO_interface.h>
#include <MUART_interface.h>
#include <string.h>

#include <HESP_interface.h>
#include <HESP_private.h>


u8 HESP_u8Timeout = 0;
u16 HESP_u8Iterator = 0;
char HESP_charBuffer[400]={0};

STD_error_t HESP_stderrInit(u8 ARG_u8UARTID, u32 ARG_u32BaudRate)
{
	/*State Local Variable*/
	STD_error_t L_stderrState = E_OK;

	/*Check for Valid UART ID*/
	if(ARG_u8UARTID <= MUART_UART3)
	{
		/*Initialize Chosen UART*/
		switch(ARG_u8UARTID)
		{
		case MUART_UART1:
		{
			/*Enable Clock For UART and GPIO*/
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART1);

			/*Set UART Pin Modes*/
			MGPIO_stderrSetPinMode(MGPIO_PIN_A9, MGPIO_MODE_AF_PUSHPULL_2MHZ);
			MGPIO_stderrSetPinMode(MGPIO_PIN_A10, MGPIO_MODE_INPUT_FLOATING);

			/*Enable NVIC UART Interrupt*/
			MNVIC_stderrEnableIRQ(USART1);

			/*Set UART Interrupt Priority*/
			MNVIC_stderrSetPriority(USART1, 12);

			/*Initialize SysTick TImer*/
			MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);

			/*Initialize UART*/
			MUART_stderrInit(MUART_UART1, ARG_u32BaudRate, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
			MUART_voidEnableTx(MUART_UART1);
			MUART_voidEnableRx(MUART_UART1);
			MUART_stderrEnableInterrupt(MUART_UART1, MUART_INTTERUPT_RXC);
			MUART_stderrSetCallback(HESP_writeToBuffer, MUART_UART1, MUART_INTTERUPT_RXC);
			break;
		}
		case MUART_UART2:
		{
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART2);

			MGPIO_stderrSetPinMode(MGPIO_PIN_A2, MGPIO_MODE_AF_PUSHPULL_2MHZ);
			MGPIO_stderrSetPinMode(MGPIO_PIN_A3, MGPIO_MODE_INPUT_FLOATING);

			MNVIC_stderrEnableIRQ(USART2);
			MNVIC_stderrSetPriority(USART2, 12);
			MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);

			MUART_stderrInit(MUART_UART2, ARG_u32BaudRate, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
			MUART_voidEnableTx(MUART_UART2);
			MUART_voidEnableRx(MUART_UART2);
			MUART_stderrEnableInterrupt(MUART_UART2, MUART_INTTERUPT_RXC);
			MUART_stderrSetCallback(HESP_writeToBuffer, MUART_UART2, MUART_INTTERUPT_RXC);
			break;
		}
		case MUART_UART3:
		{
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
			MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART3);

			MGPIO_stderrSetPinMode(MGPIO_PIN_B10, MGPIO_MODE_AF_PUSHPULL_2MHZ);
			MGPIO_stderrSetPinMode(MGPIO_PIN_B11, MGPIO_MODE_INPUT_FLOATING);

			MNVIC_stderrEnableIRQ(USART3);
			MNVIC_stderrSetPriority(USART3, 12);
			MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);

			MUART_stderrInit(MUART_UART3, ARG_u32BaudRate, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
			MUART_voidEnableTx(MUART_UART3);
			MUART_voidEnableRx(MUART_UART3);
			MUART_stderrEnableInterrupt(MUART_UART3, MUART_INTTERUPT_RXC);
			MUART_stderrSetCallback(HESP_writeToBuffer, MUART_UART3, MUART_INTTERUPT_RXC);
			break;
		}
		default :L_stderrState = E_NOK;
		}
	}
	else
	{
		L_stderrState = E_NOK;
	}

	return L_stderrState;
}

/*ESP Function to send a command to the ESP*/
static void HESP_voidSendCommand(const char *ARG_pccharCommand)
{
	/*Send the string over UART*/
	MUART_voidSendString(MUART_UART1, ARG_pccharCommand);
	MUART_voidSendString(MUART_UART1, "\r\n");
}

/*Call Back Function For SysTick Timer*/
static void HESP_voidTimeout(void)
{
	HESP_u8Timeout = 1;
}

/*Call Back Function For UART*/
void HESP_writeToBuffer(void)
{
	if(HESP_u8Iterator <400)
	{
		/*Get Data from UART without Polling (Check the RXC flag with an if condition instead of busy wait)*/
		HESP_charBuffer[HESP_u8Iterator] = MUART_voidRecieveCharBlocking(MUART_UART1);
		HESP_u8Iterator++;
	}
	else
	{
		/*Reset Iterator and overwrite*/
		HESP_u8Iterator = 0;
	}

}

/*Flush ESP Buffer*/
void HESP_voidFlushBuffer(void)
{
	/*Flush Buffer*/
	memset(HESP_charBuffer,0,sizeof(HESP_charBuffer));

	/*Reset Iterator*/
	HESP_u8Iterator = 0;
}


/*ESP Function to get response from ESP within a specific timeout*/
static HESP_status HESP_hespstatusGetResponse(u16 ARG_u16Timeout, u8 ARG_u8ResponseType)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Reset Timeout Status*/
	HESP_u8Timeout = 0;


	/*Start SysTick*/
	MSTK_stderrStartSingle(ARG_u16Timeout * 1000, HESP_voidTimeout);


	/*Loop untill timeout occurs or recieve response*/
	for(;;)
	{
		/*
		 *Break if response has reached its end identifying it with \r\nOK\r\n or \r\nERROR\r\n as a terminator.
		 *We check for Iterator not being zero and null charachter to avoid wasting time looking for the
		 *Termination String each time
		 */
		if(ARG_u8ResponseType == HESP_RESPONSE_TYPE_SYSTEM)
		{
			if((strstr(HESP_charBuffer, "OK") != NULL_POINTER) || (strstr(HESP_charBuffer, "ERROR") != NULL_POINTER))
			{
				MSTK_voidStop();
				L_hespstatusState = HESP_RESPONSE_RECIEVED;
				break;
			}
			else
			{
				/*MISRA*/
			}
		}
		else if(ARG_u8ResponseType == HESP_RESPONSE_TYPE_WIFI)
		{
			if((strstr(HESP_charBuffer, "OK") != NULL_POINTER) || (strstr(HESP_charBuffer, "FAIL") != NULL_POINTER))
			{
				MSTK_voidStop();
				L_hespstatusState = HESP_RESPONSE_RECIEVED;
				break;
			}
			else
			{
				/*MISRA*/
			}
		}
		else if(ARG_u8ResponseType == HESP_RESPONSE_TYPE_SERVER)
		{

			if(strstr(HESP_charBuffer, "EOT") != NULL_POINTER || strstr(HESP_charBuffer, "SEND OK"))
			{
				MSTK_voidStop();
				L_hespstatusState = HESP_RESPONSE_RECIEVED;
				break;
			}
			else
			{
				/*MISRA*/
			}
		}
		else
		{
			/*MISRA*/
		}

		/*Check if timeout has occured*/
		if(HESP_u8Timeout)
		{

			/*Report Error and break if timout occurs*/
			L_hespstatusState = HESP_RESPONSE_TIMEOUT;

			MSTK_voidStop();
			break;
		}
		else
		{
			/*MISRA*/
		}


	}

	/*Return ESP State*/
	return L_hespstatusState;
}

/*ESP Function That sends a command and expects a response within a specific timeout*/
static HESP_status HESP_hespstatusSendAndExpectResponse(const char *ARG_pccharCommand, const char *ARG_pccharExpectedResponse, u16 ARG_u16Timeout, u8 ARG_u8ResponseType)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Flush The Buffer*/
	HESP_voidFlushBuffer();

	/*Send The command to the ESP*/
	HESP_voidSendCommand(ARG_pccharCommand);

	/*Read the response*/
	L_hespstatusState = HESP_hespstatusGetResponse(ARG_u16Timeout, ARG_u8ResponseType);

	/*Check for timeout*/
	if(L_hespstatusState == HESP_RESPONSE_RECIEVED)
	{
		/*If all went well go ahead and compare the response with the expected response passed to the function*/
		if(strstr(HESP_charBuffer, ARG_pccharExpectedResponse) != NULL_POINTER)
		{
			/*ESP state ok if expected response is found within ESP total Response*/
			L_hespstatusState = HESP_RESPONSE_EXPECTED;
		}
		else
		{
			/*ESP State is not ok if expected response is not found within ESP total Response*/
			L_hespstatusState = HESP_RESPONSE_UNEXPECTED;
		}
	}
	else
	{
		/*State is Timeout (MISRA)*/
	}

	/*Return ESP State*/
	return L_hespstatusState;
}


HESP_status HESP_hespstatusCheckActive(void)
{
	/*Error State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Get Response and wait for a timeout of 100 ms*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT", "OK", 100, HESP_RESPONSE_TYPE_SYSTEM);

	switch(L_hespstatusState)
	{
	case HESP_RESPONSE_EXPECTED  : L_hespstatusState = HESP_STATE_ACTIVE; break;
	case HESP_RESPONSE_UNEXPECTED: L_hespstatusState = HESP_STATE_INACTIVE; break;
	default                      : L_hespstatusState = HESP_RESPONSE_TIMEOUT; break; /*MISRA*/
	}

	return L_hespstatusState;
}


/*ESP Function to Get AT Version*/
HESP_status HESP_hespstatusGetVersion(char *ARG_pcharVersion)
{
	/*Error State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Pointer for storing start address of AT version in the recieved response from ESP*/
	char *L_pcharVersionStartIndex = NULL_POINTER;

	/*Get Response and wait for a timeout of 100 ms*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+GMR", "AT version:", 200, HESP_RESPONSE_TYPE_SYSTEM);

	/*Check for timeout*/
	if(L_hespstatusState == HESP_RESPONSE_EXPECTED)
	{
		/*if everything went okay go ahead and index the part of the response that has the version*/
		L_pcharVersionStartIndex = strstr(HESP_charBuffer,"AT ver");
		u8 L_u8Iterator=0;

		for(L_u8Iterator=0; L_u8Iterator<6; L_u8Iterator++)
		{
			/*We start Copying from offset 11 to skip the "AT version:" Part in the response and get the number only*/
			ARG_pcharVersion[L_u8Iterator]=L_pcharVersionStartIndex[L_u8Iterator+11];
		}

		L_hespstatusState = HESP_VERSION_AVAILABLE;
	}
	else if(L_hespstatusState == HESP_RESPONSE_UNEXPECTED)
	{
		L_hespstatusState = HESP_VERSION_UNAVAILABLE;
	}
	else
	{
		/*State is Timeout (MISRA)*/
	}

	/*Return ESP State*/
	return L_hespstatusState;
}

HESP_status HESP_hespstatusSetOperationMode(u8 ARG_u8OperationMode)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Get Response and wait for a timeout of 100 ms*/
	switch(ARG_u8OperationMode)
	{
	case HESP_OP_MODE_STATION: L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CWMODE=1", "OK", 200, HESP_RESPONSE_TYPE_SYSTEM); break;
	case HESP_OP_MODE_AP     : L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CWMODE=2", "OK", 200, HESP_RESPONSE_TYPE_SYSTEM); break;
	case HESP_OP_MODE_BOTH   : L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CWMODE=3", "OK", 200, HESP_RESPONSE_TYPE_SYSTEM); break;
	default                  : L_hespstatusState = HESP_OP_MODE_INVALID; break;
	}

	if(L_hespstatusState != HESP_OP_MODE_INVALID)
	{
		switch(L_hespstatusState)
		{
		case HESP_RESPONSE_EXPECTED  : L_hespstatusState = HESP_OP_MODE_SET; break;
		case HESP_RESPONSE_UNEXPECTED: L_hespstatusState = HESP_OP_MODE_NOT_SET; break;
		default                      : L_hespstatusState = HESP_RESPONSE_TIMEOUT; break; /*MISRA*/
		}
	}
	else
	{
		/*MISRA*/
	}

	/*Return ESP State*/
	return L_hespstatusState;
}

HESP_status HESP_hespstatusConnectToWIFI(const char *ARG_pccharssid, const char *ARG_pccharPassword)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*String to combine SSID and Password with command*/
	char L_combinedCommand[50];

	/*Pointer for storing start address of parsing required part in response*/
	char *L_pcharStartIndex = NULL_POINTER;

	/*Combine SSID and Password into one command*/
	sprintf(L_combinedCommand, "AT+CWJAP=\"%s\",\"%s\"", ARG_pccharssid, ARG_pccharPassword);

	/*Send Combined Command And Expect OK Response, Timeout after 10 Seconds*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse(L_combinedCommand, "OK", 16000, HESP_RESPONSE_TYPE_WIFI);

	/*Check if response is the response is the expected one, if true set state to WIFI Connected*/
	if(L_hespstatusState == HESP_RESPONSE_EXPECTED)
	{
		L_hespstatusState = HESP_WIFI_CONNECTED;
	}
	else if(L_hespstatusState == HESP_RESPONSE_UNEXPECTED)
	{
		/*Index the error report string in the response in case of connection failure*/
		L_pcharStartIndex = strstr(HESP_charBuffer, "+CWJAP:");

		/*if the error response exists go ahead and determine what is the error code*/
		if(L_pcharStartIndex != NULL_POINTER)
		{
			/*check error code within the body of the response from ESP (7 is the offset from error name)*/
			switch(L_pcharStartIndex[7])
			{
			case '1': L_hespstatusState = HESP_WIFI_FAILED_TO_CONNECT; break;
			case '3': L_hespstatusState = HESP_WIFI_AP_NOT_FOUND; break;
			default: L_hespstatusState = HESP_WIFI_FAILED_TO_CONNECT;
			}
		}
		else
		{

			/*set to unknown response if none of the previous conditions match the received response*/
			L_hespstatusState = HESP_RESPONSE_UNKOWN;
		}
	}
	else
	{
		MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_HIGH);
		/*State is Timeout (MISRA)*/
	}

	return L_hespstatusState;
}


HESP_status HESP_hespstatusQueryState(void)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Pointer for storing start address of parsing required part in response*/
	char *L_pcharStartIndex = NULL_POINTER;

	/*Current State Variable*/
	char L_statusCode = '\0';

	/*Send Combined Command And Expect OK Response, Timeout after 10 Seconds*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CIPSTATUS", "OK", 1000, HESP_RESPONSE_TYPE_SYSTEM);

#ifdef UART_DEBUG
	MUART_voidSendString(MUART_UART2, HESP_charBuffer);
#endif

	/*Check if response is the response is the expected one*/
	if(L_hespstatusState == HESP_RESPONSE_EXPECTED)
	{
		/*Get the starting position of the status string*/
		L_pcharStartIndex = strstr(HESP_charBuffer, "STATUS:");

		/*Check if the status string exists*/
		if(L_pcharStartIndex != NULL_POINTER)
		{
			/*Get The status code*/
			L_statusCode = L_pcharStartIndex[7];

			switch(L_statusCode)
			{
			case '0': L_hespstatusState = HESP_STATE_INACTIVE;      break;
			case '1': L_hespstatusState = HESP_WIFI_NOT_CONNECTED;  break;
			case '2': L_hespstatusState = HESP_WIFI_CONNECTED;      break;
			case '3': L_hespstatusState = HESP_SERVER_CONNECTED;    break;
			case '4': L_hespstatusState = HESP_SERVER_DISCONNECTED; break;
			case '5': L_hespstatusState = HESP_WIFI_NOT_CONNECTED;  break;
			default : L_hespstatusState = HESP_RESPONSE_UNKOWN;     break;
			}
		}
		else
		{
			L_hespstatusState = HESP_RESPONSE_UNKOWN;
		}
	}
	else if(L_hespstatusState == HESP_RESPONSE_UNEXPECTED)
	{
		L_hespstatusState = HESP_RESPONSE_UNKOWN;
	}
	else
	{
		/*State is Timeout (MISRA)*/
	}

	return L_hespstatusState;
}


HESP_status HESP_hespstatusSetConnectionMode(u8 ARG_u8ConnectionMode)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Get Response and wait for a timeout of 100 ms*/
	switch(ARG_u8ConnectionMode)
	{
	case HESP_CONNECTION_MODE_SINGLE  : L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CIPMUX=0", "OK", 500, HESP_RESPONSE_TYPE_SYSTEM); break;
	case HESP_CONNECTION_MODE_MULTIPLE: L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CIPMUX=1", "OK", 500, HESP_RESPONSE_TYPE_SYSTEM); break;
	default                           : L_hespstatusState = HESP_RESPONSE_UNEXPECTED; break;
	}

	switch(L_hespstatusState)
	{
	case HESP_RESPONSE_EXPECTED  : L_hespstatusState = HESP_CONN_MODE_SET; break;
	case HESP_RESPONSE_UNEXPECTED: L_hespstatusState = HESP_CONN_MODE_NOT_SET; break;
	default                      : L_hespstatusState = HESP_RESPONSE_TIMEOUT; break; /*MISRA*/
	}

	/*Return ESP State*/
	return L_hespstatusState;
}


HESP_status HESP_hespstatusConnectToServer(u8 ARG_u8LinkID,u8 ARG_u8Protocol, const char *ARG_pccharHost, u16 ARG_u8Port)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*String to combine command*/
	char L_combinedCommand[100];

	/*Send Command if Configuration is Valid*/
	if((ARG_u8LinkID < HESP_SERVER_MAX_CONNECTIONS) && (ARG_u8Protocol <= HESP_SERVER_PROTOCOL_SSL) && (ARG_u8Port < HESP_SERVER_MAX_PORT_NO))
	{
		/*Combine Arguments into one command*/
		if(ARG_u8LinkID == HESP_CONNECTION_MODE_SINGLE)
		{
			switch(ARG_u8Protocol)
			{
			case HESP_SERVER_PROTOCOL_TCP: sprintf(L_combinedCommand, "AT+CIPSTART=\"TCP\",\"%s\",%hu", ARG_pccharHost, ARG_u8Port); break;
			case HESP_SERVER_PROTOCOL_UDP: sprintf(L_combinedCommand, "AT+CIPSTART=\"UDP\",\"%s\",%hu", ARG_pccharHost, ARG_u8Port); break;
			case HESP_SERVER_PROTOCOL_SSL: sprintf(L_combinedCommand, "AT+CIPSTART=\"SSL\",\"%s\",%hu", ARG_pccharHost, ARG_u8Port); break;
			default                      : L_hespstatusState = HESP_SERVER_INVALID_PROTOCOL; break; /*MISRA*/
			}
		}
		else
		{
			switch(ARG_u8Protocol)
			{
			case HESP_SERVER_PROTOCOL_TCP: sprintf(L_combinedCommand, "AT+CIPSTART=%d,\"TCP\",\"%s\",%hu", ARG_u8LinkID, ARG_pccharHost, ARG_u8Port); break;
			case HESP_SERVER_PROTOCOL_UDP: sprintf(L_combinedCommand, "AT+CIPSTART=%d,\"UDP\",\"%s\",%hu", ARG_u8LinkID, ARG_pccharHost, ARG_u8Port); break;
			case HESP_SERVER_PROTOCOL_SSL: sprintf(L_combinedCommand, "AT+CIPSTART=%d,\"SSL\",\"%s\",%hu", ARG_u8LinkID, ARG_pccharHost, ARG_u8Port); break;
			default                      : L_hespstatusState = HESP_SERVER_INVALID_PROTOCOL; break; /*MISRA*/
			}
		}

		/*Send Combined Command And Expect OK Response, Timeout after 5 Seconds*/
		L_hespstatusState = HESP_hespstatusSendAndExpectResponse(L_combinedCommand, "OK", 10000, HESP_RESPONSE_TYPE_SYSTEM);
	}
	else
	{
		L_hespstatusState = HESP_SERVER_INVALID_CONFIGURATION;
	}



	if(L_hespstatusState != HESP_SERVER_INVALID_CONFIGURATION)
	{
		switch(L_hespstatusState)
		{
		case HESP_RESPONSE_EXPECTED  : L_hespstatusState = HESP_SERVER_CONNECTED; break;
		case HESP_RESPONSE_UNEXPECTED: L_hespstatusState = HESP_SERVER_FAILED_TO_CONNECT; break;
		default                      : L_hespstatusState = HESP_RESPONSE_TIMEOUT; break; /*MISRA*/
		}
	}
	else
	{
		/*MISRA*/
	}

	return L_hespstatusState;
}

HESP_status HESP_hespstatusDisconnectServer(void)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;

	/*Send the data to the server*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse("AT+CIPCLOSE", "OK", 5000, HESP_RESPONSE_TYPE_SYSTEM);

	/**/
	if(L_hespstatusState == HESP_RESPONSE_EXPECTED)
	{

		L_hespstatusState = HESP_SERVER_DISCONNECTED;
	}
	else if(L_hespstatusState == HESP_RESPONSE_UNEXPECTED)
	{
		L_hespstatusState = HESP_RESPONSE_UNKOWN;
	}
	else
	{
		/*State is Timeout (MISRA)*/
	}

	return L_hespstatusState;
}

HESP_status HESP_hespstatusSendToServer(const char *ARG_pccharCommand, u16 ARG_u16CommandTimeout)
{
	/*ESP State Local Variable*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;


	/*String to combine command*/
	char L_combinedCommand[50];

	/*Send number of bytes to be sent to the server*/
	sprintf(L_combinedCommand,"AT+CIPSEND=%d", strlen(ARG_pccharCommand));

	/*Send the data to the server*/
	L_hespstatusState = HESP_hespstatusSendAndExpectResponse(L_combinedCommand, "OK", 500, HESP_RESPONSE_TYPE_SYSTEM);

	if(L_hespstatusState == HESP_RESPONSE_EXPECTED)
	{

		L_hespstatusState = HESP_hespstatusSendAndExpectResponse(ARG_pccharCommand, "SEND OK", ARG_u16CommandTimeout, HESP_RESPONSE_TYPE_SERVER);

		switch(L_hespstatusState)
		{
		case HESP_RESPONSE_EXPECTED   : L_hespstatusState = HESP_SERVER_SEND_SUCCESS; break;
		case HESP_RESPONSE_UNEXPECTED : L_hespstatusState = HESP_SERVER_SEND_FAIL; break;
		default                       : L_hespstatusState = HESP_RESPONSE_TIMEOUT; break; /*MISRA*/
		}

	}
	else if(L_hespstatusState == HESP_RESPONSE_UNEXPECTED)
	{
		L_hespstatusState = HESP_SERVER_SEND_FAIL;
	}
	else
	{
		/*State is Timeout (MISRA)*/
	}

	return L_hespstatusState;
}

void HESP_voidGetServerResponse(char *ARG_pccharResponse)
{
	strncpy(ARG_pccharResponse, HESP_charBuffer, HESP_u8Iterator);
	ARG_pccharResponse[HESP_u8Iterator] = '\0';
	ARG_pccharResponse[HESP_u8Iterator+1] = '\0';
	HESP_voidFlushBuffer();
}

/*This function is used to check if the buffer is empty or not empty*/
HESP_status HESP_stderrCheckIfBufferIsEmpty(void)
{
	/*This variable is used to return if the buffer is empty or not*/
	HESP_status L_hespstatusState = HESP_RESPONSE_WAITING;
	/*If the iterator is not equal to 0 this means that the buffer is not empty*/
	if(HESP_u8Iterator>0)
	{
		L_hespstatusState =  HESP_BUFFER_NOT_EMPTY;
	}
	else
	{
		L_hespstatusState =  HESP_BUFFER_EMPTY;
	}
	return L_hespstatusState;
}




