/* ******************************************************** */
/* Author      	:	 Abd-alrahman Amin						*/
/* SWC         	:	 Flash Program and erase controller		*/
/* Layer       	:	 MCAL              						*/
/* MCu     		:	 stm32f103         						*/
/* Version     	:	 1.0               						*/
/* Date        	:	 September 23, 2023						*/
/* Last Edit   	:	 N/A 									*/
/* ******************************************************** */


#ifndef _MFPEC_PRIVATE_H_
#define _MFPEC_PRIVATE_H_


/******************* FPEC Peripheral Base Addresses ********************/ 

#define MFPEC_BASE_ADDRESS		0x40022000U
 

typedef enum 
{
    PG_BIT   	,    // Bit 0
    PER_BIT   	,    // Bit 1
    MER_BIT   	,    // Bit 2
    RES1_BIT  	,    // Bit 3
    OPTPG_BIT 	,   // Bit 4
    OPTER_BIT 	,   // Bit 5
    STRT_BIT 	,    // Bit 6
    LOCK_BIT 	,    // Bit 7
    RES2_BIT 	,    // Bit 8
    OPTWRE_BIT 	,  // Bit 9
    ERRIE_BIT 	,  // Bit 10
    RES3_BIT  	,   // Bit 11
    EOPIE_BIT 	   // Bit 12
} MFPEC_CRBITS_t;

typedef enum 
{
    BSY_FLAG = 0,
    PGERR_FLAG = 2,
    WRPRT_FLAG = 4,
    EOP_FLAG 
} MFPEC_SRBITS_t;


/************************** FPEC Register Definition Structure ***********************/
typedef struct
{
	volatile u32 ACR;
	volatile u32 KEYR;
	volatile u32 OPTKEYR;
	volatile u32 SR;
	volatile u32 CR;
	volatile u32 AR;
	volatile u32 RESERVED;
	volatile u32 OBR;
	volatile u32 WRPR;
}MFPEC_RegDef_t;


#define FPEC 			((MFPEC_RegDef_t*)MFPEC_BASE_ADDRESS)


#endif
