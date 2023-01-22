#include "spi_swd.h"
#include <furi_hal_power.h>

FuriHalSpiBusHandle swd_spi_bus_handle_subghz = {
    .bus = NULL,
    .callback = swd_spi_bus_handle_subghz_event_callback,
    .miso = &swd_spi_miso,
    .mosi = &swd_spi_mosi,
    .sck = &swd_spi_sck,
    .cs = &swd_spi_cs,
};

/* Пины для подключения CC1101 */
const GpioPin swd_spi_miso = {.port = GPIOA, .pin = LL_GPIO_PIN_7}; //2
const GpioPin swd_spi_sck = {.port = GPIOA, .pin = LL_GPIO_PIN_6}; //3
const GpioPin swd_spi_mosi = {.port = GPIOA, .pin = LL_GPIO_PIN_4}; //4
const GpioPin swd_spi_cs = {.port = GPIOB, .pin = LL_GPIO_PIN_2}; //6
const GpioPin swd_cc1101_g0 = {.port = GPIOB, .pin = LL_GPIO_PIN_3}; //5

void swd_spi_bus_handle_subghz_event_callback(
    FuriHalSpiBusHandle* handle,
    FuriHalSpiBusHandleEvent event) {
    if(event == FuriHalSpiBusHandleEventInit) {
        furi_hal_gpio_write(handle->cs, true);
        furi_hal_gpio_init(handle->cs, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    } else if(event == FuriHalSpiBusHandleEventDeinit) {
        furi_hal_gpio_write(handle->cs, true);
        furi_hal_gpio_init(handle->cs, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    } else if(event == FuriHalSpiBusHandleEventActivate) {
        furi_hal_gpio_init(handle->miso, GpioModeInput, GpioPullNo, GpioSpeedVeryHigh);
        furi_hal_gpio_init(handle->mosi, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
        furi_hal_gpio_init(handle->sck, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);

        furi_hal_gpio_write(handle->cs, false);
    } else if(event == FuriHalSpiBusHandleEventDeactivate) {
        furi_hal_gpio_write(handle->cs, true);

        furi_hal_gpio_init(handle->miso, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        furi_hal_gpio_init(handle->mosi, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        furi_hal_gpio_init(handle->sck, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
}

bool swd_spi_bus_trx(
    FuriHalSpiBusHandle* handle,
    uint8_t* tx_buffer,
    uint8_t* rx_buffer,
    size_t size,
    uint32_t timeout) {
    UNUSED(timeout);

    furi_assert(handle);
    furi_assert(size > 0);

    // for(uint8_t i = 0; i < size; i++) {
    //     for(uint8_t bit = 0; bit < 8; bit++) {
    //         furi_hal_gpio_write(handle->sck, false);
    //         furi_hal_gpio_write(handle->mosi, (tx_buffer[i] & (1 << bit)) ? true : false);
    //         furi_hal_gpio_write(handle->sck, true);
    //     }
    // }
    for(uint8_t i = 0; i < size; i++) {
        for(uint8_t mask = 0x80; mask > 0; mask >>= 1) {
            furi_hal_gpio_write(handle->mosi, tx_buffer[i] & mask);
            furi_hal_gpio_write(handle->sck, true);
            if(furi_hal_gpio_read(handle->miso)) rx_buffer[i] |= mask;
            furi_hal_gpio_write(handle->sck, false);
        }
    }

    return true;
}

void swd_spi_acquire(FuriHalSpiBusHandle* handle) {
    furi_assert(handle);

    furi_hal_power_insomnia_enter();

    handle->callback(handle, FuriHalSpiBusHandleEventInit);
    handle->callback(handle, FuriHalSpiBusHandleEventActivate);
}

void swd_spi_release(FuriHalSpiBusHandle* handle) {
    furi_assert(handle);

    // Handle event and unset handle
    handle->callback(handle, FuriHalSpiBusHandleEventDeactivate);
    handle->callback(handle, FuriHalSpiBusHandleEventDeinit);

    furi_hal_power_insomnia_exit();
}