/*\
|*| Example use of the One Tact library
|*|
|*| This example shows how to check the button for any gestures.
|*| If a gesture has been entered it will be displayed to the
|*| serial port.
|*|
\*/

#include <OneTact.h>

// NOTE: change/define the following pin(s) based on your project/connections
#define   BUTTON_PIN    2

//                     Arduino pin     Active HIGH/LOW         Input Mode
OneTact  button(BUTTON_PIN,           LOW,             INPUT_PULLUP);

//
// This function will report the passed button state along with a label
//
void report_button(const uint8_t state, const char* const label = NULL)  {
    switch (state) {
        case SINGLE_PRESS_SHORT: Serial.print(F("Single click and release"));   break;
        case SINGLE_PRESS_LONG:  Serial.print(F("Single click and hold"));      break;
        case DOUBLE_PRESS_SHORT: Serial.print(F("Double click and release"));   break;
        case DOUBLE_PRESS_LONG:  Serial.print(F("Double click and hold"));      break;
        case TRIPLE_PRESS_SHORT: Serial.print(F("Triple click and release"));   break;
        case TRIPLE_PRESS_LONG:  Serial.print(F("Triple click and hold"));      break;

        case NOT_PRESSED:
        default:
            return;
    }

    if (nullptr != label) {
        Serial.print(F(" on "));
        Serial.print(label);
    }

    Serial.println();
}


void setup(void) {
    Serial.begin(115200);
    uint32_t timer = millis() + 2000;
    while (!Serial && millis() < timer);
    Serial.flush();
    Serial.println(F("\n\nArduino Core Library - OneTact Library Test"));
}


void loop(void) {
    report_button(button.check_button(), "push button 1");
}
