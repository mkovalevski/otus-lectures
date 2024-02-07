#ifndef SPI_COM
#define SPI_COM

#include "bsp.h"

void spi_bus_init(void);
void matrix_display_task(void * params);
void spi_bus_config_device(spi_device_handle_t * hSpi);


#endif