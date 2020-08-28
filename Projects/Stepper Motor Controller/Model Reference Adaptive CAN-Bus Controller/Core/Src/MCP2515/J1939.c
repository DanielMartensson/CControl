/*
 * J1939.c
 *
 *  Created on: 11 aug. 2020
 *      Author: Daniel Mårtensson
 */

#include "j1939.h"


static bool CAN_connected = false;
static uCAN_MSG txMessage;
static uCAN_MSG rxMessage;

// Write an extended CAN message
static void write_CAN_message(uint32_t ID, uint8_t data[]) {
	txMessage.frame.idType = dEXTENDED_CAN_MSG_ID_2_0B;
	txMessage.frame.id = ID;
	txMessage.frame.dlc = 8;
	txMessage.frame.data0 = data[0];
	txMessage.frame.data1 = data[1];
	txMessage.frame.data2 = data[2];
	txMessage.frame.data3 = data[3];
	txMessage.frame.data4 = data[4];
	txMessage.frame.data5 = data[5];
	txMessage.frame.data6 = data[6];
	txMessage.frame.data7 = data[7];
	CANSPI_Transmit(&txMessage);
}

// Read the extended  CAN message
static bool read_CAN_message(uint32_t* ID, uint8_t data[]){
	bool available = CANSPI_Receive(&rxMessage);
	if(available){
		//uint8_t idType = rxMessage.frame.idType; // Extended ID type
		*ID = rxMessage.frame.id;
		data[0] = rxMessage.frame.data0;
		data[1] = rxMessage.frame.data1;
		data[2] = rxMessage.frame.data2;
		data[3] = rxMessage.frame.data3;
		data[4] = rxMessage.frame.data4;
		data[5] = rxMessage.frame.data5;
		data[6] = rxMessage.frame.data6;
		data[7] = rxMessage.frame.data7;
		return available;
	}
	return available;
}

// Transport Protocol Connexion Management Broadcast Announce Message
static void send_j1939_TP_CM_BAM(J1939* j1939, uint16_t message_size, uint8_t packages_to_send, uint32_t PGN){
	// ID
	uint32_t ID = (0x1CECFF << 8) | j1939->SA_ECU;

	// Fill the data
	uint8_t data[8];
	data[0] = 0x20; // Control byte - Standard
	data[1] = message_size; // Message size LSB. Total message size is 9 to 1785 bytes
	data[2] = message_size >> 8; // Message size MSB
	data[3] = packages_to_send; // Packages to send is 2 to 255 packages
	data[4] = 0xFF; // Reserved
	data[5] = PGN; // PGN LSB
	data[6] = PGN >> 8; // PGN
	data[7] = PGN >> 16; // PGN MSB

	// Write and wait 1000 ms then 100 ms
	HAL_Delay(1000);
	write_CAN_message(ID, data);
	HAL_Delay(100);
}

// Transport Protocol Data Transfer
static void send_j1939_TP_DT(J1939* j1939, uint8_t data[], uint16_t message_size, uint8_t packages_to_send){
	uint32_t ID = (0x1CEBFF << 8) | j1939->SA_ECU;
	uint8_t package[8];
	uint16_t sended_bytes = 0;
	for(uint8_t i = 1; i <= packages_to_send; i++){
		// Number of package
		package[0] = i;

		// Data
		for(uint8_t j = 0; j < 7; j++){
			if(sended_bytes < message_size)
				package[j+1] = data[sended_bytes++];
			else
				package[j+1] = 0xFF; // No data
		}

		// Send wait 100 ms
		write_CAN_message(ID, package);
		HAL_Delay(100);
	}
}

// Connect can-bus, if we are not connected
static void connect_can_bus(J1939* j1939){
	if(CAN_connected == false){
		memset(j1939->CA, 0, 8);
		memset(j1939->DM1_Messages_Binary, 0, 12);
		memset(j1939->DM16_data, 0, 8);
		memset(j1939->EEPROM_values, 0, 2*sizeof(uint16_t));
		j1939->SA_ACU = 0;
		j1939->DM15_Status = DM15_Proceed;
		CAN_connected = CANSPI_Initialize();
	}
}

// Special case for sending a request in J1939 of 3 bytes of PNG
static void write_CAN_PGN_request(uint32_t ID, uint8_t data[]){
	txMessage.frame.idType = dEXTENDED_CAN_MSG_ID_2_0B;
	txMessage.frame.id = ID;
	txMessage.frame.dlc = 3;
	txMessage.frame.data0 = data[0];
	txMessage.frame.data1 = data[1];
	txMessage.frame.data2 = data[2];
	CANSPI_Transmit(&txMessage);
}

// Send a memory request for access EEPROM
static void DM14_Memory_access_request(uint32_t ID, uint16_t length, uint8_t command, uint32_t pointer, uint16_t key){
	uint8_t data[8];
	data[0] = length; // Length LSB
	data[1] = (length >> 3) | (0 << 4) | (command << 1) | 1; // Length MSB, Pointer Type(Diagnostic message = 0), Command, 1 (Always)
	data[2] = pointer; // Pointer LSB
	data[3] = pointer >> 8;
	data[4] = pointer >> 16; // Pointer MSB
	data[5] = 0x1; // Pointer extension 1 for access EEPROM
	data[6] = key; // Key LSB
	data[7] = key >> 8; // Key MSB
	write_CAN_message(ID, data);
}


// Write a claim address to ACU
void write_1939_request_for_claim_address(J1939* j1939){
	connect_can_bus(j1939);
	uint8_t data[3] = {0x0, 0xEE, 0x00};
	uint32_t ID = 0x18EAFFFE; // FE = 254 = Peer to Peer, FF = Boradcast
	write_CAN_PGN_request(ID, data);
	HAL_Delay(100);
}

// Request configuration parameters
void write_1939_configuration_request(J1939* j1939, uint8_t command){
	uint32_t ID = (0x18D9 << 16) | (j1939->SA_ACU << 8) | j1939->SA_ECU;
	HAL_Delay(1000);

	/*
	 * Pointers:
	 * 646 = Sensor_Stall_Neutral_Enable
	 * 647 = Sensor_Stall_Normal_Enable
	 *
	 * Length:
	 * 1 = Sensor_Stall_Neutral_Enable
	 * 1 = Sensor_Stall_Normal_Enable
	 */
	const uint8_t length_eeprom_values[2] = {1, 1}; // Between 1 and 7 only!
	const uint32_t pointers_eeprom_address[2] = {646, 647}; // Same indexing for j1939->EEPROM_values

	// DM14(Ask) -> DM15(Status) -> DM16(Read/Write) -> DM14(Close)
	for(uint8_t i = 0; i < 2; i++){
		// Send a DM14 request
		if(command == DM14_Read)
			DM14_Memory_access_request(ID, length_eeprom_values[i], DM14_Read, pointers_eeprom_address[i], 0xFFFF); // 0xFFFF = No key
		else if (command == DM14_Write)
			DM14_Memory_access_request(ID, length_eeprom_values[i], DM14_Write, pointers_eeprom_address[i], 0x2505); // 0x2505 = Write key

		// Read until we got a DM15 message response - We expecting DM15_Proceed
		j1939->DM15_Status = DM15_Reserved;
		while(j1939->DM15_Status == DM15_Reserved){
			read_1939_message(j1939); // Will update j1939->DM15_Status
			if(j1939->DM1_Messages_Binary[1]){
				// Inconsistent CAN Control message
				return;
			}else if(j1939->DM1_Messages_Binary[2]){
				// No CAN Control message received = TIME OUT
				return;
			}
		}

		// What does it say
		if(j1939->DM15_Status == DM15_Proceed){
			if(command == DM14_Read){
				// Read the EEPROM values of ECU and give them to the EEPROM_values array
				memset(j1939->DM16_data, 0, 8); // j1939->DM16_data[0] will become between 1 and 7 only!
				while(j1939->DM16_data[0] == 0){
					read_1939_message(j1939); // As long DM16_data[0], then we have not receive our DM16 data yet
					if(j1939->DM1_Messages_Binary[3]){
						// EEPROM parameter write/read error
						return;
					}
				}
				j1939->EEPROM_values[i] = 0;
				for(uint8_t j = 1; j <= j1939->DM16_data[0]; j++){
					j1939->EEPROM_values[i] |= j1939->DM16_data[j] << 8*(j-1); // EEPROM_values are uint16_t
				}

				// Read until we got a DM15 message response - We expecting DM15_Operation_Completed
				j1939->DM15_Status = DM15_Reserved;
				while(j1939->DM15_Status == DM15_Reserved){
					read_1939_message(j1939); // Will update j1939->DM15_Status
				}
			}else if (command == DM14_Write){
				// Write the EEPROM_values to data and send it to the ECU EEPROM
				memset(j1939->DM16_data, 0, 8);
				j1939->DM16_data[0] = length_eeprom_values[i]; // Between 1 and 7 only!
				for(uint8_t j = 1; j <= j1939->DM16_data[0]; j++){
					j1939->DM16_data[j] = j1939->EEPROM_values[i] >> 8*(j-1); // EEPROM_values are uint16_t
				}
				write_CAN_message(ID, j1939->DM16_data);
				// Close the read operation of DM14
				DM14_Memory_access_request(ID, length_eeprom_values[i], DM14_Operation_Completed, pointers_eeprom_address[i], 0xFFFF);
				j1939->DM15_Status = DM15_Operation_Completed;
			}
		}


		// Break the for-loop and return
		if(j1939->DM15_Status == DM15_Busy){
			break; // One single Busy status, then we cannot do anything yet
		}
	}
}

// Write the new CA and SA
void write_1939_CA_names_and_source_address(J1939* j1939){
	// Begin first to send BAM
	send_j1939_TP_CM_BAM(j1939, 0x9, 0x2, 0x00FED8); // 0x00FED8 = Commanded Address PGN

	// Send packages
	uint8_t data[9];
	memcpy(data, j1939->CA, 8); // Fill all CA to data
	data[8] = j1939->SA_ACU; // New address
	send_j1939_TP_DT(j1939, data, 0x9, 0x2);
}

// Write flow between -250 and 250. CA NAME need to have 0x81 (129d) Function code
void write_1939_auxiliary_valve_command(uint8_t DA, J1939* j1939, float flow){
	uint8_t state;
	if (flow > 0 && flow <= 250) {
		state = 0x1; // Extend
	} else if (flow < 0 && flow >= -250) {
		state = 0x2; // Retract
	}else if (flow == 255){
		state = 0xE; // Safety mode
	} else {
		state = 0x0; // Neutral
	}


	uint32_t ID = (0x0CFE << 16) | (DA << 8) | j1939->SA_ECU;
	uint8_t data[8] = {fabsf(flow), 0xFF, state, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	write_CAN_message(ID, data);
}

// Read the 1939 message
void read_1939_message(J1939* j1939) {
	// Check if we have connected the CAN
	connect_can_bus(j1939);

	uint32_t ID;
	uint8_t data[8];
	if (read_CAN_message(&ID, data) && CAN_connected) {
		uint8_t id0 = ID >> 24;
		uint8_t id1 = ID >> 16;
		uint8_t id2 = ID >> 8;
		uint8_t id3 = ID;
		// Check what message we got
		if (id0 == 0x18 && id1 == 0xEE && id2 == 0xFF && id3 != 0xFE) {
			// Address claimed
			j1939->SA_ACU = id3;
			memcpy(j1939->CA, data, 8);
		} else if (id0 == 0x18 && id1 == 0xEE && id2 == 0xFF && id3 == 0xFE) {
			// Address not claimed
			j1939->SA_ACU = 0;
			j1939->DM1_Messages_Binary[0] = true;
			memcpy(j1939->CA, data, 8);
		} else if (id0 == 0x18 && id1 == 0xFE && id2 == 0xCA && id3 == j1939->SA_ACU) {
			// DM1 message from ACU till ECU
			if (data[0] == 0x00 && data[1] == 0xFF && data[2] == 0x00 && data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x00 && data[6] == 0xFF && data[7] == 0xFF) {
				// No error
			} else {
				// Error - Check only SPN and J1939 FMI:s - The rest are reserved
				uint8_t SPN_byte_3 = data[2];
				uint8_t SPN_byte_4 = data[3];
				uint8_t SPN_byte_5 = data[4] >> 5;
				uint8_t FMI_byte = data[4] & ~(0b11100000);
				if(SPN_byte_3 == 0x3F && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x13){
					// Inconsistent CAN Control message
					j1939->DM1_Messages_Binary[1] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x3F && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x9){
					// No CAN Control message received = TIME OUT
					j1939->DM1_Messages_Binary[2] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x74 && SPN_byte_4 == 0x2 && SPN_byte_5 == 0x0 && FMI_byte == 0x2){
					// EEPROM parameter write/read error
					j1939->DM1_Messages_Binary[3] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x40 && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x0){
					// Stepper motor stall detected
					j1939->DM1_Messages_Binary[4] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x9E && SPN_byte_4 == 0x0 && SPN_byte_5 == 0x0 && FMI_byte == 0x12){
					// Undervoltage SONCEBOZ value (VDC<U_MINOFF)
					j1939->DM1_Messages_Binary[5] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x9E && SPN_byte_4 == 0x0 && SPN_byte_5 == 0x0 && FMI_byte == 0x10){
					// Overvoltage SONCEBOZ value (VDC>U_MAXOFF)
					j1939->DM1_Messages_Binary[6] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x42 && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x0){
					// SCU temperature higher than T_MAXOFF
					j1939->DM1_Messages_Binary[7] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x42 && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x1){
					// SCU temperature below T_MINOFF
					j1939->DM1_Messages_Binary[8] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x44 && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x0){
					// Current exceeds upper limit I_MAX
					j1939->DM1_Messages_Binary[9] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x43 && SPN_byte_4 == 0xF0 && SPN_byte_5 == 0x7 && FMI_byte == 0x2){
					// Error position in IDLE Mode
					j1939->DM1_Messages_Binary[10] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}else if(SPN_byte_3 == 0x76 && SPN_byte_4 == 0x2 && SPN_byte_5 == 0x0 && FMI_byte == 0x2){
					// Range error for Linearization parameter in EEPROM
					j1939->DM1_Messages_Binary[11] = true;
					j1939->DM15_Status = DM15_Error_Codes;
				}
			}
		} else if (id0 == 0x18 && id1 == 0xD8 && id2 == j1939->SA_ECU && id3 == j1939->SA_ACU) {
			// DM15 message - When we send DM14 message we will get DM15 message from ACU to ECU
			uint8_t status = (data[1] >> 1) & ~(0b11110000);
			if(status == DM15_Proceed){
				j1939->DM15_Status = DM15_Proceed;
			}else if(status == DM15_Busy){
				j1939->DM15_Status = DM15_Busy;
			}else if(status == DM15_Operation_Completed){
				j1939->DM15_Status = DM15_Operation_Completed;
			}else if(status == DM15_Operation_Failed){
				j1939->DM15_Status = DM15_Operation_Failed;
			}
		} else if (id0 == 0x18 && id1 == 0xD7 && id2 == j1939->SA_ECU && id3 == j1939->SA_ACU) {
			// DM16 message from ACU to ECU
			memset(j1939->DM16_data, 0, 8);
			memcpy(j1939->DM16_data, data, 8); // Byte1(Number of occurrences 1-7) to Byte2 -> Byte8 - Raw binary data
		}
	}
}
