#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned char byte;
typedef unsigned short int word;

/*
 * Zero Flag
 * This bit is set when the result of a math operation
 * is zero or two values match when using the CP
 * instruction.
 */
#define Z_FLAG 0x80
/*
 * Subtract Flag
 * This bit is set if a subtraction was performed in the
 * last math instruction.
 */
#define N_FLAG 0x40
/*
 * Half Carry Flag
 * This bit is set if a carry occurred from the lower
 * nibble in the last math operation.
 */
#define H_FLAG 0x20
/*
 * Carry Flag
 * This bit is set if a carry occurred from the las
 * math operation or if register A is the smaller value
 * when executing the CP instruction.
 */
#define C_FLAG 0x10


#endif // TYPES_H_
