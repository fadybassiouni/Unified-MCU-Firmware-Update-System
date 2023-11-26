/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : SysTick            */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/

#include <LBIT_math.h>
#include <LSTD_types.h>

#include <MSTK_private.h>
#include <MSTK_interface.h>

static void(*MSTK_pvoidfuncUserFunction)(void) = NULL_POINTER;
static volatile u8 MSTK_u8CountMode = MSTK_SINGLE_INTERVAL;

STD_error_t MSTK_stderrInit(u8 ARG_u8ClockSource)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	switch(ARG_u8ClockSource)
	{
	case MSTK_CLOCKSRC_AHB_8: CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_CLKSOURCE); break;
	case MSTK_CLOCKSRC_AHB:   SET_BIT(MSTK->CTRL, MSTK_CTRL_CLKSOURCE); break;
	default : L_stderrState = E_NOK; break;
	}

	/*Return Error State*/
	return L_stderrState;
}

void MSTK_voidBusyWait(u32 ARG_u32Ticks)
{
	/*Load ticks into load register*/
	MSTK->LOAD = ARG_u32Ticks;

	/*Start SysTick timer*/
	SET_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);

	/*Wait till flag is raised*/
	while(!GET_BIT(MSTK->CTRL,MSTK_CTRL_COUNTFLAG));

	/*Stop SysTick Timer*/
	CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;
}


STD_error_t MSTK_stderrStartSingle(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void))
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_pvoidfuncUserFunction != NULL_POINTER)
	{
		/*Clear Last Value*/
		MSTK -> VAL  = 0;

		/*Load ticks into load register*/
		MSTK->LOAD = ARG_u32Ticks;

		/*Start SysTick timer*/
		SET_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);

		/*Set Callback Function*/
		MSTK_pvoidfuncUserFunction = ARG_pvoidfuncUserFunction;

		/*Set Mode to Single*/
		MSTK_u8CountMode = MSTK_SINGLE_INTERVAL;

		/*Enable SysTick Interrupt */
		SET_BIT(MSTK->CTRL, MSTK_CTRL_TICKINT);
	}
	else
	{
		/*State is Error due to Null Pointer*/
		L_stderrState = E_NULL_POINTER;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MSTK_stderrStartPeriodic(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void))
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_pvoidfuncUserFunction != NULL_POINTER)
	{
		/*Clear Last Value*/
		MSTK -> VAL  = 0;

		/*Load ticks into load register*/
		MSTK->LOAD = ARG_u32Ticks;

		/*Start SysTick timer*/
		SET_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);

		/*Set Callback Function*/
		MSTK_pvoidfuncUserFunction = ARG_pvoidfuncUserFunction;

		/*Set Mode to Periodic*/
		MSTK_u8CountMode = MSTK_PERIOD_INTERVAL;

		/*Enable SysTick Interrupt */
		SET_BIT(MSTK->CTRL, MSTK_CTRL_TICKINT);
	}
	else
	{
		/*State is Error due to Null Pointer*/
		L_stderrState = E_NULL_POINTER;
	}

	/*Return Error State*/
	return L_stderrState;
}

void MSTK_voidStop()
{
	/*Disable SysTick Interrupt*/
	CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_TICKINT);

	/*Stop SysTick Timer*/
	CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;
}

u32 MSTK_u32GetElapsedTime(void)
{
	/*Local Variable to Store Elapsed Time*/
	u32 L_u32ElapsedTime;

	/*Calculate Elapsed Time*/
	L_u32ElapsedTime = (MSTK->LOAD) - (MSTK -> VAL);

	/*Return Elapsed Time*/
	return L_u32ElapsedTime;
}

u32 MSTK_u32GetRemainingTime(void)
{
	/*Local Variable to Store Remaining Time*/
	u32 L_u32RemainingTime;

	/*Calculate Remaining Time*/
	L_u32RemainingTime = MSTK->VAL;

	/*Return Remaining Time*/
	return L_u32RemainingTime;
}


void SysTick_Handler(void)
{
	u8 L_u8Temporary;

	if (MSTK_u8CountMode == MSTK_SINGLE_INTERVAL)
	{
		/*Disable SysTick Interrupt*/
		CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_TICKINT);

		/*Stop SysTick Timer*/
		CLEAR_BIT(MSTK->CTRL, MSTK_CTRL_ENABLE);
		MSTK -> LOAD = 0;
		MSTK -> VAL  = 0;
	}
	else
	{
		/*MISRA*/
	}

	/* Callback notification */
	MSTK_pvoidfuncUserFunction();

	/*Clear interrupt flag*/
	L_u8Temporary = GET_BIT(MSTK->CTRL,MSTK_CTRL_COUNTFLAG);
}
