/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : EEPROM             */
/* Layer     : HAL  		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef _HNVM_PRIVATE_H_
#define _HNVM_PRIVATE_H_

#define HNVM_MAX_BURST_WRITE_BYTE_COUNT 64
#define HNVM_MAX_BURST_READ_BYTE_COUNT  256000

static void HNVM_voidSetAddressPins(u32 ARG_u32Address);

#endif
