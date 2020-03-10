#ifndef UI_TYPES
#define UI_TYPES

#include "control_interfaz.h"
#include <Arduino.h>

// ------ ESTRUCTURAS ------------
typedef struct {
    volatile int cursor; // entre 0 y 3
    volatile char display[4];
    volatile int time_to_die[4];
} DisplayInfo;

typedef struct {
    unsigned char pin;
    ButtonType type;
    void (*handler)(DisplayInfo* di);
} Button;

// ------ BUTTON HANDLERS -----------

// README: EL di_top_button_handler puede que requiera modificacion, al igual
// que el di_bottom_button_handler
#define GET_MIN_STEP(cursor) 1
#define CANT_INCREMENT(di) \
    (di->display[di->cursor] == 0x09)
#define CANT_DECREMENT(di) \
    (di->display[di->cursor] == 0x00)

// #define CURSOR_CAN_CHANGE(di) (di->cursor < 3 && di->cursor >= 0)
#define CURSOR_CAN_MOVE_RIGHT(di) (di->cursor > 0)
#define CURSOR_CAN_MOVE_LEFT(di)  (di->cursor < 3)
static inline void di_right_button_handler(DisplayInfo* di) {
    if (CURSOR_CAN_MOVE_RIGHT(di))
        di->cursor -= 1;
}
static inline void di_left_button_handler(DisplayInfo* di) {
    if (CURSOR_CAN_MOVE_LEFT(di))
        di->cursor += 1;
}

static void di_top_button_handler(DisplayInfo* di) {
    if(CANT_INCREMENT(di)) {}

    else {
        char step = GET_MIN_STEP(di->cursor);
        di->display[di->cursor] = (di->display[di->cursor] + step)%10;
    }
}
static void di_bottom_button_handler(DisplayInfo* di) {
    if(CANT_DECREMENT(di)) {}
    
    else {
        char step = GET_MIN_STEP(di->cursor);
        di->display[di->cursor] = (di->display[di->cursor] - step)%10;
    }
}

// -------- DisplayInfo ----------------

#define INIT_DISPLAY_INFO(display_info) \
    display_info = {0, {0, 0, 0, 0}, {10, 10, 10, 10}}

// -------- Buttons --------------
static inline void set_button_pin(Button* button, const unsigned char button_pin) {
    button->pin = button_pin;
}

static void set_button_handler(Button* button, const ButtonType type) {
    button->type = type;
    switch (type)
    {
    case UI_UP_BUTTON:
        button->handler = di_top_button_handler;
        break;
    
    case UI_RIGHT_BUTTON:
        button->handler = di_right_button_handler;
        break;

    case UI_BOTTOM_BUTTON:
        button->handler = di_bottom_button_handler;
        break;

    case UI_LEFT_BUTTON:
        button->handler = di_left_button_handler;
        break;
    default:
        break;
    }
}

static inline bool is_button_high(Button* button) {
    return (digitalRead(button->pin) == HIGH);
}

static inline bool is_button_low(Button* button) {
    return (digitalRead(button->pin) == LOW);
}

#define for_each_button(current_button, button_length) \
    for(current_button = 0; current_button < button_length; current_button++)

#endif