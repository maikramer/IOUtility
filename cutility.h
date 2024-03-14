/*
 * cutiity.h
 *
 *  Created on: Mar 9, 2024
 *      Author: maikeu
 */

#ifndef CUTILITY_H_
#define CUTILITY_H_

#include <stdint.h> // For standard integer types
#include "stm32f0xx_hal.h"// Function type for the action you want to debounce
#include "stdbool.h"

typedef void (*DebouncedAction)();

void debounce(uint32_t delay_ms, DebouncedAction action, int ch) {
    static uint32_t last_trigger_time[8] = {0};
    uint32_t current_time = HAL_GetTick(); // Or your equivalent time function

    if (current_time - last_trigger_time[ch] > delay_ms) {
        action(); // Execute the debounced action
        last_trigger_time[ch] = current_time;
    }
}


#endif /* CUTILITY_H_ */
