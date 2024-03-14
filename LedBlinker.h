//
// Created by maikeu on 22/10/22.
//

#ifndef PROJETO_BETA_LEDBLINKER_H
#define PROJETO_BETA_LEDBLINKER_H
#include "stm32f0xx_hal.h"

class LedBlinker {
public:
    LedBlinker(int interval_ms, GPIO_TypeDef * gpio, uint16_t pin) {
        LastBlink = HAL_GetTick();
        Interval = interval_ms;
        Gpio = gpio;
        Pin = pin;
    }

    void update() {
        auto ticks = HAL_GetTick();
        if (ticks - LastBlink > Interval || LastBlink > ticks) {
			HAL_GPIO_TogglePin(Gpio, Pin);
            LastBlink = ticks;
        }
    }

    uint32_t LastBlink;
    uint32_t Interval;
    GPIO_TypeDef * Gpio;
    uint16_t Pin;

};


#endif //PROJETO_BETA_LEDBLINKER_H
