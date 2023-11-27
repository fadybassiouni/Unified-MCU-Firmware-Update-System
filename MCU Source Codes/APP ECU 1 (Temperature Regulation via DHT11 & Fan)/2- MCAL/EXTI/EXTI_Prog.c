#include "LBIT_MATH.h"
#include "LSTD_types.h"
#include <stdint.h>

#include "EXTI_Interface.h"
#include "EXTI_Private.h"

static void (*EXTI_CallBack[EXTI_NUM])(void);

u8 EXTI_u8ConfigLine (u8 Copy_u8EXTI_Line , u8 Copy_u8TriggeringMode)
{
  u8 Local_u8ErrorState= E_OK;
  
	if (Copy_u8EXTI_Line > 18) 
	{
		Local_u8ErrorState=E_NOK;
	}
	else{
		switch (Copy_u8TriggeringMode)
		{
			case RISING_EDGE_TRIGGERING :
			  SETBIT(EXTI->RTSR , Copy_u8EXTI_Line);
			  CLRBIT(EXTI->FTSR , Copy_u8EXTI_Line); break;
			case FALLING_EDGE_TRIGGERING:
			  SETBIT(EXTI->FTSR , Copy_u8EXTI_Line);
			  CLRBIT(EXTI->RTSR , Copy_u8EXTI_Line); break;
			case ON_CHANGE_TRIGGERING   :
			  SETBIT(EXTI->RTSR , Copy_u8EXTI_Line);
			  SETBIT(EXTI->FTSR , Copy_u8EXTI_Line); break;
			default                     :
			  Local_u8ErrorState=E_NOK;
		}
    }  
  return Local_u8ErrorState;
}
u8 EXTI_u8SetTrigger(u8 Copy_EXTI_Num ,u8 Copy_TriggerState ){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState= E_NOK;
	}
	else{
		switch(Copy_TriggerState)
			{
				case RISING_EDGE_TRIGGERING: 	EXTI->RTSR |= 1<< Copy_EXTI_Num;
											EXTI->FTSR &= ~(1<<Copy_EXTI_Num); break;
				case FALLING_EDGE_TRIGGERING: 	EXTI->FTSR |= 1<< Copy_EXTI_Num;
											EXTI->RTSR &= ~(1<< Copy_EXTI_Num); break;
				case ON_CHANGE_TRIGGERING: EXTI->RTSR |= 1<< Copy_EXTI_Num;
											EXTI->FTSR |= 1<< Copy_EXTI_Num; 	  break;
				default: Local_ErrorState = E_NOK;
			}
	}

	return Local_ErrorState;
}

u8 EXTI_u8ClearPendingFlag(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/*Clear Pending Flag*/
		EXTI->PR = 1<<Copy_EXTI_Num; /* 0 doesn't affect */
	}

	return Local_ErrorState;
}


u8 EXTI_u8GetPendingFlag(u8 Copy_EXTI_Num , u8 *Copy_PenValue){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/* Set bit */
		*Copy_PenValue= (EXTI->PR >> Copy_EXTI_Num) & 1 ;
	}

	return Local_ErrorState;
}


u8 EXTI_u8IntEnable(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/* Set bit enable  */
		EXTI->IMR |= 1<<Copy_EXTI_Num;
	}

	return Local_ErrorState;
}


u8 EXTI_u8IntDisable(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState= E_NOK;
	}
	else{
		/* Set bit enable  */
		EXTI->IMR &= ~(1<<Copy_EXTI_Num);
	}
	return Local_ErrorState;
}

u8 EXTI_voidSetCallBackFn (u8 Copy_u8EXTI_Line , void (*pvoidfCallback)(void))
{
	u8 Local_u8ErrorState= E_OK;
	if(NULL_POINTER == pvoidfCallback)
	{
		Local_u8ErrorState=E_NULL_POINTER;
	}
	else
	{
		if (Copy_u8EXTI_Line > 18)
		{
			Local_u8ErrorState=E_NOK;
		}
		else
		{
			EXTI_CallBack[Copy_u8EXTI_Line] = pvoidfCallback;
		}
	}
	return Local_u8ErrorState;
}

/******************************************************************************/
/********************* Handlers definition for EXTI IRQs *********************/
/******************************************************************************/
#ifdef EXTI0_LINE
void EXTI0_IRQHandler(void){
	(EXTI_CallBack[EXTI0_LINE])();
//	SETBIT( EXTI->PR , EXTI0_LINE );
}
#endif

#ifdef EXTI4_LINE
void EXTI4_IRQHandler(void){
		SETBIT( EXTI->PR , EXTI4_LINE );
	(EXTI_CallBack[EXTI4_LINE])();

}
#endif



#ifdef EXTI9_5_LINES_IRQ
void EXTI9_5_IRQHandler(void){
//	(EXTI_CallBack[EXTI9_5_LINES_IRQ])();
	(EXTI_CallBack[EXTI7_LINE])();
	SETBIT( EXTI->PR , EXTI9_5_LINES_IRQ );
}
#endif

