#include <control_interfaz.h>

const unsigned char interrupt_pin = 2;
const unsigned char button_pins[4] = {3, 4, 5, 6};
const unsigned char d_in = 12;
const unsigned char clk = 11;
const unsigned char cs = 10;

void setup() {
    ui_init_display(d_in, clk, cs);
    ui_init_buttons(button_pins, 4);
    ui_map_button(button_pins[0], UI_UP_BUTTON);
    ui_map_button(button_pins[1], UI_RIGHT_BUTTON);
    ui_map_button(button_pins[2], UI_BOTTOM_BUTTON);
    ui_map_button(button_pins[3], UI_LEFT_BUTTON);

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(interrupt_pin), ui_interrupt_handler, RISING);
}

void loop() {
}
