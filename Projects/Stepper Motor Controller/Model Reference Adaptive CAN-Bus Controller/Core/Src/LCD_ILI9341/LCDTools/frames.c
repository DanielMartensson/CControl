/*
 * frames.c
 *
 *  Created on: May 22, 2020
 *      Author: Daniel Mårtensson
 */

#include "LCDTools.h"

// Create the main frame for mrac
void show_main_frame_mrac(ILI9341_SPI* spi, bool closedloop_on, bool update_main_frame){
	// Set rotation
	ILI9341_setRotation(spi, 2);

	if(update_main_frame == true){
		// Set background
		ILI9341_Fill(spi, COLOR_NAVY);

		// Print frame - large square
		ILI9341_drawFastHLine(spi, 5, 235, 310, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 5, 310, COLOR_GREEN);
		ILI9341_drawFastVLine(spi, 5, 5, 230, COLOR_GREEN);
		ILI9341_drawFastVLine(spi, 315, 5, 230, COLOR_GREEN);

		// Print buttons - Small squares
		ILI9341_drawFastVLine(spi, 50, 5, 230, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 49, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 96, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 143, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 190, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 235, 45, COLOR_GREEN);

		// This line is for the input and output text
		ILI9341_drawFastHLine(spi, 50, 30, 265, COLOR_GREEN);

		// Apply them
		ILI9341_printImage(spi, 8, 8, 40, 40, mrac_gain_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 55, 40, 40, mrac_saturation_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 100, 40, 40, valve_delay_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 193, 40, 40, to_screen_1_icon, 40*80*sizeof(uint8_t));
	}

	// Change the icon if we are in control mode, or open loop mode
	if(closedloop_on == true)
		ILI9341_printImage(spi, 8, 146, 40, 40, closedloop_icon, 40*80*sizeof(uint8_t));
	else
		ILI9341_printImage(spi, 8, 146, 40, 40, openloop_icon, 40*80*sizeof(uint8_t));

}

// Create the main frame for j1939
void show_j1939_main_frame(ILI9341_SPI* spi, bool update_main_frame){
	// Set rotation
	ILI9341_setRotation(spi, 2);

	if(update_main_frame == true){
		// Set background
		ILI9341_Fill(spi, COLOR_NAVY);

		// Print frame - large square
		ILI9341_drawFastHLine(spi, 5, 235, 310, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 5, 310, COLOR_GREEN);
		ILI9341_drawFastVLine(spi, 5, 5, 230, COLOR_GREEN);
		ILI9341_drawFastVLine(spi, 315, 5, 230, COLOR_GREEN);

		// Print buttons - Small squares
		ILI9341_drawFastVLine(spi, 50, 5, 230, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 49, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 96, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 143, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 190, 45, COLOR_GREEN);
		ILI9341_drawFastHLine(spi, 5, 235, 45, COLOR_GREEN);

		// Apply them
		ILI9341_printImage(spi, 8, 8, 40, 40, set_address_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 55, 40, 40, set_configurations_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 100, 40, 40, error_log_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 146, 40, 40, save_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 193, 40, 40, to_screen_0_icon, 40*80*sizeof(uint8_t));
	}

}

// Show 12 j1939 error messages when we pressed the button
void show_j1939_DM1_error_messages_frame(ILI9341_SPI* spi, bool DM1_Messages_Binary[]){
	// Clear the screen , but not the icons
	ILI9341_Fill_Rect(spi, 51, 6, 314, 234, COLOR_NAVY);

	// Write the title
	ILI9341_drawFastHLine(spi, 50, 30, 265, COLOR_GREEN);
	char text_message[40];
	sprintf(text_message, "DM1 Error Messages. Green = OK");
	ILI9341_printText(spi, text_message, 55, 10, COLOR_YELLOW, COLOR_NAVY, 1);

	// Print out the error messages and mark them with red if they are true
	uint16_t text_color;
	for(uint8_t i = 0; i < 12; i++){
		if(i == 0){
			sprintf(text_message, "Address not claimed");
		}else if(i == 1){
			sprintf(text_message, "Inconsistent CAN Control message");
		}else if(i == 2){
			sprintf(text_message, "No CAN Control message received");
		}else if(i == 3){
			sprintf(text_message, "EEPROM parameter write/read error");
		}else if(i == 4){
			sprintf(text_message, "Motor stall detected");
		}else if(i == 5){
			sprintf(text_message, "Undervoltage - Need more voltage");
		}else if(i == 6){
			sprintf(text_message, "Overvoltage - Need less voltage");
		}else if(i == 7){
			sprintf(text_message, "SCU temperature higher than");
		}else if(i == 8){
			sprintf(text_message, "SCU temperature below");
		}else if(i == 9){
			sprintf(text_message, "Current exceeds upper limit");
		}else if(i == 10){
			sprintf(text_message, "Error position in IDLE Mode");
		}else if(i == 11){
			sprintf(text_message, "Range error for Linearization parameter");
		}
		text_color = DM1_Messages_Binary[i] == true ? COLOR_RED : COLOR_GREEN;
		ILI9341_printText(spi, text_message, 55, 35 + 10*i, text_color, COLOR_NAVY, 1);
	}

	// Place a reset button for the messages
	ILI9341_printImage(spi, 90, 160, 80, 40, reset_button, 80*40*2*sizeof(uint8_t));
}

// Show the CA names from where we got when we claimed the address
void show_j1939_CA_names_frame(ILI9341_SPI* spi, uint8_t CA[], uint8_t SA_ACU){
	// Clear the screen , but not the icons
	ILI9341_Fill_Rect(spi, 51, 6, 314, 234, COLOR_NAVY);

	// Write the title
	ILI9341_drawFastHLine(spi, 50, 30, 265, COLOR_GREEN);
	char text_message[40];
	sprintf(text_message, "CA Names J1939 and source address");
	ILI9341_printText(spi, text_message, 55, 10, COLOR_YELLOW, COLOR_NAVY, 1);

	// Print out the CA names
	uint32_t identity_number = ((CA[2] & 0b00011111) << 16) | (CA[1] << 8) | CA[0];
	uint16_t manufacturer_code = (CA[3] << 3) | (CA[2] >> 5);
	uint8_t function_instance = CA[4] >> 3;
	uint8_t ECU_instance = CA[4] & 0b00000111;
	uint8_t function = CA[5];
	uint8_t vehicle_system = CA[6] >> 1;
	uint8_t arbitrary_address_capable = CA[7] >> 7;
	uint8_t industry_group = (CA[7] >> 4) & 0b0111;
	uint8_t vehicle_system_instance = CA[7] & 0b00001111;

	for(uint8_t i = 0; i < 10; i++){
		if(i == 0){
			sprintf(text_message, "Identity:%d", identity_number);
		}else if(i == 1){
			sprintf(text_message, "Manufacturer code:%d", manufacturer_code);
		}else if(i == 2){
			sprintf(text_message, "Function instance:%d", function_instance);
		}else if(i == 3){
			sprintf(text_message, "ECU instance:%d", ECU_instance);
		}else if(i == 4){
			sprintf(text_message, "Function:%d", function);
		}else if(i == 5){
			sprintf(text_message, "Vehicle system:%d", vehicle_system);
		}else if(i == 6){
			sprintf(text_message, "Arbitrary address capable:%d", arbitrary_address_capable);
		}else if(i == 7){
			sprintf(text_message, "Industry group:%d", industry_group);
		}else if(i == 8){
			sprintf(text_message, "Vehicle system instance:%d", vehicle_system_instance);
		}else if(i == 9){
			sprintf(text_message, "ACU Source address:%d", SA_ACU);
		}

		ILI9341_printText(spi, text_message, 55, 35 + 10*i, COLOR_YELLOW, COLOR_NAVY, 1);
	}

	// Place two buttons - Change and request
	ILI9341_printImage(spi, 90, 160, 80, 40, change_button, 80*40*2*sizeof(uint8_t));
	ILI9341_printImage(spi, 200, 160, 80, 40, request_button, 80*40*2*sizeof(uint8_t));

}

// Set the CA names + source address
bool show_j1939_set_CA_names_numpad(ILI9341_SPI* spi, uint8_t CA[], uint8_t* SA_ACU){
	bool decimalbutton_show = false;
	bool minusbutton_show = false;

	// Initial values
	float identity_number = ((CA[2] & 0b00011111) << 16) | (CA[1] << 8) | CA[0];
	float manufacturer_code = (CA[3] << 3) | (CA[2] >> 5);
	float function_instance = CA[4] >> 3;
	float ECU_instance = CA[4] & 0b00000111;
	float function = CA[5];
	float vehicle_system = CA[6] >> 1;
	float arbitrary_address_capable = CA[7] >> 7;
	float industry_group = (CA[7] >> 4) & 0b0111;
	float vehicle_system_instance = CA[7] & 0b00001111;
	float source_address = *SA_ACU;
	uint8_t CA_new[9];
	uint8_t status;
	for(uint8_t i = 0; i < 10; i++){
		if(i == 0){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &identity_number, "Set identity 0 to 2097151");
			if(identity_number > 2097151)
				identity_number = 2097151;
		}else if(i == 1){
			manufacturer_code = 327; // Always
		}else if(i == 2){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &function_instance, "Set function instance 0 to 15");
			if(function_instance > 15)
				function_instance = 15;
		}else if(i == 3){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &ECU_instance, "Set ECU instance 0 to 7");
			if(ECU_instance > 7)
				ECU_instance = 7;
		}else if(i == 4){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &function, "Set function 0 to 255");
			if(function > 255)
				function = 255;
		}else if(i == 5){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &vehicle_system, "Set vehicle system 0 to 127");
			if(vehicle_system > 127)
				vehicle_system = 127;
		}else if(i == 6){
			arbitrary_address_capable = 0; // Always
		}else if(i == 7){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &industry_group, "Set industry group 0 to 7");
			if(industry_group > 7)
				industry_group = 7;
		}else if(i == 8){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &vehicle_system_instance, "Set vehicle system instance 0 to 15");
			if(vehicle_system > 15)
				vehicle_system = 15;
		}else if(i == 9){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &source_address, "Set ACU source address 128 to 143");
			if(source_address < 128)
				source_address = 128;
			else if(source_address > 143)
				source_address = 143;
		}
		// If we abort
		if(status == false){
			return false;
		}
	}
	// Make sure that all are uint8_t for bitwise operations
	uint32_t identity_number_u = identity_number;
	uint16_t manufacturer_code_u = manufacturer_code;
	uint8_t function_instance_u = function_instance;
	uint8_t ECU_instance_u = ECU_instance;
	uint8_t function_u = function;
	uint8_t vehicle_system_u = vehicle_system;
	uint8_t arbitrary_address_capable_u = arbitrary_address_capable;
	uint8_t industry_group_u = industry_group;
	uint8_t vehicle_system_instance_u = vehicle_system_instance;
	uint8_t SA_ACU_new = source_address;

	// Create the data package now
	CA_new[0] = identity_number_u;
	CA_new[1] = identity_number_u >> 8;
	CA_new[2] = (manufacturer_code_u << 5) | (identity_number_u >> 16);
	CA_new[3] = manufacturer_code_u >>  3;
	CA_new[4] = (function_instance_u << 3) | ECU_instance_u;
	CA_new[5] = function_u;
	CA_new[6] = vehicle_system_u << 1;
	CA_new[7] = (arbitrary_address_capable_u << 7) | (industry_group_u << 4) | vehicle_system_instance_u;

	// Call the frame and show the user the future settings
	bool update_main_frame = true;
	show_j1939_main_frame(spi, update_main_frame);
	show_j1939_CA_names_frame(spi, CA_new, SA_ACU_new);

	// Place two buttons - Abort or flash
	ILI9341_printImage(spi, 90, 160, 80, 40, abort_button, 80*40*2*sizeof(uint8_t)); // This will overwrite the "Change" button
	ILI9341_printImage(spi, 200, 160, 80, 40, flash_button, 80*40*2*sizeof(uint8_t));

	// Select buttons if we want to use these settings or not
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if(X >= 100 && X <= 160 && Y >= 170 && Y <= 197){
				// Abort button - Just quit
				return false;
			}else if (X >= 215 && X <= 277 && Y >= 170 && Y <= 197){
				// Flash button - Move over new data and flash
				memcpy(CA, CA_new, 8);
				*SA_ACU = SA_ACU_new;
				return true;
			}
		}
	}
}

bool show_j1939_set_configurations_numpad(ILI9341_SPI* spi, uint16_t EEPROM_values[], uint8_t DM15_Status){
	bool decimalbutton_show = false;
	bool minusbutton_show = false;

	// Initial values
	float Sensor_Stall_Neutral_Enable = EEPROM_values[0];
	float Sensor_Stall_Normal_Enable = EEPROM_values[1];
	uint8_t status;
	for(uint8_t i = 0; i < 2; i++){
		if(i == 0){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &Sensor_Stall_Neutral_Enable, "Set Sensor Stall Neutral Enable 0 or 1");
			if(Sensor_Stall_Neutral_Enable > 1)
				Sensor_Stall_Neutral_Enable = 1;
		}else if(i == 1){
			status = show_num_pad_frame(spi, decimalbutton_show, minusbutton_show, &Sensor_Stall_Normal_Enable, "Set Sensor Stall Normal Enable 0 or 1");
			if(Sensor_Stall_Normal_Enable > 1)
				Sensor_Stall_Normal_Enable = 1;
		}
		// If we abort
		if(status == false){
			return false;
		}
	}

	// Call the frame and show the user the future settings
	bool update_main_frame = true;
	show_j1939_main_frame(spi, update_main_frame);
	uint16_t EEPROM_values_new[2] = {Sensor_Stall_Normal_Enable, Sensor_Stall_Neutral_Enable};
	show_j1939_configurations_frame(spi, EEPROM_values_new, DM15_Status);

	// Place two buttons - Abort or flash
	ILI9341_printImage(spi, 90, 160, 80, 40, abort_button, 80*40*2*sizeof(uint8_t)); // This will overwrite the "Change" button
	ILI9341_printImage(spi, 200, 160, 80, 40, flash_button, 80*40*2*sizeof(uint8_t));

	// Select buttons if we want to use these settings or not
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if(X >= 100 && X <= 160 && Y >= 170 && Y <= 197){
				// Abort button - Just quit
				return false;
			}else if (X >= 215 && X <= 277 && Y >= 170 && Y <= 197){
				// Flash button - Move over new data and flash
				EEPROM_values[0] = Sensor_Stall_Normal_Enable;
				EEPROM_values[1] = Sensor_Stall_Neutral_Enable;
				return true;
			}
		}
	}

}

// Show the configuration frame
void show_j1939_configurations_frame(ILI9341_SPI* spi, uint16_t EEPROM_values[], uint8_t DM15_Status){
	// Clear the screen , but not the icons
	ILI9341_Fill_Rect(spi, 51, 6, 314, 234, COLOR_NAVY);

	// Write the title
	ILI9341_drawFastHLine(spi, 50, 30, 265, COLOR_GREEN);
	char text_message[40];
	sprintf(text_message, "Parameter configuration");
	ILI9341_printText(spi, text_message, 55, 10, COLOR_YELLOW, COLOR_NAVY, 1);

	// Show current settings
	for(uint8_t i = 0; i < 3; i++){
		if(i == 0){
			sprintf(text_message, "Sensor Stall Neutral Enable:%d", EEPROM_values[0]);
		}else if(i == 1){
			sprintf(text_message, "Sensor Stall Normal Enable:%d", EEPROM_values[1]);
		}else if(i == 2){
			if(DM15_Status == 0){
				sprintf(text_message, "DM15 status message:Proceed");
			}else if(DM15_Status == 1){
				sprintf(text_message, "DM15 status message:Busy");
			}else if(DM15_Status == 4){
				sprintf(text_message, "DM15 status message:Operation completed");
			}else if(DM15_Status == 5){
				sprintf(text_message, "DM15 status message:Operation failed");
			}else if(DM15_Status == 8){
				sprintf(text_message, "Please, clear the error codes first");
			}
		}
		ILI9341_printText(spi, text_message, 55, 35 + 10*i, COLOR_YELLOW, COLOR_NAVY, 1);
		sprintf(text_message, ""); // Clear
	}

	// Place two buttons - Change and request
	ILI9341_printImage(spi, 90, 160, 80, 40, change_button, 80*40*2*sizeof(uint8_t));
	ILI9341_printImage(spi, 200, 160, 80, 40, request_button, 80*40*2*sizeof(uint8_t));

}

// Create numpad
uint8_t show_num_pad_frame(ILI9341_SPI* spi, bool decimalbutton_show, bool minusbutton_show, float* number_value, char* title){
	// Set background
	ILI9341_Fill(spi, COLOR_BLACK);

	// Set rotation
	ILI9341_setRotation(spi, 2);

	// Print frame - large square
	ILI9341_drawFastHLine(spi, 5, 235, 310, COLOR_GREEN);
	ILI9341_drawFastHLine(spi, 5, 5, 310, COLOR_GREEN);
	ILI9341_drawFastVLine(spi, 5, 5, 230, COLOR_GREEN);
	ILI9341_drawFastVLine(spi, 315, 5, 230, COLOR_GREEN);

	// Create gray, white square
	ILI9341_Fill_Rect(spi, 6, 6, 314, 55, COLOR_DGRAY);
	ILI9341_Fill_Rect(spi, 12, 24, 308, 50, COLOR_WHITE);

	// Set title
	ILI9341_printText(spi, title, 12, 12, COLOR_WHITE, COLOR_DGRAY, 1);

	// Create buttons
	uint16_t x = 0;
	char number[3];
	for(uint8_t i = 0; i < 5; i++){
		x += 23;
		// Upper numbers
		ILI9341_Fill_Rect(spi, x, 70, x+40, 110, COLOR_DGRAY);
		ILI9341_Fill_Rect(spi, x+5, 75, x+35, 105, COLOR_WHITE);
		sprintf_(number, "%d", i);
		ILI9341_printText(spi, number, x+16, 83, COLOR_BLACK, COLOR_WHITE, 2);

		// Lower numbers
		ILI9341_Fill_Rect(spi, x, 125, x+40, 165, COLOR_DGRAY);
		ILI9341_Fill_Rect(spi, x+5, 130, x+35, 160, COLOR_WHITE);
		sprintf_(number, "%d", i+5);
		ILI9341_printText(spi, number, x+16, 138, COLOR_BLACK, COLOR_WHITE, 2);

		// This hide or show the buttons
		if(i == 0 && minusbutton_show == true){
			ILI9341_Fill_Rect(spi, x, 180, x+40, 220, COLOR_DGRAY);
			ILI9341_Fill_Rect(spi, x+5, 185, x+35, 215, COLOR_WHITE);
		}else if(i == 1 && decimalbutton_show == true){
			ILI9341_Fill_Rect(spi, x, 180, x+40, 220, COLOR_DGRAY);
			ILI9341_Fill_Rect(spi, x+5, 185, x+35, 215, COLOR_WHITE);
		}
		if(i > 1){
			ILI9341_Fill_Rect(spi, x, 180, x+40, 220, COLOR_DGRAY);
			ILI9341_Fill_Rect(spi, x+5, 185, x+35, 215, COLOR_WHITE);
		}

		// Give the buttons a text
		if(i == 0 && minusbutton_show == true){
			sprintf_(number, "-");
			ILI9341_printText(spi, number, x+15, 193, COLOR_BLACK, COLOR_WHITE, 2);
		}else if(i == 1 && decimalbutton_show == true){
			sprintf_(number, ".");
			ILI9341_printText(spi, number, x+15, 193, COLOR_BLACK, COLOR_WHITE, 2);
		}else if(i == 2){
			sprintf_(number, "C");
			ILI9341_printText(spi, number, x+15, 193, COLOR_BLACK, COLOR_WHITE, 2);
		}else if(i == 3){
			sprintf_(number, "<-");
			ILI9341_printText(spi, number, x+9, 193, COLOR_BLACK, COLOR_WHITE, 2);
		}else if(i == 4){
			sprintf_(number, "OK");
			ILI9341_printText(spi, number, x+9, 193, COLOR_BLACK, COLOR_WHITE, 2);
		}
		x += 35; // Space between buttons
	}

	// Here is the logic
	char text[17];
	bool haveDot = false;
	uint8_t k = 0;

	// Display the current value
	sprintf_(text, "%f", *number_value); // Float value to text
	ILI9341_printText(spi, text, 15, 25, COLOR_BLACK, COLOR_WHITE, 3);
	HAL_Delay(2000);
	ILI9341_Fill_Rect(spi, 12, 24, 308, 50, COLOR_WHITE); // Clear
	memset(text, 0, sizeof(text));

	// Select numbers
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if (X >= 23 && X <= 63 && Y >= 70 && Y <= 110) {
				// 0
				if (k >= 0 && k < 16) {
					text[k] = '0';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 70 && Y <= 110) {
				// 1
				if(k < 16){
					text[k] = '1';
					k++;
				}
			} else if (X >= 139 && X <= 179 && Y >= 70 && Y <= 110) {
				// 2
				if(k < 16){
					text[k] = '2';
					k++;
				}
			} else if (X >= 197 && X <= 237 && Y >= 70 && Y <= 110) {
				// 3
				if(k < 16){
					text[k] = '3';
					k++;
				}
			} else if (X >= 255 && X <= 295 && Y >= 70 && Y <= 110) {
				// 4
				text[k] = '4';
				k++;
			} else if (X >= 23 && X <= 63 && Y >= 125 && Y <= 165) {
				// 5
				if(k < 16){
					text[k] = '5';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 125 && Y <= 165) {
				// 6
				if(k < 16){
					text[k] = '6';
					k++;
				}
			} else if (X >= 139 && X <= 179 && Y >= 125 && Y <= 165) {
				// 7
				if(k < 16){
					text[k] = '7';
					k++;
				}
			} else if (X >= 197 && X <= 237 && Y >= 125 && Y <= 165) {
				// 8
				if(k < 16){
					text[k] = '8';
					k++;
				}
			} else if (X >= 255 && X <= 295 && Y >= 125 && Y <= 165) {
				// 9
				if(k < 16){
					text[k] = '9';
					k++;
				}
			} else if (X >= 23 && X <= 63 && Y >= 180 && Y <= 220) {
				// -
				if (minusbutton_show == true && k == 0) {
					text[k] = '-';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 180 && Y <= 220) {
				// .
				if (decimalbutton_show == true && k > 0 && haveDot == false && k < 16) {
					text[k] = '.';
					k++;
					haveDot = true;
				}
			} else if (X >= 139 && X <= 179 && Y >= 180 && Y <= 220) {
				// C
				memset(text, 0, sizeof(text));
				k = 0;
				haveDot = false;
				ILI9341_Fill_Rect(spi, 12, 24, 308, 50, COLOR_WHITE); // Clear
			} else if (X >= 197 && X <= 237 && Y >= 180 && Y <= 220) {
				// <-
				return 0;
			} else if (X >= 255 && X <= 295 && Y >= 180 && Y <= 220) {
				// OK
				*number_value = (float)atof(text); // char array to float
				return 1;
			}
			// Display the selected number text
			ILI9341_printText(spi, text, 15, 25, COLOR_BLACK, COLOR_WHITE, 3);
		}
		osDelay(100); // Wait 100 ms so we don't double click
	}
}

uint8_t show_question_save_settings_dialog(ILI9341_SPI *spi){
	ILI9341_printImage(spi, 40, 80, 250, 83, question_save_settings_dialog, 500*83*sizeof(uint8_t));
	// Select numbers
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if (X >= 120 && X <= 155 && Y >= 140 && Y <= 155) {
				ILI9341_drawFastHLine(spi, 123, 136, 37, COLOR_MAGENTA);
				ILI9341_drawFastHLine(spi, 123, 154, 37, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 123, 137, 17, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 160, 137, 17, COLOR_MAGENTA);
				return 1; // Yes
			}else if(X >= 170 && X <= 205 && Y >= 140 && Y <= 155){
				ILI9341_drawFastHLine(spi, 170, 136, 38, COLOR_MAGENTA);
				ILI9341_drawFastHLine(spi, 170, 154, 38, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 170, 137, 17, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 207, 137, 17, COLOR_MAGENTA);
				return 0; // NO
			}
		}
		osDelay(100); // Wait 100 ms so we don't double click
	}
}

static void move(uint16_t i, uint16_t inputs0[], uint16_t inputs1[], uint16_t outputs[], uint16_t references[], uint16_t new_input0, uint16_t new_input1, uint16_t new_output, uint16_t new_reference);

void show_plot_frame(ILI9341_SPI *spi, float new_input0_float, float new_input1_float, float new_output_float, float new_reference_float, uint16_t new_input0_uint, uint16_t new_input1_uint, uint16_t new_output_uint, uint16_t new_reference_uint, bool only_move_array, bool clear_plot) {
	// We plot from x1 = 51 to x2 = 314, which is 263 pixels. Max value the plot can show is 203
	static uint16_t inputs0[264];
	static uint16_t inputs1[264];
	static uint16_t outputs[264];
	static uint16_t references[264];
	// Add more static arrays here

	// If we want to clear the plot for some reason
	if(clear_plot == true){
		memset(inputs0, 0, sizeof(inputs0));
		memset(inputs1, 0, sizeof(inputs1));
		memset(outputs, 0, sizeof(outputs));
		memset(references, 0, sizeof(references));
		// Add here
	}

	if (only_move_array == false) {
		// Update the text
		char text[20];
		sprintf_(text, "Input0:%0.2f ", new_input0_float);
		ILI9341_printText(spi, text, 55, 10, COLOR_RED, COLOR_NAVY, 1);
		sprintf_(text, "Input1:%0.2f  ", new_input1_float);
		ILI9341_printText(spi, text, 145, 10, COLOR_YELLOW, COLOR_NAVY, 1);
		sprintf_(text, "Ref:%0.2f", new_reference_float);
		ILI9341_printText(spi, text, 240, 10, COLOR_MAGENTA, COLOR_NAVY, 1);
		sprintf_(text, "Output:%0.2f    ", new_output_float);
		ILI9341_printText(spi, text, 55, 20, COLOR_CYAN, COLOR_NAVY, 1);
		// Add more printText functions here

		// Change
		for (int16_t i = 51; i < 315; i++) {
			// Clear
			ILI9341_DrawPixel(spi, i, -inputs0[i - 51] + 234, COLOR_NAVY);
			ILI9341_DrawPixel(spi, i, -inputs1[i - 51] + 234, COLOR_NAVY);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_NAVY);
			ILI9341_DrawPixel(spi, i, -references[i - 51] + 234, COLOR_NAVY);
			// Add more clear pixels here

			// Move
			move(i, inputs0, inputs1, outputs, references, new_input0_uint, new_input1_uint, new_output_uint, new_reference_uint);
			// Add more move arrays here

			// Fill
			ILI9341_DrawPixel(spi, i, -inputs0[i - 51] + 234, COLOR_RED);
			ILI9341_DrawPixel(spi, i, -inputs1[i - 51] + 234, COLOR_YELLOW);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_CYAN);
			ILI9341_DrawPixel(spi, i, -references[i - 51] + 234, COLOR_MAGENTA);
			// Add more fill pixles here
		}
	} else {
		for (int16_t i = 51; i < 315; i++) {
			move(i, inputs0, inputs1, outputs, references, new_input0_uint, new_input1_uint, new_output_uint, new_reference_uint);
			// Add more move arrays here
		}
	}
}

static void move(uint16_t i, uint16_t inputs0[], uint16_t inputs1[], uint16_t outputs[], uint16_t references[], uint16_t new_input0, uint16_t new_input1, uint16_t new_output, uint16_t new_reference) {
	if (i < 314) {
		// We can only move values that have a next element
		inputs0[i - 51] = inputs0[i - 50];
		inputs1[i - 51] = inputs1[i - 50];
		outputs[i - 51] = outputs[i - 50];
		references[i - 51] = references[i - 50];
	} else {
		// Limits so we don't write outside the box
		if (new_input0 < 203) {
			inputs0[i - 51] = new_input0;
		} else {
			inputs0[i - 51] = 203;
		}

		if (new_input1 < 203) {
			inputs1[i - 51] = new_input1;
		} else {
			inputs1[i - 51] = 203;
		}

		if (new_output < 203) {
			outputs[i - 51] = new_output;
		} else {
			outputs[i - 51] = 203;
		}

		if (new_reference < 203) {
			references[i - 51] = new_reference;
		} else {
			references[i - 51] = 203;
		}
	}
}
