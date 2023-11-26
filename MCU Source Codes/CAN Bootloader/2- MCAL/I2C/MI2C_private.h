/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : I2C                */
/* Layer     : MCAL   		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef _I2C_PRIVATE_H_
#define _I2C_PRIVATE_H_

#define I2C1_BASE_ADDRESS ((volatile u32 *)0x40005400)

typedef struct 
{
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 OAR1;
	volatile u32 OAR2;
	volatile u32 DR;
	volatile u32 SR1;
	volatile u32 SR2;
	volatile u32 CCR;
	volatile u32 TRISE;
}MI2C_t;

#define MI2C1 ((volatile MI2C_t *)I2C1_BASE_ADDRESS)

/*Control Register 1 Bits*/
#define MI2C_CR1_PE                 0
#define MI2C_CR1_SMBUS              1
#define MI2C_CR1_SMBTYPE            3
#define MI2C_CR1_ENARP              4
#define MI2C_CR1_ENPEC              5
#define MI2C_CR1_ENGC               6
#define MI2C_CR1_NOSTRETCH          7
#define MI2C_CR1_START              8
#define MI2C_CR1_STOP               9
#define MI2C_CR1_ACK                10
#define MI2C_CR1_POS                11
#define MI2C_CR1_PEC                12
#define MI2C_CR1_ALERT              13
#define MI2C_CR1_SWRST              15


/*Control Register 2 Bits*/
#define MI2C_CR2_FREQ0              0
#define MI2C_CR2_FREQ5              5
#define MI2C_CR2_ITERREN            8
#define MI2C_CR2_ITEVTEN            9
#define MI2C_CR2_ITBUFEN            10
#define MI2C_CR2_DMAEN              11
#define MI2C_CR2_LAST               12

/*Own Address Register 1 Bits*/
#define MI2C_OAR1_ADD0              0
#define MI2C_OAR1_ADD1              1
#define MI2C_OAR1_ADD7              7
#define MI2C_OAR1_ADD8              8
#define MI2C_OAR1_ADD9              9
#define MI2C_OAR1_ADDMODE           15

/*Own Address Register 2 Bits*/
#define MI2C_OAR2_ENDUAL            0
#define MI2C_OAR2_ADD20             1
#define MI2C_OAR2_ADD27             7

/*Status Register 1 Bits*/
#define MI2C_SR1_SB                 0
#define MI2C_SR1_ADDR               1
#define MI2C_SR1_BTF                2
#define MI2C_SR1_ADD10              3
#define MI2C_SR1_STOPF              4
#define MI2C_SR1_RxNE               6
#define MI2C_SR1_TxE                7
#define MI2C_SR1_BERR               8
#define MI2C_SR1_ARLO               9
#define MI2C_SR1_AF                 10
#define MI2C_SR1_OVR                11
#define MI2C_SR1_PECERR             12
#define MI2C_SR1_TIMEOUT            14
#define MI2C_SR1_SMBALERT           15

/*Status Register 2 Bits*/
#define MI2C_SR2_MSL                0
#define MI2C_SR2_BUSY               1
#define MI2C_SR2_TRA                2
#define MI2C_SR2_GENCALL            4
#define MI2C_SR2_SMBDEFAULT         5
#define MI2C_SR2_SMBHOST            6
#define MI2C_SR2_DUALF              7
#define MI2C_SR2_PEC0               8
#define MI2C_SR2_PEC7               15

/*Clock Control Register Bits*/
#define MI2C_CCR_CCR0               0
#define MI2C_CCR_CCR11              11
#define MI2C_CCR_DUTY               14
#define MI2C_CCR_FS                 15

/*Rise Time Register Bits*/
#define MI2C_TRISE_TRISE0           0
#define MI2C_TRISE_TRISE5           5

#endif
