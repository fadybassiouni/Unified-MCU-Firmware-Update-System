/**********************************/
/* Author    : Amir Galal         */
/* SWC       : TIMER2             */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 20,2023    */
/* Last Edit : N/A                */
/**********************************/


#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MTIM2_private.h>
#include <MTIM2_interface.h>


static void (*MTIM2_pvoidfUserFunction)(void) = NULL_POINTER;

static volatile u8 MTIM2_u16UnderFLowsCount = 0;

void MTIM2_voidInit(void)
{
	// Configure Timer 2
	MTIM2->CR1 = 0x00000010; // Down counting Mode
	MTIM2->PSC = 7;  //Prescaler value (8)
}



void MTIM2_voidEnableTimer(void)
{
	// Enable Timer 1
	MTIM2->CR1 |= (1 << 0);
}


void MTIM2_voidDisableTimer(void)
{
	CLEAR_BIT(MTIM2->CR1,0);
}



STD_error_t MTIM2_stderrStartPeriodic(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void))
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_pvoidfuncUserFunction != NULL_POINTER)
	{
		/*Load ticks into ARR register*/
		MTIM2->ARR = MTIM2_AUTORELOAD_VALUE;

		/*Calculate Required Underflows*/
		MTIM2_u16UnderFLowsCount = ARG_u32Ticks / MTIM2_UNDERFLOW_TIME;

		/*Set Callback Function*/
		MTIM2_pvoidfUserFunction = ARG_pvoidfuncUserFunction;
		
		/*Enable TIMER 2*/
		MTIM2_voidEnableTimer();
	}
	else
	{
		/*State is Error due to Null Pointer*/
		L_stderrState = E_NULL_POINTER;
	}

	/*Return Error State*/
	return L_stderrState;
}

void MTIM2_voidEnableInterrupt(void)
{
	SET_BIT(MTIM2->DIER, 0);
}

void MTIM2_voidDisableInterrupt(void)
{
	CLEAR_BIT(MTIM2->DIER, 0);
}


void TIM2_IRQHandler(void)
{
	static u8 L_u8UnderFlows = 0;
	if(MTIM2_pvoidfUserFunction != NULL_POINTER)
	{
		L_u8UnderFlows++;
		if(L_u8UnderFlows == MTIM2_u16UnderFLowsCount)
		{
			L_u8UnderFlows=0;
			MTIM2_pvoidfUserFunction();
		}

	}
	else
	{
		/*MISRA*/
	}
	CLEAR_BIT(MTIM2->SR, 0);
}


