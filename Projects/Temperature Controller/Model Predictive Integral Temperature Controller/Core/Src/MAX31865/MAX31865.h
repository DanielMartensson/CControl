
#ifndef MAX31865_LIB_MAX31865_H
#define MAX31865_LIB_MAX31865_H

#include "main.h"

// Sensor defines
#define MAX31865_RREF 434                        // Reference resistor
#define MAX31865_FACTOR 32768                    // 2^15 used for data to resistance conversion
#define MAX31865_ALPHA 0.00387                   // PT-100 temperature coefficient

// User defines
#define WIRE4 4
#define WIRE3 3
#define WIRE2 2

typedef struct {
    GPIO_TypeDef* CE_PORT;
    uint16_t CE_PIN;
    SPI_HandleTypeDef* hspi;
    uint8_t WIRES;
} MAX31865_SPI;

void MAX31865_init(MAX31865_SPI* spi, GPIO_TypeDef* CE_PORT, uint16_t CE_PIN, SPI_HandleTypeDef* hspi, uint8_t WIRES);
float MAX31865_temperature(MAX31865_SPI* spi);
uint8_t MAX31865_configuration(MAX31865_SPI* spi);
uint16_t MAX31865_raw_resistance(MAX31865_SPI* spi);
#endif //MAX31865_LIB_MAX31865_H
