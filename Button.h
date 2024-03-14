#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f0xx_hal.h"
#include <functional>
#include <vector>

/**
 * @brief Enum representing different states of the button.
 */
enum class ButtonState {
	BUTTON_IDLE, /**< Button is idle. */
	BUTTON_DOWN, /**< Button is pressed down. */
	BUTTON_UP, /**< Button is released. */
	BUTTON_LONG_PRESS, /**< Button is pressed for a long duration. */
	BUTTON_DOUBLE_CLICK, /**< Button is double-clicked. */
	BUTTON_CLICK /**< Button is single-clicked. */
};

/**
 * @brief Class to handle button input and events.
 */
class Button {
public:
	/**
	 * @brief Constructor to initialize the Button instance.
	 *
	 * @param port GPIO port of the button pin.
	 * @param pin GPIO pin of the button.
	 */
	Button(GPIO_TypeDef *port, uint16_t pin);

	/**
	 * @brief Update method to be called periodically to update the button state.
	 *
	 * This method should be called periodically, typically within the main loop, to update the state of the button based on its input.
	 * It performs debouncing, detects button presses, releases, long presses, double clicks, and single clicks, and triggers the appropriate event callbacks.
	 * The update process considers the configured debounce delay, long press threshold, and double click threshold.
	 *
	 * When a button event is detected, such as a press, release, long press, double click, or single click, the corresponding event callback registered with addListener() will be triggered.
	 *
	 * @note This method should be called frequently enough to accurately detect button events.
	 */
	void update();

	/**
	 * @brief Add a listener for a specific button state.
	 *
	 * @param state The button state to listen for.
	 * @param callback The callback function to be called when the specified state is detected.
	 */
	void addListener(ButtonState state, std::function<void()> callback);

	/**
	 * @brief Get the number of consecutive clicks.
	 *
	 * @return The number of consecutive clicks.
	 */
	uint8_t getClickCount() const;

	/**
	 * @brief Get the last recorded state of the button.
	 *
	 * @return The last recorded state of the button.
	 */
	bool getLastButtonState() const;

	/**
	 * @brief Get the current state of the button.
	 *
	 * @return The current state of the button.
	 */
	ButtonState getCurrentState() const;

	/**
	 * @brief Check if the state has changed since the last call of stateChanged().
	 *
	 * @return True if the state has changed, false otherwise.
	 */
	bool stateChanged();

	/**
	 * @brief Add a listener for state changes.
	 *
	 * @param callback The callback function to be called when the button state changes.
	 */
	void addStateChangedListener(std::function<void()> callback);

private:
	GPIO_TypeDef *m_buttonPort; /**< GPIO port of the button pin. */
	uint16_t m_buttonPin; /**< GPIO pin of the button. */
	ButtonState m_state; /**< Current state of the button. */
	uint32_t m_lastStateChange; /**< Timestamp of the last state change. */
	uint32_t m_longPressTimeThreshold; /**< Time threshold for detecting a long press. */
	uint32_t m_doubleClickTimeThreshold; /**< Time threshold for detecting a double click. */
	uint8_t m_clickCount; /**< Number of consecutive clicks. */
	uint32_t m_lastClickTime; /**< Timestamp of the last click. */
	uint32_t m_debounceDelay; /**< Debounce delay for the button input. */
	uint32_t m_lastDebounceTime; /**< Timestamp of the last debounce. */
	bool m_lastButtonState; /**< Last recorded state of the button. */
	std::vector<std::pair<ButtonState, std::function<void()>>> m_listeners; /**< List of event listeners. */
	std::function<void()> m_stateChangedCallback; /**< Callback for state changes. */
	bool m_stateChangedFlag; /**< Flag indicating if the state has changed. */
};

#endif // BUTTON_H
