/*\
|*| OneTact.cpp
|*| One Tact Library
|*|
|*| version 1.0
|*| written 2011 - trent m. wyatt
|*| Original writing
|*|
|*| version 2.0
|*| written 2022 - trent m. wyatt
|*| Converted to class library
|*|
|*| edit 2023 - fyrus
\*/

#include <Arduino.h>
#include "OneTact.h"

OneTact::OneTact(const int _pin) :
    state(NONE),
    pin(_pin),
    active(HIGH),
    input_mode(INPUT)
{
    set_button_input();
}

OneTact::OneTact(const int _pin, const int _active) :
    state(NONE),
    pin(_pin),
    active(_active),
    input_mode(INPUT)
{
    set_button_input();
}

OneTact::OneTact(const int _pin, const int _active, const int _input_mode) :
    state(NONE),
    pin(_pin),
    active(_active),
    input_mode(_input_mode)
{
    set_button_input();
}

bool OneTact::set_callback(const uint8_t _state, const ButtonPressCallback _cb) {
    switch (_state) {
        default:        return false;
        case SHORT1:    short1 = _cb;   return true;
        case  LONG1:     long1 = _cb;   return true;
        case SHORT2:    short2 = _cb;   return true;
        case  LONG2:     long2 = _cb;   return true;
        case SHORT3:    short3 = _cb;   return true;
        case  LONG3:     long3 = _cb;   return true;
    }
}

ButtonPressCallback OneTact::callback(const uint8_t _state) const {
    switch (_state) {
        default:        return nullptr;
        case SHORT1:    if (nullptr == short1) { return nullptr; } else { short1(pin, _state);  return short1; }
        case  LONG1:    if (nullptr ==  long1) { return nullptr; } else {  long1(pin, _state);  return  long1; }
        case SHORT2:    if (nullptr == short2) { return nullptr; } else { short2(pin, _state);  return short2; }
        case  LONG2:    if (nullptr ==  long2) { return nullptr; } else {  long2(pin, _state);  return  long2; }
        case SHORT3:    if (nullptr == short3) { return nullptr; } else { short3(pin, _state);  return short3; }
        case  LONG3:    if (nullptr ==  long3) { return nullptr; } else {  long3(pin, _state);  return  long3; }
    }
}

/*\
|*| Set up a specific input pin for use as a push button input.
|*| Note: The input pin will be configured to be an INPUT or an
|*| INPUT_PULLUP depending on the value of input_mode.
\*/
void OneTact::set_button_input() {
  pinMode(pin, input_mode);     // set the button pin as an input with optional pull-up resistor
}


/*\
|*| Get the state of a push button input
|*| Returns: true if the specified button was continuously pressed, otherwise returns false.
|*|
|*| Note: The button must be continuously pressed (no jitter/makes/breaks) for at least as long as KEYDBDELAY
|*|       (key debounce delay). This smooths out the dozens of button connections/disconnections detected at
|*|       the speed of the CPU when the button first begins to make contact into a lower frequency responce.
\*/
bool OneTact::button_pressed() {
    uint32_t presstime = millis() + KEYDBDELAY;
    while (active == digitalRead(pin)) {
        if (millis() >= presstime) {
            return true;
        }
    }

    return false;
}


/*\
|*| See if the user presses a button once, twice, or three times.  Also detect whether the user has held
|*| down the button to indicate a "long" press.  This is an enhanced button press check
|*| (as opposed to button_pressed(...)) in that it attempts to detect gestures.
\*/
uint8_t OneTact::check_button_tact() {
    if (!button_pressed()) {
        return NOT_PRESSED;
    }

    // The button is pressed.
    // Time how long the user presses the button to get the intent/gesture
    uint32_t presstime = millis() + KEYLONGDELAY;  // get the current time in milliseconds plus the long-press offset
    while (button_pressed()) {
        if (millis() >= presstime) {
            return SINGLE_PRESS_LONG;
        }
    }

    // check for double-tap/press
    // The user has released the button, but this might be a double-tap.  Check again and decide.
    presstime = millis() + ALLOWED_MULTIPRESS_DELAY;
    while (millis() < presstime) {
        if (button_pressed()) {
            presstime = millis() + KEYLONGDELAY;
            while (button_pressed()) {
                if (millis() >= presstime) {
                    return DOUBLE_PRESS_LONG;
                }
            }

            // check for triple-tap/press
            // The user has released the button, but this might be a triple-tap.  Check again and decide.
            presstime = millis() + ALLOWED_MULTIPRESS_DELAY;
            while (millis() < presstime) {
                if (button_pressed()) {
                    presstime = millis() + KEYLONGDELAY;
                    while (button_pressed()) {
                        if (millis() >= presstime) {
                            return TRIPLE_PRESS_LONG;
                        }
                    }

                    return TRIPLE_PRESS_SHORT;
                }
            }

            return DOUBLE_PRESS_SHORT;
        }
    }

    return SINGLE_PRESS_SHORT;
}


/*\
|*| This wrapper function is used to allow consistent return values for back-to-back calls of
|*| check_button_internal(...) while the button is continuously pressed.  Without this extra step
|*| DOUBLE_BUTTON_LONG and TRIPLE_BUTTON_LONG presses would return correctly on their first check but
|*| would then be reported as SINGLE_BUTTON_LONG after that if the user kept the button pressed.
|*|
|*| Additionally this function prevents the spurious reporting of a *_BUTTON_SHORT state to be reported
|*| after the user has let go of a button once one or more *_BUTTON_LONG states have been observed.
\*/
uint8_t OneTact::check_button() {
    uint8_t newstate = check_button_tact();
    if (newstate & LONG_PRESS) {
        if (state & LONG_PRESS) {
            newstate = LONG_PRESS | (state & BUTTON_MASK);
            callback(newstate);
            return newstate;
        }
    }
    else {
        if (state & LONG_PRESS) {
            return state = NOT_PRESSED;
        }
    }

    state = newstate;

    if (NONE != state) {
        callback(state);
    }

    return state;
}
