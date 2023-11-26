/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : NVIC               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/
#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MNVIC_private.h>
#include <MNVIC_interface.h>

/*----------------------------------------------------------------------------------------------------------
 * NVIC Driver works by taking a Vector ID as an input and analyzing the input number to get
 * the register that the IRQ Enable bit is located in and the bit number out of the 32 bits in that register
 * The Explanation is as follows:
 *
 * The user enters Vector ID SPI3 which translates to predefined value 51 decimal 
 * and (00000000)(00000000)(00000000)(<010><10010>) binary, and from that binary number
 * we can see that the 5th, 6th, and 7th bit contain the register number out of the Eight Registers 
 * which is 2 in decimal and <010>10010 in binary (Register ISER[2]).
 * Now we have the register number by which we can index the desired
 * register ISERx. to get the bit number we can see that the first five bits
 * represent the pin number (010<10010>) which is 18 in decimal. from that we can conclude that a general rule
 *
 * Vector ID (AB) -> (<AAA><BBBBB>) where A represents register Number and B represents bit Number
 *
 * Now we can get the port number by Shifting the register number to the right 5 times (AB) >> 5
 * and we can get the bit number by masking the Vector ID by the number 0x0000001F (AB) & 0x0000001F;
 ---------------------------------------------------------------------------------------------------------*/

STD_error_t MNVIC_stderrEnableIRQ(VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		MNVIC->ISER[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR] = (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrDisableIRQ(VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		MNVIC->ICER[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR] = (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}


STD_error_t MNVIC_stderrSetPendingIRQ(VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		MNVIC->ISPR[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR] = (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrClearPendingIRQ(VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		MNVIC->ICPR[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR] = (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrGetPendingIRQ(u8 *ARG_pu8PendingState, VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		*ARG_pu8PendingState = GET_BIT(MNVIC->ICPR[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR], (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK)));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrGetActiveIRQ(u8 *ARG_pu8ActiveState, VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		*ARG_pu8ActiveState = GET_BIT(MNVIC->IABR[ARG_vector_tVectorID>>MNVIC_REGISTER_SHIFT_FACTOR], (1<<(ARG_vector_tVectorID & MNVIC_BIT_NO_MASK)));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrSetPriority(VECTOR_t ARG_vector_tVectorID, u8 ARG_u8Priority)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		MNVIC->IPR[ARG_vector_tVectorID] = ARG_u8Priority<<MNVIC_PRIORITY_SHIFT_FACTOR;
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrGetPriority(u8 *ARG_pu8Priority, VECTOR_t ARG_vector_tVectorID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_vector_tVectorID <= DMA2_CHANNEL4_5)
	{
		*ARG_pu8Priority = MNVIC->IPR[ARG_vector_tVectorID]>>MNVIC_PRIORITY_SHIFT_FACTOR;
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MNVIC_stderrSetGroupingMode(u8 ARG_u8GroupingMode)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	if(ARG_u8GroupingMode >= MNVIC_GROUPING_16G_0S && ARG_u8GroupingMode <= MNVIC_GROUPING_0G_16S)
	{
		AIRCR = (AIRCR & MNVIC_AIRCR_GROUP_MASK) | (ARG_u8GroupingMode<<MNVIC_AIRCR_PRIGROUP0) | (MNVIC_VECTKEY << MNVIC_AIRCR_VECTKEY0);
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}
