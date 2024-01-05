/*
 * detectmemoryleak.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "hardware.h"

void detectmemoryleak() {
	/* Memory leacks - Only for Visual Studio */
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /*!_MSC_VER */ 
}

