#ifndef NVIC_PRIVATE_H_
#define NVIC_PRIVATE_H_


#define NVIC_BASE_ADDRESS  			0xE000E100

/*************** NVIC Register Definition Structure **********/
typedef struct
{
    volatile uint32_t ISER[8];      /* Interrupt Set-enable Registers */
    uint32_t 		reserved1[24];
    volatile uint32_t ICER[8];      /* Interrupt Clear-enable Registers */
    uint32_t 		reserved2[24];
    volatile uint32_t ISPR[8];      /* Interrupt Set-pending Registers */
    uint32_t 		reserved3[24];
    volatile uint32_t ICPR[8];      /* Interrupt Clear-pending Registers */
    uint32_t 		reserved4[24];
    volatile uint32_t IABR[8];      /* Interrupt Active Bit Registers */
    uint32_t 		reserved5[56];
    volatile uint8_t IPR[240];      /* Interrupt Priority Registers  */
    uint32_t 		reserved6[228];
    volatile uint32_t STIR;              /* Software Trigger Interrupt Register */
}NVIC_RegDef_t;


/******************* NVIC Peripheral Definition **************/
#define NVIC 			             ((NVIC_RegDef_t *) NVIC_BASE_ADDRESS)


#define IRQ_REGSIZE				32U
#define IRQ_PIRSIZE				4U
#define IRQ_SETBIT				1U
#define IRQ_BITMASK				1U
#define IRQ_SHIFTBITS			4U

#define AIRCR_MASK              0xFFFF0700
#define AIRCR_VECTKEY			0x5FA
#define VECTKEY_SHIFTBITS		16U
#define AIRCR_RESET				0xFA050000


#define PRIGROUP_SHIFTBITS		8U


#endif
