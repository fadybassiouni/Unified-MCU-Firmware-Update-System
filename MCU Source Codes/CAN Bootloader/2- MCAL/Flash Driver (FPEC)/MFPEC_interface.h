

#ifndef _MFPEC_INTERFACE_H_
#define _MFPEC_INTERFACE_H_

void MFPEC_voidEraseAppArea(void);
	
void MFPEC_voidFlashPageErase(u8 ARG_u8PageNumber);

void MFPEC_voidFlashWrite(u32 ARG_u32Address, u16* ARG_pu16Data, u16 ARG_u16Length);

u8 AsciToHex(u8 Copy_u8Asci);

#endif
