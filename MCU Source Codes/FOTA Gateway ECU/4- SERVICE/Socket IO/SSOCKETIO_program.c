/*
 * SSOCKETIO_program.c
 *
 *  Created on: Oct 30, 2023
 *      Author: fady3
 */

#include <LBIT_math.h>
#include <LSTD_types.h>


#include <MRCC_interface.h>
#include <MSTK_interface.h>
#include <MNVIC_interface.h>
#include <MGPIO_interface.h>
#include <MTIM2_interface.h>
#include <MUART_interface.h>

#include <HESP_interface.h>
#include <SSOCKETIO_interface.h>
#include <SSOCKETIO_private.h>
#include <string.h>
#include <stdio.h>


void (*SSOCKETIO_pvoidfEvents[SSOCKETIO_MAX_NUMBER_OF_EVENTS])(const char *ARG_pccharEventData);
char SSOCKETIO_charEventNames[SSOCKETIO_MAX_NUMBER_OF_EVENTS][SSOCKETIO_MAX_EVENT_NAME_LENGTH];
u8 SSOCKETIO_u8CounterForPTF = 0 ;

extern u8 currentState;

void SSOCKETIO_voidInit(void)
{
	/*TIMER 2 Initialization*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_TIM2);
	MTIM2_voidInit();
	MTIM2_voidEnableInterrupt();

	/*Interrupt Configurations*/
	MNVIC_stderrEnableIRQ(TIM2);
	MNVIC_stderrSetPriority(TIM2, 13);

}

void SSOCKETIO_voidListen(void)
{
	/*Enable Periodic Event Listener*/
	MTIM2_stderrStartPeriodic(50000, SSOCKETIO_voidExecuteEvents);
}

static SSOCKETIO_status SSOCKETIO_socketiostatusHandshake(void)
{
	/*State Variables for Socket IO and ESP*/
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;
	HESP_status L_hespstatusstate = HESP_RESPONSE_WAITING;

	/*Buffer to Load ESP Received Data Into*/
	char L_stringToCheck[401];

	/*Send Socket IO Upgrade Request*/
	L_hespstatusstate = HESP_hespstatusSendToServer("GET /socket.io/?EIO=4&transport=websocket HTTP/1.1\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\nSec-WebSocket-Key: 1Mrz+ZvS7YP5Sd/RjSdwXQ==\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nHost: 2.tcp.eu.ngrok.io:18328\r\n\r\n", 2000);

	/*Check for server send success state*/
	if(L_hespstatusstate == HESP_SERVER_SEND_SUCCESS)
	{
		/*Wait for 500 ms so that the response is received*/
		MSTK_voidBusyWait(500000);

		/*Read Server Response*/
		HESP_voidGetServerResponse(L_stringToCheck);

		/*Check for Switching Protocols within the response*/
		if(strstr(L_stringToCheck, "Switching Protocols") != NULL_POINTER)
		{
			/*Set State to Handshake Success*/
			L_ssocketiostatusState = SSOCKETIO_HANDSHAKE_SUCCESS;

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Handshake Success\n");
			MUART_voidSendString(MUART_UART2, "DEBUG: ");
			MUART_voidSendString(MUART_UART2, L_stringToCheck);
			MUART_voidSendChar(MUART_UART2, '\n');
#endif
		}
		else
		{
			/*Set State to Handshake Fail*/
			L_ssocketiostatusState = SSOCKETIO_HANDSHAKE_FAIL;

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "DEBUG: ");
			MUART_voidSendString(MUART_UART2, L_stringToCheck);
			MUART_voidSendChar(MUART_UART2, '\n');
#endif
		}
	}
	else
	{

#ifdef UART_DEBUG
		/*Dump Error Code to UART (For Debugging Purposes)*/
		MUART_voidSendString(MUART_UART2, "Error Code: ");
		MUART_voidSendChar(MUART_UART2, L_hespstatusstate+'0');
		MUART_voidSendChar(MUART_UART2, '\n');
#endif

		L_ssocketiostatusState = SSOCKETIO_HANDSHAKE_FAIL;
	}

	return L_ssocketiostatusState;
}

STD_error_t SSOCKETIO_stderrEncodeSocketIOFrame(u8* ARG_pu8frame, u16* ARG_pu16frameSize, const char *ARG_pcchEventName, const char *ARG_pcchEventData, u8 ARG_u8FrameType)
{
	/*State Variable*/
	STD_error_t L_stderrState = E_OK;

	/*Frame Construction Variables*/
	u8* L_pu8payloadData = NULL_POINTER;
	u16 L_u16payloadLen = 0;
	u32 L_messageLength = 0;
	char L_charConcatenated[100]={0};

	/*Check For NULL Input*/
	if(ARG_pu8frame != NULL_POINTER && ARG_pu16frameSize != NULL_POINTER && ARG_pcchEventName != NULL_POINTER && ARG_pcchEventData != NULL_POINTER)
	{
		/*Check for frame type and compose frame message accordingly*/
		if(ARG_u8FrameType == SSOCKETIO_FRAME_CONNECT)
		{
			strcpy(L_charConcatenated, "40");
		}
		else if(ARG_u8FrameType == SSOCKETIO_FRAME_DISCONNECT)
		{
			strcpy(L_charConcatenated, "41");
		}
		else if(ARG_u8FrameType == SSOCKETIO_FRAME_EVENT)
		{
			sprintf(L_charConcatenated, "42[\"%s\",\"%s\"]",ARG_pcchEventName, ARG_pcchEventData);
		}
		else
		{
			/*Report Invalid Frame Type*/
			L_stderrState = E_NOK;
		}

		/*Calculate Total Message Length*/
		L_messageLength = strlen(L_charConcatenated);

		/*Check for message length ranges to determine how many bits does it fit in*/
		if (L_messageLength < 126)
		{
			/*Payload length fits in 7 bits so add 2 bytes only to the frame size one for the message length and one for the Encoded Start Byte*/
			*ARG_pu16frameSize = 2 + L_messageLength;

			/*Add The message length byte*/
			ARG_pu8frame[1] = (u8)L_messageLength;

			/*Set The start of data after the message length byte*/
			L_pu8payloadData = &ARG_pu8frame[2];

			/*Payload length equals to total message length*/
			L_u16payloadLen = L_messageLength;
		}
		else if (L_messageLength < 65536)
		{
			/*Payload length fits in 16 bits so add 4 bytes only to the frame size 3 for the message length and one for the Encoded Start Byte*/
			*ARG_pu16frameSize = 4 + L_messageLength;

			/*Add The message length bytes*/
			ARG_pu8frame[1] = 126;
			ARG_pu8frame[2] = (u8)((L_messageLength >> 8) & 0xFF);
			ARG_pu8frame[3] = (u8)(L_messageLength & 0xFF);

			/*Set The start of data after the message length bytes*/
			L_pu8payloadData = &ARG_pu8frame[4];

			/*Payload length equals to total message length*/
			L_u16payloadLen = L_messageLength;
		}
		else
		{
			/*Payload length fits in 64 bits so add 4 bytes only to the frame size 9 for the message length and one for the Encoded Start Byte*/
			*ARG_pu16frameSize = 10 + L_messageLength;

			/*Add The message length bytes*/
			ARG_pu8frame[1] = 127;
			/* Big-endian byte order for 64-bit payload length*/
			for (u8 i = 0; i < 8; i++) {
				ARG_pu8frame[2 + i] = (u8)((L_messageLength >> ((7 - i) * 8)) & 0xFF);
			}

			/*Set The start of data after the message length bytes*/
			L_pu8payloadData = &ARG_pu8frame[10];

			/*Payload length equals to total message length*/
			L_u16payloadLen = L_messageLength;
		}

		/*FIN bit and Opcode for text frame*/
		ARG_pu8frame[0] = 0x81;

		/*Copy the message data into the frame Payload*/
		memcpy(L_pu8payloadData, L_charConcatenated, L_u16payloadLen);

		/*Add The null terminator*/
		*(L_pu8payloadData + strlen(L_charConcatenated)) = '\0';
	}
	else
	{
		/*Report Null pointer*/
		L_stderrState = E_NULL_POINTER;
	}

	/*Return State*/
	return L_stderrState;
}

SSOCKETIO_status SSOCKETIO_socketiostatusConnect(const char *ARG_pcchHostName, u16 ARG_u16Port)
{
	/*State Variables for Socket IO and ESP*/
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;
	HESP_status L_hespstatusstate = HESP_RESPONSE_WAITING;

	/*Buffer to Load ESP Received Data Into*/
	char L_stringToCheck[401];

	/*Checking for NULL input*/
	if(ARG_pcchHostName != NULL_POINTER)
	{
		/*Try to Connect to Server using TCP*/
		L_hespstatusstate = HESP_hespstatusConnectToServer(HESP_CONNECTION_MODE_SINGLE, HESP_SERVER_PROTOCOL_TCP, ARG_pcchHostName, ARG_u16Port);

		/*Check for server connection*/
		if(L_hespstatusstate == HESP_SERVER_CONNECTED)
		{
#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "TCP Connected\n");
#endif
			/*Start Handshake Procedure in case of server connection success*/
			L_ssocketiostatusState = SSOCKETIO_socketiostatusHandshake();

			/*Check for handshake success*/
			if(L_ssocketiostatusState == SSOCKETIO_HANDSHAKE_SUCCESS)
			{
				/*Send Connect Packet in case of handshake success*/
				SSOCKETIO_socketiostatusEmit("connect","");

				/*Wait for 500 ms so that the response is received*/
				MSTK_voidBusyWait(500000);

				/*Read Server Response*/
				HESP_voidGetServerResponse(L_stringToCheck);

				/*Check if Connected*/
				if(strstr(L_stringToCheck, "Server connected") != NULL_POINTER)
				{
					L_ssocketiostatusState = SSOCKETIO_CONNECT_SUCCESS;
				}
				else
				{
					L_ssocketiostatusState = SSOCKETIO_CONNECT_FAIL;
				}
			}
			else
			{
				/*Report Failure in case of handshake fail*/
				L_ssocketiostatusState = SSOCKETIO_HANDSHAKE_FAIL;
			}
		}
		else
		{
#ifdef UART_DEBUG
			/*Dump Error Code to UART (For Debugging Purposes)*/
			MUART_voidSendString(MUART_UART2, "Error Code: ");
			MUART_voidSendChar(MUART_UART2, L_hespstatusstate+'0');
			MUART_voidSendChar(MUART_UART2, '\n');
#endif
			/*Report Server failing to connect*/
			L_ssocketiostatusState = SSOCKETIO_TCP_CONNECT_FAIL;
		}
	}
	else
	{
		/*Report invalid configuration*/
		L_ssocketiostatusState = SSOCKETIO_INVALID_CONFIG;
	}

	/*Return Status*/
	return L_ssocketiostatusState;
}


SSOCKETIO_status SSOCKETIO_socketiostatusEmit(const char *ARG_pcchEventName, const char *ARG_pcchEventData)
{
	/*Socket IO and ESP State Variables*/
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;
	HESP_status L_hespstatusstate = HESP_RESPONSE_WAITING;

	/*Frame Construction Variables*/
	u8 L_u8frame[256] = {0};
	u16 L_u8frameSize = 0;
	u8 ARG_u8FrameType = 0;

	/*Check For NULL Input*/
	if(ARG_pcchEventName != NULL_POINTER && ARG_pcchEventData != NULL_POINTER)
	{
		/*Check For Frame type*/
		if(!strcmp(ARG_pcchEventName, "connect"))
		{
			ARG_u8FrameType = SSOCKETIO_FRAME_CONNECT;
		}
		else if(!strcmp(ARG_pcchEventName, "disconnect"))
		{
			ARG_u8FrameType = SSOCKETIO_FRAME_DISCONNECT;
		}
		else
		{
			ARG_u8FrameType = SSOCKETIO_FRAME_EVENT;
		}

		/*Encode Socket IO Frame*/
		SSOCKETIO_stderrEncodeSocketIOFrame(L_u8frame, &L_u8frameSize, ARG_pcchEventName, ARG_pcchEventData, ARG_u8FrameType);

		/*Send Encoded Socket IO Frame to Server*/
		L_hespstatusstate = HESP_hespstatusSendToServer(L_u8frame, 1000);

		/*Check for server send success*/
		if(L_hespstatusstate == HESP_SERVER_SEND_SUCCESS)
		{
			/*Report Emission Success*/
			L_ssocketiostatusState = SSOCKETIO_EMIT_SUCCESS;
		}
		else
		{
			/*Report Emission Failure*/
			L_ssocketiostatusState = SSOCKETIO_EMIT_FAIL;
		}

	}
	else
	{
		L_ssocketiostatusState = SSOCKETIO_INVALID_CONFIG;
	}

	/*Return System State*/
	return L_ssocketiostatusState;
}


SSOCKETIO_status SSOCKETIO_socketiostatusOnEvent(const char *ARG_pcchEventName, void (*ARG_pvoidfEventFunction)(const char *ARG_pccharEventData))
{
	/*Socket IO State Variable*/
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;

	/*Index for event*/
	s8 	L_u8CheckVar = -1;

	/*Check For Null Input*/
	if(ARG_pcchEventName != NULL_POINTER && ARG_pvoidfEventFunction != NULL_POINTER && SSOCKETIO_u8CounterForPTF < SSOCKETIO_MAX_NUMBER_OF_EVENTS && strlen(ARG_pcchEventName) <= SSOCKETIO_MAX_EVENT_NAME_LENGTH)
	{
		/*Search for the event in the event list*/
		L_u8CheckVar = SSOCKETIO_s8SearchForEvent(ARG_pcchEventName, SSOCKETIO_charEventNames);

		/*Check if not found*/
		if(L_u8CheckVar == -1)
		{
			/*Add The Event Callback function to the array of pointers to function*/
			SSOCKETIO_pvoidfEvents[SSOCKETIO_u8CounterForPTF] = ARG_pvoidfEventFunction;

			/*Add The Event Name to the array of strings*/
			strcpy(SSOCKETIO_charEventNames[SSOCKETIO_u8CounterForPTF], ARG_pcchEventName);

			/*Increment the number of events*/
			SSOCKETIO_u8CounterForPTF++;

			/*Report Event Add Success*/
			L_ssocketiostatusState = SSOCKETIO_EVENT_ADD_SUCCESS;

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Event add success\n");
#endif

		}
		else if(L_u8CheckVar != -1)
		{
			/*If the event name already exists just its corresponding call back function*/
			SSOCKETIO_pvoidfEvents[L_u8CheckVar] = ARG_pvoidfEventFunction;

			/*Report Event Add Success*/
			L_ssocketiostatusState = SSOCKETIO_EVENT_ADD_SUCCESS;
		}
		else
		{
			/*MISRA*/
		}
	}
	else
	{
		/*Report Event Add Failure*/
		L_ssocketiostatusState = SSOCKETIO_EVENT_ADD_FAIL;
	}
	return L_ssocketiostatusState;

}





/*This function is used to check if the buffer has an event or not */
static SSOCKETIO_status SSOCKETIO_socketiostatusCheckInBufferForEvent(const char *ARG_pccharBuffer)
{
	/*Socket IO State Variable*/
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;

	/*We only check on the 2nd & 3rd character*/
	if(ARG_pccharBuffer[2] == '4' && ARG_pccharBuffer[3]=='2')
	{
		/*Report that there is an event in the buffer*/
		L_ssocketiostatusState =  SSOCKETIO_HAS_EVENT;
	}
	else
	{
		/*Report that no event exits in buffer*/
		L_ssocketiostatusState =  SSOCKETIO_NO_EVENT;
	}

	/*Return State*/
	return L_ssocketiostatusState;
}


/*Function to extract the event name from the buffer*/
STD_error_t SSOCKETIO_stderrGetEventName(const char *ARG_pccharBuffer, char *ARG_pcharEventName)
{
	/*State Variable*/
	STD_error_t L_stderrState = E_OK;

	/*Counter for max event name length Check*/
	u16 L_u16Counter = 6;

	/*Check for Null Input*/
	if(ARG_pcharEventName != NULL_POINTER && ARG_pccharBuffer != NULL_POINTER)
	{
		/*If yes copy the buffer to the string till you find a (") */
		do
		{

			*ARG_pcharEventName = ARG_pccharBuffer[L_u16Counter];
			ARG_pcharEventName++;
			L_u16Counter++;
		}
		while((ARG_pccharBuffer[L_u16Counter]!='\"') && (L_u16Counter < SSOCKETIO_MAX_EVENT_NAME_LENGTH + 5));

		/*Add Null Terminator*/
		*ARG_pcharEventName = '\0';

		/*Check if the event name is invalid*/
		if(L_u16Counter >= SSOCKETIO_MAX_EVENT_NAME_LENGTH + 5)
		{
			L_stderrState = E_NOK;
		}
		else
		{
			/*MISRA*/
		}
	}
	else
	{
		/*Report Null input*/
		L_stderrState = E_NULL_POINTER;
	}

	return L_stderrState;
}


/*Function to extract the event data from the buffer*/
STD_error_t SSOCKETIO_stderrGetEventData(const char *ARG_pccharBuffer, char* ARG_pccharEvenData)
{
	/*State Variable*/
	STD_error_t L_stderrStatus = E_OK;

	/*Counter for max event data length Check*/
	u16 L_u16CounterForBuffer = 0;

	/*Check For NULL Input*/
	if(ARG_pccharBuffer != NULL_POINTER && ARG_pccharEvenData != NULL_POINTER)
	{
		/*Search for the event name and data separator (",") */
		ARG_pccharBuffer = strstr(ARG_pccharBuffer, "\",\"");

		/*Check if found*/
		if(ARG_pccharBuffer != NULL_POINTER)
		{
			/*Move the pointer to the start of the data*/
			ARG_pccharBuffer+=3;

			/*Copy the data until the (")*/
			do
			{
				*ARG_pccharEvenData = *ARG_pccharBuffer;
				ARG_pccharEvenData++;
				ARG_pccharBuffer++;
				L_u16CounterForBuffer++;

			}while(*ARG_pccharBuffer !='\"' && L_u16CounterForBuffer <= SSOCKETIO_MAX_EVENT_DATA_SIZE);

			/*Add The Null Terminator*/
			*ARG_pccharEvenData='\0';

			if(L_u16CounterForBuffer >= SSOCKETIO_MAX_EVENT_DATA_SIZE)
			{
				/*Report Excess Data*/
				L_stderrStatus = E_NOK;
			}
			else
			{
				/*MISRA-C*/
			}
		}
		else
		{
			/*Report Null input*/
			L_stderrStatus = E_NULL_POINTER;
		}
	}
	else
	{
		/*Report Null input*/
		L_stderrStatus = E_NULL_POINTER;
	}

	/*Return State*/
	return L_stderrStatus;
}

/*This function searches for an event within the array of strings containing event names*/
s8 SSOCKETIO_s8SearchForEvent(const char* ARG_pccharEventName, const char ARG_charEventNames[][20])
{
	/*Index Variable*/
	s8 L_s8Index = -1;

	/*Loop Over the event names array*/
	for (u8 i = 0; (i < SSOCKETIO_MAX_NUMBER_OF_EVENTS) && (L_s8Index == -1); i++)
	{
		/*Save index if found*/
		if (strcmp(ARG_charEventNames[i], ARG_pccharEventName) == 0)
		{
			L_s8Index = i;
		}
		else
		{
			/*MISRA*/
		}
	}

	/*Return Index*/
	return L_s8Index;
}




void SSOCKETIO_voidExecuteEvents(void)
{
	/*State Variables*/
	STD_error_t L_stderrState = E_OK;
	SSOCKETIO_status L_ssocketiostatusState = SSOCKETIO_WAITING;


	/*Local Buffer to Store ESP Response if any was found*/
	char L_charBuffer[401]={0};

	/*Data Start Index because ESP Response has some extra headers prior to the actual response*/
	char *L_pcharDataStartIndex = NULL_POINTER;

	/*Local Array to store event name*/
	char L_charEventName[SSOCKETIO_MAX_EVENT_NAME_LENGTH]={0};

	/*Local Array to store event data*/
	char L_charEventData[SSOCKETIO_MAX_EVENT_DATA_SIZE]={0};

	/*Local Variable to store event index if found*/
	s8 L_s8EventIndex = 0;

	/*Check if there is any data in the ESP Buffer*/
	if(HESP_stderrCheckIfBufferIsEmpty() == HESP_BUFFER_NOT_EMPTY)
	{
#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "\n\n\nBuffer Not Empty\n");
#endif

		/*Get The Data from the ESP buffer to the Local Buffer*/
		HESP_voidGetServerResponse(L_charBuffer);

		/*Search for the response start index*/
		L_pcharDataStartIndex = strstr(L_charBuffer, ":");

		/*Check if the response start index was found and it doesn't equal a null value*/
		if(L_pcharDataStartIndex != NULL_POINTER)
		{
			/*Increment by one so that it points to the first character in the response because strstr returns a pointer to the found string which is before the response by one character*/
			L_pcharDataStartIndex++;

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, L_pcharDataStartIndex);
			MUART_voidSendString(MUART_UART2, "\n");
#endif

			/*Check if the data in the buffer has an event or not because it may be data for anything else such as an ESP command Response which is not an interest for us*/
			L_ssocketiostatusState = SSOCKETIO_socketiostatusCheckInBufferForEvent(L_pcharDataStartIndex);

			/*If there is an event in the buffer start by getting its name*/
			if(L_ssocketiostatusState == SSOCKETIO_HAS_EVENT)
			{

#ifdef UART_DEBUG
				MUART_voidSendString(MUART_UART2, "Socket has event\n");
#endif

				/*Get the event name*/
				L_stderrState = SSOCKETIO_stderrGetEventName(L_pcharDataStartIndex, L_charEventName);

#ifdef UART_DEBUG
				MUART_voidSendString(MUART_UART2, "Event Name: ");
				MUART_voidSendString(MUART_UART2, L_charEventName);
				MUART_voidSendString(MUART_UART2, "\n");
#endif

				/*If there are no errors search for the event name in the stored event list*/
				if(L_stderrState == E_OK)
				{
					/*Return event index if found and -1 if not found*/
					L_s8EventIndex = SSOCKETIO_s8SearchForEvent(L_charEventName, SSOCKETIO_charEventNames);

					/*Execute the event if found and its function pointer doesn't equal Null*/
					if((L_s8EventIndex != -1) && (SSOCKETIO_pvoidfEvents[L_s8EventIndex] != NULL_POINTER))
					{
						L_stderrState = SSOCKETIO_stderrGetEventData(L_pcharDataStartIndex, L_charEventData);

						if(L_stderrState == E_OK)
						{
#ifdef UART_DEBUG
							MUART_voidSendString(MUART_UART2, "Executing Event\n");
#endif
							SSOCKETIO_pvoidfEvents[L_s8EventIndex](L_charEventData);
						}
						else
						{
							SSOCKETIO_pvoidfEvents[L_s8EventIndex](NULL_POINTER);
#ifdef UART_DEBUG
							MUART_voidSendString(MUART_UART2, "Error, Event Data: ");
							MUART_voidSendString(MUART_UART2, L_charEventData);
							MUART_voidSendString(MUART_UART2, "\n");
#endif
						}

					}
					else
					{
#ifdef UART_DEBUG
						MUART_voidSendString(MUART_UART2, "Error, Not Found: ");
						MUART_voidSendString(MUART_UART2, L_charEventData);
						MUART_voidSendString(MUART_UART2, "\n");
#endif
					}
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
		}
		else
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "DEBUG:\n");
			MUART_voidSendString(MUART_UART2, L_charBuffer);
			MUART_voidSendString(MUART_UART2, "DEBUG END\n");
#endif

			if(strstr(L_charBuffer, "CLOS") != NULL_POINTER || strstr(L_charBuffer, "ERROR") != NULL_POINTER)
			{
				currentState = HESP_SERVER_DISCONNECTED;
			}
//			else if(strstr(L_charBuffer, "OK") != NULL_POINTER || strstr(L_charBuffer, "42[\"") != NULL_POINTER)
//			{
//
//			}
//			else
//			{
//				currentState = HESP_hespstatusQueryState();
//			}
		}
	}
	else
	{

	}
}


SSOCKETIO_status SSOCKETIO_socketiostatusHeartbeat(void)
{
	/*Socket IO Stauts Variable*/
	SSOCKETIO_status L_socketiostatusState = SSOCKETIO_WAITING;

	/*Local buffer to store server response*/
	char L_charBuffer[401] = {'\0'};

	/*Disable listening for events as this is a critical section*/
	MTIM2_voidDisableTimer();

	/*Send a heartbeat to the server to check the liveness of the connection*/
	SSOCKETIO_socketiostatusEmit("heartbeat", "");

	/*Wait for 500 ms*/
	MSTK_voidBusyWait(500000);

	/*Read Server Response*/
	HESP_voidGetServerResponse(L_charBuffer);

#ifdef UART_DEBUG
	MUART_voidSendString(MUART_UART2, L_charBuffer);
#endif

	/*Check if the server responded with an alive message*/
	if(strstr(L_charBuffer, "alive") != NULL_POINTER)
	{
		/*If the connection is still alive go ahead and re-enable the event listener and set the state to server listening*/
		L_socketiostatusState = SSOCKETIO_CONNECT_SUCCESS;
	}
	else
	{
		/*If the connection is dead go ahead and try to reconnect to the Socket IO Server*/
		L_socketiostatusState = SSOCKETIO_CONNECT_FAIL;
	}

	/*return the socket io server state*/
	return L_socketiostatusState;
}


