//
// Created by maikeu on 16/11/2021.
//

#include "FilteredInput.h"

void FilteredInput::Update() {
	auto value = _readFunction();
	if (value == _lastInput) {
		if (!_filtered) {
			uint32_t now = HAL_GetTick();
			_debounce -= (now - _lastRead);
			_filtered = _debounce <= 0;
			_lastRead = now;
			if (_filtered) {
				_actualInput = value;
			}
		}
	} else {
		_filtered = false;
		_debounce = _debounceTime;
		_lastRead = HAL_GetTick();
	}
	_lastInput = value;
}
