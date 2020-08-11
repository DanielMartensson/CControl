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

void show_main_frame(ILI9341_SPI* spi, bool closedloop_on, bool update_main_frame);
uint8_t show_num_pad_frame(ILI9341_SPI* spi, bool decimalbutton_show, bool minusbutton_show, float* number_value, char* title);
void show_plot_frame(ILI9341_SPI *spi, float new_input_float, float new_output_float, uint16_t new_input_uint, uint16_t new_output_uint, bool only_move_array, bool clear_plot);
uint8_t show_question_save_settings_dialog(ILI9341_SPI *spi);


#endif /* SRC_LCD_ILI9341_LCDTOOLS_LCDTOOLS_H_ */
