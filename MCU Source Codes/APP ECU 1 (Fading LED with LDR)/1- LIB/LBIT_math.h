/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : BIT_MATH           */
/* Layer     : LIB   	          */
/* Version   : 1.0                */
/* Date      : August 2,2023      */
/* Last Edit : N/A                */
/**********************************/

#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(reg, bitNo)    ((reg) |=  (1<<(bitNo)))
#define CLEAR_BIT(reg, bitNo)  ((reg) &= ~(1<<(bitNo)))
#define TOGGLE_BIT(reg, bitNo) ((reg) ^=  (1<<(bitNo)))
#define GET_BIT(reg, bitNo)    (((reg)>>(bitNo)) & (0x01))
#define ROR(reg, bitNo)        ((reg)=(((reg)>>(bitNo)) | ((reg)<<(8-(bitNo)))))
#define ROL(reg, bitNo)        ((reg)=(((reg)<<(bitNo)) | ((reg)>>(8-(bitNo)))))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))


#endif /* BIT_MATH_H_ */
