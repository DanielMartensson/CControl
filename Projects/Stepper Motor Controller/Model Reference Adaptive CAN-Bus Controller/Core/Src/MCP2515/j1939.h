/*
 * J1939.h
 *
 *  Created on: 11 aug. 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_MCP2515_J1939_H_
#define SRC_MCP2515_J1939_H_

#include "CANSPI.h"
#include <math.h>
#include <string.h>

enum DM15_Status_Code{
	DM15_Proceed = 0,
	DM15_Busy = 1,
	DM15_Operation_Completed = 4,
	DM15_Operation_Failed = 5,
	DM15_Reserved = 7,
	DM15_Error_Codes = 8
};

enum DM14_Command_Code{
	DM14_Read = 1,
	DM14_Write = 2,
	DM14_Operation_Completed = 4,
	DM14_Operation_Failed = 5
};

// Don't FORGET to initialize these arrays and variables inconnect_can_bus() function
typedef struct{
	// Addresses and CA Names
	uint8_t SA_ACU;
	uint8_t SA_ECU;
	uint8_t CA[8];

	// Messages that being send ACU -> ECU
	bool DM1_Messages_Binary[12];
	uint8_t DM15_Status;
	uint8_t DM16_data[8];

	/*
	 * EEPROM:
	 * [0] = Sensor Stall Neutral Enable (Address 646)
	 * [1] = Sensor Stall Normal Enable (Address 647)
	 */
	uint16_t EEPROM_values[2]; // Here we store the system parameters
}J1939;

void write_1939_configuration_request(J1939* j1939, uint8_t command);
void write_1939_request_for_claim_address(J1939* j1939);
void write_1939_CA_names_and_source_address(J1939* j1939);
void write_1939_auxiliary_valve_command(uint8_t DA, J1939* j1939, float flow);
void read_1939_message(J1939* j1939);


#endif /* SRC_MCP2515_J1939_H_ */
