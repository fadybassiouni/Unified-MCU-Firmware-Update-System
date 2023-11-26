/************************************/
/*	Author	  :	Elamir Galal		*/
/*	SWC		  : TIMER				*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	November 5, 2023	*/
/*	Last Edit :						*/
/************************************/



#ifndef _MTIMER_INTERFACE_H_
#define _MTIMER_INTERFACE_H_






/*************	Timer Numbers	 *************/
#define MTIM1			1
#define MTIM2			2
#define MTIM3			3
#define MTIM4			4
#define MTIM5			5
#define MTIM9			6
#define MTIM10			7
#define MTIM11			8
/*************	Triggering Edge	 *************/
#define MTIM_EDGE_TRIGGER_FALLING		1
#define MTIM_EDGE_TRIGGER_RISING		2
#define MTIM_EDGE_TRIGGER_BOTH			3
/*************	Channel IDs	 *************/
#define MTIM_CHANNEL1	1
#define MTIM_CHANNEL2	2
#define MTIM_CHANNEL3	3
#define MTIM_CHANNEL4	4
/*************	Interrupts 	 *************/
#define MTIM_INT_UPDATE_ENABLE		0
#define MTIM_INT_CC1				1
#define MTIM_INT_CC2				2
#define MTIM_INT_CC3				3
#define MTIM_INT_CC4				4

/*************	Func Prototypes	 *************/
STD_error_t MTIM3_stderrTimerInit(u8 ARG_u8MTIMChannel, u16 ARG_u16Prescaler);
STD_error_t MTIM3_stderrEdgeTrigger(u8 ARG_u8MTIMChannel, u8 ARG_u8Trigger);
STD_error_t MTIM3_stderrEnableICU(u8 ARG_u8MTIMChannel);
STD_error_t MTIM3_stderrInterruptEnable(u8 ARG_u8Interrupt);
STD_error_t MTIM3_stderrInterruptDisable(u8 ARG_u8Interrupt);
STD_error_t MTIM3_stderrSetCallBack(void(*PtrtoFunction)(void));
void        MTIM3_voidTimerStart(void);
void        MTIM3_voidTimerStop(void);
u16 		MTIM3_u16GetCapturedValue(u8 ARG_u8MTIMChannel);
void MTIM3_voidResetCounter(void);


#endif
