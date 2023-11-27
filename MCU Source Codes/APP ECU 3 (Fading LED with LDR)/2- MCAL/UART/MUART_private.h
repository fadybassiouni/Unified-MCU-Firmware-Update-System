/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : UART               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 23,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MUART_PRIVATE_H_
#define _MUART_PRIVATE_H_

#define MUART1_BASE_ADDRESS ((volatile u32 *)0x40013800)
#define MUART2_BASE_ADDRESS ((volatile u32 *)0x40004400)
#define MUART3_BASE_ADDRESS ((volatile u32 *)0x40004800)

typedef struct 
{
	volatile u32 SR;
	volatile u32 DR;
	volatile u32 BRR;
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 CR3;
	volatile u32 GTPR;
}UART_t;

#define MUART1 ((volatile UART_t *)MUART1_BASE_ADDRESS)
#define MUART2 ((volatile UART_t *)MUART2_BASE_ADDRESS)
#define MUART3 ((volatile UART_t *)MUART3_BASE_ADDRESS)

/*UART Status Register Bits*/
#define MUART_SR_PE        0
#define MUART_SR_FE        1
#define MUART_SR_NE        2
#define MUART_SR_ORE       3
#define MUART_SR_IDLE      4
#define MUART_SR_RXNE      5
#define MUART_SR_TC        6
#define MUART_SR_TXE       7
#define MUART_SR_LBD       8
#define MUART_SR_CTS       9

/*UART Baud Rate Register Bits*/
#define MUART_BRR_DIV_FRACTION0     0
#define MUART_BRR_DIV_MANTISSA0     4

/*UART Control Register 1*/
#define MUART_CR1_SBK               0
#define MUART_CR1_RWU               1
#define MUART_CR1_RE                2
#define MUART_CR1_TE                3
#define MUART_CR1_IDLEIE            4
#define MUART_CR1_RXNEIE            5
#define MUART_CR1_TCIE              6
#define MUART_CR1_TXEIE             7
#define MUART_CR1_PEIE              8
#define MUART_CR1_PS                9
#define MUART_CR1_PCE               10
#define MUART_CR1_WAKE              11
#define MUART_CR1_M                 12
#define MUART_CR1_UE                13

/*UART Control Register 2*/
#define MUART_CR2_ADD0              0
#define MUART_CR2_LBDL              5
#define MUART_CR2_LBDIE             6
#define MUART_CR2_LBCL              8
#define MUART_CR2_CPHA              9
#define MUART_CR2_CPOL              10
#define MUART_CR2_CLKEN             11
#define MUART_CR2_STOP0             12
#define MUART_CR2_LINEN             14

/*UART Control Register 3*/
#define MUART_CR3_EIE               0    
#define MUART_CR3_IREN              1
#define MUART_CR3_IRLP              2
#define MUART_CR3_HDSEL             3
#define MUART_CR3_NACK              4
#define MUART_CR3_SCEN              5
#define MUART_CR3_DMAR              6
#define MUART_CR3_DMAT              7
#define MUART_CR3_RTSE              8
#define MUART_CR3_CTSE              9
#define MUART_CR3_CTSIE             10

#endif
