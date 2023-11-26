/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : STD_TYPES          */
/* Layer     : LIB   	          */
/* Version   : 1.1                */
/* Date      : August 3,2023      */
/* Last Edit : August 2,2023      */
/**********************************/


#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* Boolean Data Type */
typedef unsigned char bool;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif

#ifndef TRUE
#define TRUE        (1u)
#endif



typedef unsigned char         u8;          /*           0 .. 255             */
typedef signed char           s8;          /*        -128 .. +127            */
typedef unsigned short        u16;         /*           0 .. 65535           */
typedef signed short          s16;         /*      -32768 .. +32767          */
typedef unsigned long         u32;         /*           0 .. 4294967295      */
typedef signed long           s32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    u64;         /*       0..18446744073709551615  */
typedef signed long long      s64;
typedef float                 f32;
typedef double                f64;


typedef enum 
{
	E_OK,
	E_NOK,
	E_NULL_POINTER
}STD_error_t;

#define NULL_POINTER ((void *)0)

/*Define if to use UART Debugging or not*/
//#define UART_DEBUG

#endif /* STD_TYPE_H_ */
