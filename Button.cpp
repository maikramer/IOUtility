#include "Button.h"

Button::Button(GPIO_TypeDef *port, uint16_t pin) :
		m_buttonPort(port), m_buttonPin(pin), m_state(ButtonState::BUTTON_IDLE), m_lastStateChange(0), m_longPressTimeThreshold(
				1000), m_doubleClickTimeThreshold(400), m_clickCount(0), m_lastClickTime(0), m_debounceDelay(50), m_lastDebounceTime(
				0), m_lastButtonState(false), m_stateChangedFlag(false) {
	// Initialize GPIO pin
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = m_buttonPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(m_buttonPort, &GPIO_InitStruct);
}

void Button::update() {
	uint32_t currentTime = HAL_GetTick();
	bool buttonState = HAL_GPIO_ReadPin(m_buttonPort, m_buttonPin);

	if ((currentTime - m_lastDebounceTime) < m_debounceDelay) {
		return;
	}

	ButtonState prevState = m_state;

	switch (m_state) {
	case ButtonState::BUTTON_IDLE:
		if (!buttonState) {
			m_state = ButtonState::BUTTON_DOWN;
			m_lastStateChange = currentTime;
		}
		break;

	case ButtonState::BUTTON_DOWN:
		if (buttonState) {
			m_state = ButtonState::BUTTON_UP;
			m_lastStateChange = currentTime;
			if ((currentTime - m_lastClickTime) < m_doubleClickTimeThreshold) {
				m_clickCount++;
				if (m_clickCount == 2) {
					m_state = ButtonState::BUTTON_DOUBLE_CLICK;
					m_clickCount = 0;
				}
			} else {
				m_clickCount = 1;
			}
			m_lastClickTime = currentTime;
		} else if ((currentTime - m_lastStateChange) > m_longPressTimeThreshold) {
			m_state = ButtonState::BUTTON_LONG_PRESS;
		} else if ((currentTime - m_lastClickTime) > m_doubleClickTimeThreshold) {
			m_clickCount = 0; // Reset click count if time since last click exceeds double-click threshold
		}
		break;

	case ButtonState::BUTTON_UP:
		if (!buttonState) {
			m_state = ButtonState::BUTTON_DOWN;
			m_lastStateChange = currentTime;
		} else {
			if ((currentTime - m_lastStateChange) > m_doubleClickTimeThreshold) {
				m_state = ButtonState::BUTTON_CLICK;
			}
		}
		break;

	case ButtonState::BUTTON_LONG_PRESS:
		if (buttonState) {
			m_state = ButtonState::BUTTON_IDLE;
		}
		break;

	case ButtonState::BUTTON_DOUBLE_CLICK:
		if (buttonState) {
			m_state = ButtonState::BUTTON_IDLE;
		}
		break;

	case ButtonState::BUTTON_CLICK:
		if (buttonState) {
			m_state = ButtonState::BUTTON_IDLE;
		}
		break;
	}

	if (buttonState != m_lastButtonState) {
		m_lastDebounceTime = currentTime;
	}

	if (m_state != prevState) {
		for (const auto &listener : m_listeners) {
			if (listener.first == m_state) {
				listener.second();
			}
		}
	}

	if (m_state != prevState) {
		m_lastStateChange = currentTime;
		m_stateChangedFlag = true;
		if (m_stateChangedCallback) {
			m_stateChangedCallback();
		}
	} else {
		m_stateChangedFlag = false;
	}

	m_lastButtonState = buttonState;
}

void Button::addListener(ButtonState state, std::function<void()> callback) {
	m_listeners.push_back( { state, callback });
}

uint8_t Button::getClickCount() const {
	return m_clickCount;
}

bool Button::getLastButtonState() const {
	return m_lastButtonState;
}

ButtonState Button::getCurrentState() const {
	return m_state;
}

bool Button::stateChanged() {
	return m_stateChangedFlag;
}

void Button::addStateChangedListener(std::function<void()> callback) {
	m_stateChangedCallback = callback;
}
