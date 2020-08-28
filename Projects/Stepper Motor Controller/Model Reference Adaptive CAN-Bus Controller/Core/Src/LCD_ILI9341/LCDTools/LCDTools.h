/*
 * LCDTools.h
 *
 *  Created on: May 22, 2020
 *      Author: Daniel Mårtensson
 */


#ifndef SRC_LCD_ILI9341_LCDTOOLS_LCDTOOLS_H_
#define SRC_LCD_ILI9341_LCDTOOLS_LCDTOOLS_H_

#include "stdbool.h"
#include <stdio.h> // For sprintf
#include <string.h> // For memset
#include "../LCDCore/LCD_ILI9341.h"
#include "pictures.h"
#include "printf.h"
#include "cmsis_os.h"


void show_j1939_main_frame(ILI9341_SPI* spi, bool update_main_frame);
void show_j1939_DM1_error_messages_frame(ILI9341_SPI* spi, bool DM1_Messages_Binary[]);

void show_j1939_CA_names_frame(ILI9341_SPI* spi, uint8_t CA[], uint8_t SA_ACU);
bool show_j1939_set_CA_names_numpad(ILI9341_SPI* spi, uint8_t CA[], uint8_t* SA_ACU);

void show_j1939_configurations_frame(ILI9341_SPI* spi, uint16_t EEPROM_values[], uint8_t DM15_Status);
bool show_j1939_set_configurations_numpad(ILI9341_SPI* spi, uint16_t EEPROM_values[], uint8_t DM15_Status);

uint8_t show_num_pad_frame(ILI9341_SPI* spi, bool decimalbutton_show, bool minusbutton_show, float* number_value, char* title);
void show_plot_frame(ILI9341_SPI *spi, float new_input0_float, float new_input1_float, float new_output_float, float new_reference_float, uint16_t new_input0_uint, uint16_t new_input1_uint, uint16_t new_output_uint, uint16_t new_reference_uint, bool only_move_array, bool clear_plot);
uint8_t show_question_save_settings_dialog(ILI9341_SPI *spi);
void show_main_frame_mrac(ILI9341_SPI* spi, bool closedloop_on, bool update_main_frame);

#endif /* SRC_LCD_ILI9341_LCDTOOLS_LCDTOOLS_H_ */
