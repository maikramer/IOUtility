/*
 * FilteredInput.h
 *
 *  Created on: Jul 10, 2021
 *      Author: maikeu
 */

#ifndef INC_FILTEREDINPUT_H_
#define INC_FILTEREDINPUT_H_

#include <functional>
#include "stm32f0xx_hal.h"

class FilteredInput {
public:
	explicit FilteredInput(std::function<GPIO_PinState()> readFunction,
			uint16_t debounce_ms) {
		_readFunction = readFunction;
		_debounceTime = debounce_ms;
		_debounce = _debounceTime;
	}

	bool IsFiltered() {
		Update();
		return (_filtered);
	}

	GPIO_PinState GetValue() {
		Update();
		return (_actualInput);
	}

	bool Changed() {
		Update();
		bool ret = false;
		if (_lastChangedCall != _actualInput) {
			ret = true;
		}
		_lastChangedCall = _actualInput;
		return ret;
	}

	void Update();

private:

	std::function<GPIO_PinState()> _readFunction;
	bool _filtered = false;
	int32_t _debounce = 0;
	uint32_t _debounceTime = 0;
	GPIO_PinState _actualInput = GPIO_PIN_RESET;
	GPIO_PinState _lastInput = GPIO_PIN_RESET;
	GPIO_PinState _lastChangedCall = GPIO_PIN_RESET;
	uint32_t _lastRead = 0;
};

#endif /* INC_FILTEREDINPUT_H_ */
