#include "di_types.h"
#include <LedControl.h>
#include <TimerOne.h>

#define MIN_INTERRUPT_TIME_INTERVAL 200
#define BUTTON_NUMBER 4

static DisplayInfo display_info;
static Button buttons[BUTTON_NUMBER];
static int empty_button_index = 0;
static LedControl* p_display;

static void update_display(DisplayInfo* di) {
  p_display->setDigit(0, 3, di->display[3], false);
  p_display->setDigit(0, 2, di->display[2], true);
  p_display->setDigit(0, 1, di->display[1], false);
  p_display->setDigit(0, 0, di->display[0], false);
}

static void start_blink() {
    Timer1.attachInterrupt(blink_callback);
    Timer1.start();
}
static void stop_blink() {
    Timer1.detachInterrupt();
    Timer1.stop();
}
static void blink_callback() {
    static int cursor = display_info.cursor;
    static bool digit_state = 0;
    static const int blink_amount = 5;
    static int counter = 0;

    if (display_info.cursor != cursor) {
        cursor = display_info.cursor;
        digit_state = 0;
        cursor = display_info.cursor;
    }
    if (counter == blink_amount*2)
    {
        counter = 0;
        stop_blink();
    }

    if (digit_state == 0) 
        p_display->setChar(0,
                            display_info.cursor,
                            " ",
                            display_info.cursor == 2);
    else
        p_display->setDigit(0,
                            display_info.cursor, 
                            display_info.display[display_info.cursor],
                            display_info.cursor == 2);
    digit_state = !digit_state;
    counter++;
}

// ------------------ Definicion Interfaz UI ------------------------
void ui_init_display(const unsigned char data_in, const unsigned char clk, const unsigned char load_cs) {
    INIT_DISPLAY_INFO(display_info);
    static LedControl display = LedControl(data_in, clk, load_cs, 1);
    display.shutdown(0, false);
    display.setIntensity(0, 8);
    display.clearDisplay(0);

    p_display = &display;
    p_display->setDigit(0, 3, 0x00, false);
    p_display->setDigit(0, 2, 0x00, false);
    p_display->setDigit(0, 1, 0x00, true);
    p_display->setDigit(0, 0, 0x00, false);

    Timer1.initialize(200000);
    Timer1.stop();
}

void ui_init_buttons(const unsigned char* button_pins, const int button_number) {
    int current_button;
    for_each_button(current_button, button_number) {
        pinMode(current_button, INPUT);
    }
}

void ui_map_button(const unsigned char button_pin, const ButtonType button_type) {
    Button new_button;
    set_button_pin(&new_button, button_pin);
    set_button_handler(&new_button, button_type);
    buttons[empty_button_index] = new_button;
    empty_button_index += 1;
}

Button* get_pressed_button();
void ui_interrupt_handler() {
    noInterrupts();
    static int start_time = 0;
    int interrupt_time = millis();
    if ((interrupt_time - start_time) < MIN_INTERRUPT_TIME_INTERVAL) 
        return;

    // Comienza zona de interrupcion
    Button* active_button = get_pressed_button();
    active_button->handler(&display_info);
    update_display(&display_info);
    start_blink();

    start_time = millis();
    interrupts();
}
#define BUTTON_NOT_FOUND 1
Button* get_pressed_button() {
    int current_button = 0;
    while(BUTTON_NOT_FOUND) {
        if (is_button_low(&buttons[current_button])) {
            // logging
            Serial.println("Button pressed:");
            Serial.println(buttons[current_button].pin);
            Serial.println(" ");
            return &buttons[current_button];
        }
        current_button += 1;
        if (current_button == BUTTON_NUMBER)
            current_button = 0;
    }
}