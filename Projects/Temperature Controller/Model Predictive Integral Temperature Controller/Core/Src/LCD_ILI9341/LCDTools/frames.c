/*
 * frames.c
 *
 *  Created on: May 22, 2020
 *      Author: Daniel Mårtensson
 */

#include "LCDTools.h"

// Create the main frame
void show_main_frame(ILI9341_SPI* spi, bool closedloop_on, bool update_main_frame){
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
		ILI9341_printImage(spi, 8, 8, 40, 40, setpoint_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 55, 40, 40, horizon_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi,  8, 100, 40, 40, time_constant_icon, 40*80*sizeof(uint8_t));
		ILI9341_printImage(spi, 8, 193, 40, 40, save_icon, 40*80*sizeof(uint8_t));
	}

	// Change the icon if we are in control mode, or open loop mode
	if(closedloop_on == true)
		ILI9341_printImage(spi, 8, 146, 40, 40, closedloop_icon, 40*80*sizeof(uint8_t));
	else
		ILI9341_printImage(spi, 8, 146, 40, 40, openloop_icon, 40*80*sizeof(uint8_t));

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

static void move(uint16_t i, uint16_t inputs[], uint16_t outputs[], uint16_t new_input, uint16_t new_output);

void show_plot_frame(ILI9341_SPI *spi, float new_input_float, float new_output_float, uint16_t new_input_uint, uint16_t new_output_uint, bool only_move_array, bool clear_plot) {
	// We plot from x1 = 51 to x2 = 314, which is 263 pixels. Max value the plot can show is 203
	static uint16_t inputs[264];
	static uint16_t outputs[264];
	// Add more static arrays here

	// If we want to clear the plot for some reason
	if(clear_plot == true){
		memset(inputs, 0, sizeof(inputs));
		memset(outputs, 0, sizeof(outputs));
		// Add here
	}

	if (only_move_array == false) {
		// Update the text
		char text[20];
		sprintf_(text, "Input:%0.2f    ", new_input_float);
		ILI9341_printText(spi, text, 55, 10, COLOR_RED, COLOR_NAVY, 1);
		sprintf_(text, "Output:%0.2f    ", new_output_float);
		ILI9341_printText(spi, text, 55, 20, COLOR_CYAN, COLOR_NAVY, 1);
		// Add more printText functions here

		// Change
		for (int16_t i = 51; i < 315; i++) {
			// Clear
			ILI9341_DrawPixel(spi, i, -inputs[i - 51] + 234, COLOR_NAVY);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_NAVY);
			// Add more clear pixels here

			// Move
			move(i, inputs, outputs, new_input_uint, new_output_uint);
			// Add more move arrays here

			// Fill
			ILI9341_DrawPixel(spi, i, -inputs[i - 51] + 234, COLOR_RED);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_CYAN);
			// Add more fill pixles here
		}
	} else {
		for (int16_t i = 51; i < 315; i++) {
			move(i, inputs, outputs, new_input_uint, new_output_uint);
			// Add more move arrays here
		}
	}
}

static void move(uint16_t i, uint16_t inputs[], uint16_t outputs[], uint16_t new_input, uint16_t new_output) {
	if (i < 314) {
		// We can only move values that have a next element
		inputs[i - 51] = inputs[i - 50];
		outputs[i - 51] = outputs[i - 50];
	} else {
		// Limits so we don't write outside the box
		if (new_input < 203) {
			inputs[i - 51] = new_input;
		} else {
			inputs[i - 51] = 203;
		}
		if (new_output < 203) {
			outputs[i - 51] = new_output;
		} else {
			outputs[i - 51] = 203;
		}
	}
}
