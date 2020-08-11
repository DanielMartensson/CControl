
#include "MAX31865.h"

/*
 * Write to
 */
static void MAX31865_write(MAX31865_SPI* spi, uint8_t* address, uint8_t* data, uint16_t lenght) {

	// Enable CE
	HAL_GPIO_WritePin(spi->CE_PORT, spi->CE_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi->hspi, address, 1, 10);
	HAL_SPI_Transmit(spi->hspi, data, lenght, 10);
	// Disable CE
	HAL_GPIO_WritePin(spi->CE_PORT, spi->CE_PIN, GPIO_PIN_SET);
}

/*
 * Read from
 */
static void MAX31865_read(MAX31865_SPI* spi, uint8_t* address, uint8_t* buffer, uint16_t lenght) {

	// Enable CE
	HAL_GPIO_WritePin(spi->CE_PORT, spi->CE_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi->hspi, address, 1, 10);
	HAL_SPI_Receive(spi->hspi, buffer, lenght, 10);
	// Disable CE
	HAL_GPIO_WritePin(spi->CE_PORT, spi->CE_PIN, GPIO_PIN_SET);

}

/*
 * Read temperature
 */
float MAX31865_temperature(MAX31865_SPI* spi) {

	uint8_t buffer[2];
	uint8_t address = 0x01; // MAX31865_RTD_MSB
	MAX31865_read(spi, &address, buffer, 2);

	// Combine 2 bytes into 1 number, and shift 1 down to remove fault bit
	uint16_t data = buffer[0] << 8;
	data |= buffer[1];
	data >>= 1;

	// Calculate the actual resistance of the sensor
	float resistance = ((float) data * MAX31865_RREF) / MAX31865_FACTOR;

	// Calculate the temperature from the measured resistance
	float temp = ((resistance / 100) - 1) / MAX31865_ALPHA;

	return temp;

}

/*
 * Get the raw resistance
 */
uint16_t MAX31865_raw_resistance(MAX31865_SPI* spi){
	uint8_t buffer[2];
	uint8_t address = 0x01; // MAX31865_RTD_MSB
	MAX31865_read(spi, &address, buffer, 2);

	// Combine 2 bytes into 1 number, and shift 1 down to remove fault bit
	uint16_t data = buffer[0] << 8;
	data |= buffer[1];
	data >>= 1;
	return data;
}

/*
 * Get the configuration
 */
uint8_t MAX31865_configuration(MAX31865_SPI* spi){
	uint8_t address = 0x00; // MAX31865_CONFIGURATION
	uint8_t buffer;
	MAX31865_read(spi, &address, &buffer, 1);
	return buffer;
}

void MAX31865_init(MAX31865_SPI* spi, GPIO_TypeDef* CE_PORT, uint16_t CE_PIN, SPI_HandleTypeDef* hspi, uint8_t WIRES){
	// Declare structure
	spi->CE_PIN = CE_PIN;
	spi->CE_PORT = CE_PORT;
	spi->hspi = hspi;
	spi->WIRES = WIRES;

	// CE in reset state
	HAL_GPIO_WritePin(spi->CE_PORT, spi->CE_PIN, GPIO_PIN_SET);

	/*
	 * SPI settings:
	 * CPHA = 2 Edge
	 * Prescaler = 256
	 * First bit = MBS first
	 * CPOL = High
	 */

	// Do default configuration and begin with creating the data
	/*
	 * VBias =  ON
	 * Conversion mode = Auto
	 * 1-shot = Off
	 * Fault detection = No action
	 * Fault Status = Off
	 * Filter = 60 Hz
	 */
	uint8_t data = 0x00;
	if (spi->WIRES == WIRE3) {
		data = 0b11010000; // 3 wire
	} else {
		data = 0b11000000; // 2 or 4 wire
	}

	// 0x80 will convert to WRITE address
	uint8_t address = 0x00 | 0x80; // MAX31865_CONFIGURATION
	MAX31865_write(spi, &address, &data, 1);


}
