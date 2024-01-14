/*
 * macros.h
 *
 *  Created on: 8 November 2023
 *      Author: Daniel Mårtensson
 */

#ifndef MACROS_H_
#define MACROS_H_

#define ARRAY_SIZE(X) sizeof(X) / sizeof(sizeof(X[0]))	/* Length mesaurement for non-pointer array */
#ifndef IS_C89
#define INLINE inline
#else
#define INLINE
#endif /* !IS_C89 */



#endif /* !MACROS_H_*/