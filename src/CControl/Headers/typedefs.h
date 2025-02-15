/*
 * typedefs.h
 *
 *  Created on: 15 Februari. 2025
 *      Author: Daniel Mårtensson
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

/* Use this typedef if your function can return different type of status codes that can result harmful situations */
typedef enum {
	STATUS_NAN,
	STATUS_INF,
	STATUS_OK,
	STATUS_NOT_OPTIMAL_SOLUTION
}STATUS_CODES;

#endif /* !TYPEDEFS_H_ */