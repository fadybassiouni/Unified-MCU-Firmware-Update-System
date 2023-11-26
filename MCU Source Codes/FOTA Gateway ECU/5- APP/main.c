#include <LBIT_math.h>
#include <LSTD_types.h>
#include <MRCC_interface.h>
#include <MSTK_interface.h>
#include <MNVIC_interface.h>
#include <MGPIO_interface.h>
#include <MTIM2_interface.h>
#include <MUART_interface.h>
#include <CAN_interface.h>
#include <MFPEC_interface.h>
#include <HESP_interface.h>
#include <SSOCKETIO_interface.h>
#include <string.h>
#include <stdio.h>

#define SSID "Paradox"
#define PASSWORD "tryagainlaterin2020"

/*Define if to use Local network or a remote NGROK Server*/
#define USE_LOCAL 1

#if (USE_LOCAL == 1)

#define SERVER "192.168.1.4"
#define PORT   55555

#else

#define SERVER "4.tcp.eu.ngrok.io"
#define PORT   17888


#endif


/*System State Variable*/
u8 currentState=0;


/*System Flash Variables*/
u8 UMFUS_u8FlashFlag = 0;
u8 UMFUS_u8FlashTimeout = 0;
u16 UMFUS_u16FlashCounter = 0;
char UMFUS_charECUID = '9';

char UMFUS_charHexHashEnd[48] = ":00000001FF#";

/*System Timeout Function*/
void UMFUS_voidTimeout(void);


/*Event Handlers prototypes*/
void E_Flash(const char *ARG_pccharEventData);
void E_connectCar(const char *ARG_pccharEventData);
void E_connectECU(const char *ARG_pccharEventData);


/*********************************************************************CAN***********************************************************************************/



u8 gDATA[50]={0}; /* buffer to receive*/
/* will be base address from flash lets say 0x0800A000 */
u8 *g_pu8BaseAddress=(u8*)0x08004800;

void CAN_TX(u8* ARG_pu8BaseAddress, u8 ARG_u8DLCValue,u16 ARG_u16ID )
{
	CAN_TxHeaderTypeDef tx_header;

	tx_header.StdId = ARG_u16ID;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	/* assign the DLC value */
	tx_header.DLC = ARG_u8DLCValue;
	tx_header.TransmitGlobalTime=DISABLE;
	CAN_voidAddTxMsg(&tx_header, ARG_pu8BaseAddress);
}

void CAN_RX(void)
{
	CAN_RxHeaderTypeDef Rx_header;

	while(CAN_voidRXPending(CAN_RX_FIFO0) == 0 && !UMFUS_u8FlashTimeout);
	CAN_voidGetRxMsg(CAN_RX_FIFO0,&Rx_header,gDATA);


}

void CAN_TXRecord(u8* ARG_pu8BaseAddress,u8 ARG_u8ECU)
{
	u8 i=0 , counter=0  ;
	u16 dummyDelay = 0;
	u16  ID=0;
	/* this func will transmit 8 byte from record in frame if the terminator found then send the length */
	/* just count the number of DLC in last frame */
	/* note the terminator will be  '\0' */
	switch (ARG_u8ECU){
	case 1 :
		ID = 0x65D;
		break;
	case 2 :
		ID = 0x75D;
		break;
	case 3:
		ID = 0x85D;
		break;
	default:break;
	}
	while(1 && !UMFUS_u8FlashTimeout)
	{
		for( i= 0; ARG_pu8BaseAddress[i] != '\0'; i++)
		{
			counter ++;
			if( counter == 8 )
			{
				while(dummyDelay--);
				dummyDelay = 4000;
				//MSTK_voidBusyWait(1000);
				/* transmit data and counter=0 */
				CAN_TX( &ARG_pu8BaseAddress[i - 7 ] , counter ,ID);
				counter=0 ;
			}
		}
		/* to send the last part of record */
		if(counter != 0 )
		{
			/* we will call func CAN func TX it will transmit the frame */
			CAN_TX( &ARG_pu8BaseAddress[ i - counter ] , counter, ID );
		}
		else
		{
			/* do nothing */
		}
		CAN_RX();

		if (strcmp(gDATA, "done") == 0)
		{
			break;
		}
		counter=0;
		ARG_pu8BaseAddress += 48;
	}
}



u16 Std_id_High(u16 local_u16Std_Id)
{
	return (local_u16Std_Id << 5);
}
void CAN_FilterConfig(void)
{
	CAN_FilterTypeDef CAN_FilterInit;
	CAN_FilterInit.FilterActivation = ENABLE;
	CAN_FilterInit.FilterBank = 0;
	CAN_FilterInit.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterInit.FilterIdHigh = Std_id_High(0x55D);
	CAN_FilterInit.FilterIdLow = 0x0000;
	CAN_FilterInit.FilterMaskIdHigh = 0x0000;
	CAN_FilterInit.FilterMaskIdLow = 0x0000;
	CAN_FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;

	CAN_voidConfigFilter(&CAN_FilterInit);
}

void CAN_Interrupt(void)
{
	//	CAN_voidInitInterput(CAN_TX_IRQn);
	//	CAN_voidInitInterput(CAN_RX0_IRQn);
	//	CAN_voidInitInterput(CAN_RX1_IRQn);
	//	CAN_voidInitInterput(CAN_SCE_IRQn);

	CAN_voidEnableInterrupt(CAN_IER_TX_MAILBOX_EMPTY | CAN_IER_RX_FIFO0_MSG_PENDING | CAN_IER_ERROR | CAN_IER_LAST_ERROR_CODE | CAN_IER_BUSOFF);
}



void CAN_RxFifo0MsgPendingCallback()
{
	CAN_RX();
	// GPIO_voidSetPinValue(GPIOA, PIN1, HIGH);
	//NVIC_voidSetPendingFlag(CAN_RX0_IRQn);
}

/*********************************************************************CAN***********************************************************************************/


int main(void)
{
	/*System Clock Initialization*/
	MRCC_stderrInitSystemClock(MRCC_SYSCLOCK_HSE_CRYSTAL);

	/*System Buses Clock Initialization*/
	MRCC_stderrInitBusClock(MRCC_BUS_APB2, MRCC_APB2_NO_PRESCALAR);
	MRCC_stderrInitBusClock(MRCC_BUS_APB1, MRCC_APB1_NO_PRESCALAR);

	/*Enable Clock Security System*/
	MRCC_voidEnableCSS();

	/*Enable Clock For Peripherals*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_CAN);

#ifdef UART_DEBUG
	/*Enable Clock For Peripheral*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART2);

	/*UART2 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A2, MGPIO_MODE_AF_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A3, MGPIO_MODE_INPUT_FLOATING);

	/*UART2 Initialization*/
	MUART_stderrInit(MUART_UART2, 0, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
	MUART_voidEnableTx(MUART_UART2);
	MUART_voidEnableRx(MUART_UART2);
#endif


	/*CAN Initialization*/
	CAN_voidInit();

	/*CAN Filter set*/
	CAN_FilterConfig();

	CAN_Interrupt();

	/*CAN Start*/
	CAN_voidStart();

	/*ECU Reset Pins Initialization*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A4, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A5, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A6, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);

	/*Set NVIC Grouping Mode*/
	MNVIC_stderrSetGroupingMode(MNVIC_GROUPING_16G_0S);

	/*ESP Initialize*/
	HESP_stderrInit(MUART_UART1, 115200);

	/*Socket IO Hardware Initialize*/
	SSOCKETIO_voidInit();

	/*Socket IO Initialize Event Callbacks*/
	SSOCKETIO_socketiostatusOnEvent("E_Flash", E_Flash);
	SSOCKETIO_socketiostatusOnEvent("E_connectCar", E_connectCar);
	SSOCKETIO_socketiostatusOnEvent("E_connectECU", E_connectECU);

	/*Check that ESP is up and running*/
	currentState =  HESP_hespstatusQueryState();;




	for(;;)
	{

		switch(currentState)
		{

		case HESP_STATE_ACTIVE:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "ESP Active!\n");
#endif

			/*Set ESP Operation Mode To Station*/
			currentState = HESP_hespstatusSetOperationMode(HESP_OP_MODE_STATION);
			break;
		}

		case HESP_STATE_INACTIVE:
		{
			MSTK_voidBusyWait(1000000);

			currentState = HESP_hespstatusCheckActive();
			break;
		}

		case HESP_OP_MODE_SET:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Mode Set Success\n");
			MUART_voidSendString(MUART_UART2, "WIFI Connecting\n");
#endif

			/*Try Connecting to WIFI*/
			currentState = HESP_hespstatusConnectToWIFI(SSID, PASSWORD);
			break;
		}

		case HESP_WIFI_CONNECTED:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "WIFI Connected\n");
#endif

			/*Set Server Connection Type to single*/
			currentState = HESP_hespstatusSetConnectionMode(HESP_CONNECTION_MODE_SINGLE);
			break;
		}

		case HESP_WIFI_NOT_CONNECTED:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "WIFI Not Connected\n");
			MUART_voidSendString(MUART_UART2, "WIFI Reconnecting\n");
#endif

			/*Retry to connect to WIFI*/
			MTIM2_voidDisableTimer();
			currentState = HESP_hespstatusConnectToWIFI(SSID, PASSWORD);
			break;
		}

		case HESP_WIFI_AP_NOT_FOUND:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "WIFI AP Not Found\n");
			MUART_voidSendString(MUART_UART2, "WIFI Reconnecting\n");
#endif

			/*Retry to connect to WIFI*/
			currentState = HESP_hespstatusConnectToWIFI(SSID, PASSWORD);
			break;
		}

		case HESP_WIFI_FAILED_TO_CONNECT:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "WIFI Failed to Connect\n");
			MUART_voidSendString(MUART_UART2, "WIFI Reconnecting\n");
#endif
			/*Retry to connect to WIFI*/
			currentState = HESP_hespstatusConnectToWIFI(SSID, PASSWORD);
			break;
		}

		case HESP_CONN_MODE_SET:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Connection Type: Single\n");
			MUART_voidSendString(MUART_UART2, "Connecting to Socket IO Server\n");
#endif

			/*Connect to Socket IO Server*/
			currentState = SSOCKETIO_socketiostatusConnect(SERVER, PORT);
			break;
		}

		case HESP_CONN_MODE_NOT_SET:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Connection Mode Not Set\n");
#endif

			currentState = HESP_STATE_INACTIVE;
			break;
		}

		case HESP_SERVER_DISCONNECTED:
		{

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Reconnecting to Socket IO Server\n");
#endif

			/*Disable Event Listener*/
			MTIM2_voidDisableTimer();

			/*Reconnect to SocketIO Server*/
			currentState = SSOCKETIO_socketiostatusConnect(SERVER, PORT);
			break;
		}

		case SSOCKETIO_TCP_CONNECT_FAIL:
		{
			/*Query ESP State*/
			currentState = HESP_hespstatusQueryState();
			break;
		}

		case SSOCKETIO_HANDSHAKE_FAIL:
		{
			/*Query ESP State*/
			currentState = HESP_hespstatusQueryState();
			break;
		}

		case SSOCKETIO_CONNECT_FAIL:
		{
			/*Disconnect TCP*/
			currentState = HESP_hespstatusDisconnectServer();

			break;
		}

		case SSOCKETIO_CONNECT_SUCCESS:
		{
			SSOCKETIO_voidListen();
			currentState = SSOCKETIO_SERVER_LISTENING;
			break;
		}

		case SSOCKETIO_SERVER_LISTENING:
		{
			/*Listening for events in background*/
			break;
		}

		case HESP_SERVER_CONNECTED:
		{
			currentState = SSOCKETIO_socketiostatusHeartbeat();
			break;
		}

		case HESP_RESPONSE_TIMEOUT:
		{
			/*Disable Event Listener*/
			MTIM2_voidDisableTimer();

			MSTK_voidBusyWait(1000000);

			currentState = HESP_hespstatusQueryState();

			if(currentState == HESP_RESPONSE_TIMEOUT)
			{
				currentState = HESP_STATE_INACTIVE;
			}
			else
			{
				/*MISRA-C*/
			}

			break;
		}
		}
	}
}



void E_connectCar(const char *ARG_pccharEventData)
{
#ifdef UART_DEBUG
	MUART_voidSendString(MUART_UART2, ARG_pccharEventData);
#endif

	if(strstr(ARG_pccharEventData, "xV1uEk56s") != NULL_POINTER)
	{
		SSOCKETIO_socketiostatusEmit("E_connectCar", "Connection Accepted");

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Car Connected\n");
#endif

	}
	else
	{
		SSOCKETIO_socketiostatusEmit("E_connectCar", "Bad Token");

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Bad Token\n");
#endif
	}
}

void E_connectECU(const char *ARG_pccharEventData)
{

#ifdef UART_DEBUG
	MUART_voidSendString(MUART_UART2, ARG_pccharEventData);
#endif


	if(strstr(ARG_pccharEventData, "Connect to ECU") != NULL_POINTER)
	{
		UMFUS_charECUID = ARG_pccharEventData[15];

		if(UMFUS_charECUID >= '1' && UMFUS_charECUID <='3')
		{
			/*Send Reset Signal Over EXTI*/
			switch(UMFUS_charECUID)
			{
			case '1':
				MGPIO_stderrSetPinState(MGPIO_PIN_A4 ,MGPIO_LOW);
				MGPIO_stderrSetPinState(MGPIO_PIN_A4 ,MGPIO_HIGH);
				break;
			case '2':
				MGPIO_stderrSetPinState(MGPIO_PIN_A5 ,MGPIO_LOW);
				MGPIO_stderrSetPinState(MGPIO_PIN_A5 ,MGPIO_HIGH);
				break;
			case '3':
				MGPIO_stderrSetPinState(MGPIO_PIN_A6 ,MGPIO_LOW);
				MGPIO_stderrSetPinState(MGPIO_PIN_A6 ,MGPIO_HIGH);
				break;
			}

			/*Wait for the ECU to enter The bootloader mode and send OK*/
			MSTK_stderrStartSingle(5000000, UMFUS_voidTimeout);
			CAN_RX();
			MSTK_voidStop();

			/*Check if timeout has occured*/
			if(!UMFUS_u8FlashTimeout)
			{
				/*check for the acknowledge from the ECU if correct reply to the server if not respond with something else*/
				if(strstr(gDATA, "OK") != NULL_POINTER)
				{
#ifdef UART_DEBUG
					MUART_voidSendString(MUART_UART2, "ECU Connected\n");
#endif
					SSOCKETIO_socketiostatusEmit("E_connectECU", "ECU Connected");

				}
				else
				{
#ifdef UART_DEBUG
					MUART_voidSendString(MUART_UART2, "Invalid Connect Request\n");
#endif
					SSOCKETIO_socketiostatusEmit("E_connectECU", "Invalid Connect Request");
				}

			}
			else
			{
				/*Reset Timeout Status*/
				UMFUS_u8FlashTimeout = 0;
			}

			/*Reset EXTI Pin*/
			switch(UMFUS_charECUID)
			{
			case '1':
				MGPIO_stderrSetPinState(MGPIO_PIN_A4 ,MGPIO_LOW);
				break;
			case '2':
				MGPIO_stderrSetPinState(MGPIO_PIN_A5 ,MGPIO_LOW);
				break;
			case '3':
				MGPIO_stderrSetPinState(MGPIO_PIN_A6 ,MGPIO_LOW);
				break;
			}
		}
		else
		{
			SSOCKETIO_socketiostatusEmit("E_connectECU", "Invalid ECU ID");

#ifdef UART_DEBUG
			MUART_voidSendString(MUART_UART2, "Invalid ECU ID\n");
#endif

		}

	}
	else
	{
		SSOCKETIO_socketiostatusEmit("E_connectECU", "Invalid Connect Request");

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Invalid Connect Request\n");
#endif

	}


}



void E_Flash(const char *ARG_pccharEventData)
{

	char ecuFlashingStatus[100] = {0};
	char *HESP_pcharHEXStartIndex = NULL_POINTER;
	char *HESP_pcharEndIndex = NULL_POINTER;
	char *HESP_pcharDone = NULL_POINTER;

#ifdef UART_DEBUG
	MUART_voidSendString(MUART_UART2, "Record: ");
	MUART_voidSendString(MUART_UART2, ARG_pccharEventData);
	MUART_voidSendString(MUART_UART2, "\n");
#endif

	HESP_pcharHEXStartIndex = strstr(ARG_pccharEventData, "::");
	HESP_pcharEndIndex = strstr(ARG_pccharEventData, "-EOT");
	HESP_pcharDone = strstr(ARG_pccharEventData, "Done");

	if(UMFUS_u8FlashFlag == 0 && strstr(ARG_pccharEventData, "ready ?") != NULL_POINTER)
	{

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Erasing Flash");
#endif

		/*Erase Flash*/
		for(u8 i=0; i<30; i++)
		{
			MFPEC_voidFlashPageErase(18+i);
		}


		SSOCKETIO_socketiostatusEmit("E_Flash", "start");
		UMFUS_u8FlashFlag = 1;
		UMFUS_u16FlashCounter = 0;
	}

	else if(HESP_pcharHEXStartIndex != NULL_POINTER && HESP_pcharEndIndex != NULL_POINTER)
	{

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Record Valid\n");
#endif

		/*Packet Format for CAN on the receiving end*/
		HESP_pcharHEXStartIndex++;
		*HESP_pcharEndIndex = '\0';
		//		*(HESP_pcharEndIndex + 1) = '\0';
		//		*(HESP_pcharEndIndex + 2) = '\0';
		//		*(HESP_pcharEndIndex + 3) = '\0';
		//		*(HESP_pcharEndIndex + 4) = '\0';

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Flashing...\n");
#endif

		MFPEC_voidFlashWrite(0x08004800 + UMFUS_u16FlashCounter, (u16 *)(HESP_pcharHEXStartIndex), 48/2);
		UMFUS_u16FlashCounter+=48;

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Flashed Record\n");
#endif

		SSOCKETIO_socketiostatusEmit("E_Flash", "next");

		UMFUS_u8FlashFlag = 1;
	}
	else if(HESP_pcharDone != NULL_POINTER)
	{

#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Done Flashing\n");
#endif


		/*Send Flashing Status Update (Master has flashed)*/
		SSOCKETIO_socketiostatusEmit("E_ECU_Flash", "Master Flashed");



		//		/*Wait until timeout for the Backup Start message from the ECU over CAN*/
		//		MSTK_stderrStartSingle(1000000, UMFUS_voidTimeout);
		//		CAN_RX();
		//		MSTK_voidStop();

		//		/*Check ECU Backup status*/
		//		if(!UMFUS_u8FlashTimeout && (strstr(gDATA, "BS_OK") != NULL_POINTER))
		//		{
		/*Form ECU Flashing Status Response Message*/
		sprintf(ecuFlashingStatus, "ECU %c Backing up", UMFUS_charECUID);

		/*Send Flashing Status Update (ECU Will Start to Backup)*/
		SSOCKETIO_socketiostatusEmit("E_ECU_Flash", ecuFlashingStatus);
		//		}
		//		else
		//		{
		/*Send Flashing Status Update (Flash Failure)*/
		//SSOCKETIO_socketiostatusEmit("E_ECU_FLASH", "ECU Flashing Error, Restoring from backup");
		//		}


		//		/*Wait for the Backup confirmation and flashing start message from the ECU over CAN*/
		//		MSTK_stderrStartSingle(16000000, UMFUS_voidTimeout);
		//		CAN_RX();
		//		MSTK_voidStop();

		//		/*Check ECU Backup status*/
		//		if(!UMFUS_u8FlashTimeout && (strstr(gDATA, "B_OK") != NULL_POINTER))
		//		{
		/*Form ECU Flashing Status Response Message*/
		sprintf(ecuFlashingStatus, "ECU %c started Flashing", UMFUS_charECUID);

		/*Send Flashing Status Update (ECU Finished Backup and started flashing)*/
		SSOCKETIO_socketiostatusEmit("E_ECU_Flash", ecuFlashingStatus);
		//		}
		//		else
		//		{
		//			/*Send Flashing Status Update (Flash Failure)*/
		//			SSOCKETIO_socketiostatusEmit("E_ECU_FLASH", "ECU Flashing Error, Restoring from backup");
		//		}

		/*Send the Hex over CAN to be flashed by the bootloader*/
		MSTK_stderrStartSingle(16000000, UMFUS_voidTimeout);
		CAN_TXRecord((u8*)g_pu8BaseAddress, UMFUS_charECUID - 48);
		MSTK_voidStop();

		if(!UMFUS_u8FlashTimeout)
		{
			/*Form ECU Flashing Status Response Message*/
			sprintf(ecuFlashingStatus, "ECU %c Flashed", UMFUS_charECUID);

			/*Send Flashing Status Update (ECU has successfully flashed)*/
			SSOCKETIO_socketiostatusEmit("E_ECU_Flash", ecuFlashingStatus);
		}
		else
		{
			/*Send Flashing Status Update (Flash Failure)*/
			SSOCKETIO_socketiostatusEmit("E_ECU_FLASH", "ECU Flashing Error, Restoring from backup");
		}

		/*Reset Flashing Variables*/
		UMFUS_u16FlashCounter = 0;
		UMFUS_u8FlashFlag = 0;
		UMFUS_u8FlashTimeout = 0;
	}
	else
	{
#ifdef UART_DEBUG
		MUART_voidSendString(MUART_UART2, "Record Invalid\n");
#endif
		/*Request the invalid record again*/
		SSOCKETIO_socketiostatusEmit("E_Flash", "again");
	}

}



void UMFUS_voidTimeout(void)
{
	UMFUS_u8FlashTimeout = 1;
}





