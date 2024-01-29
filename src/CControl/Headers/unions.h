/*
 * unions.h
 *
 *  Created on: 29 Januari 2024
 *      Author: Daniel Mårtensson
 */

#ifndef UNIONS_H_
#define UNIONS_H_

/* Covert float to uint8_t array */
typedef union {
    float value;
    uint8_t array[4];
}FLOAT_UINT8;

#endif /* !UNIONS_H_*/