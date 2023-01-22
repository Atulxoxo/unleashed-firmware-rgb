#ifndef SPI_SWD_H_
#define SPI_SWD_H_
#include <furi.h>
#include "furi_hal_spi.h"

void swd_spi_bus_handle_subghz_event_callback(
    FuriHalSpiBusHandle* handle,
    FuriHalSpiBusHandleEvent event);

bool swd_spi_bus_trx(
    FuriHalSpiBusHandle* handle,
    uint8_t* tx_buffer,
    uint8_t* rx_buffer,
    size_t size,
    uint32_t timeout);

void swd_spi_acquire(FuriHalSpiBusHandle* handle);

void swd_spi_release(FuriHalSpiBusHandle* handle);

extern const GpioPin swd_spi_miso;
extern const GpioPin swd_spi_sck;
extern const GpioPin swd_spi_mosi;
extern const GpioPin swd_spi_cs;
extern const GpioPin swd_cc1101_g0;

extern FuriHalSpiBusHandle swd_spi_bus_handle_subghz;

#endif