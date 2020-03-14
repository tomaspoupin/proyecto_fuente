#include <control_interfaz.h>

const unsigned char interrupt_pin = 3;
const unsigned char voltage_button_pins[4] = {4, 6, 7, 8};
const unsigned char current_button_pins[4] = {0, 1, 2, 9};
const unsigned char d_in = 11;
const unsigned char clk = 13;
const unsigned char cs = 10;

void setup() {
    ui_init_display(d_in, clk, cs);
    ui_init_buttons(voltage_button_pins, 4);
    ui_init_buttons(current_button_pins, 4);
    ui_map_voltage_button(voltage_button_pins[0], UI_UP_BUTTON);
    ui_map_voltage_button(voltage_button_pins[1], UI_RIGHT_BUTTON);
    ui_map_voltage_button(voltage_button_pins[2], UI_BOTTOM_BUTTON);
    ui_map_voltage_button(voltage_button_pins[3], UI_LEFT_BUTTON);
    ui_map_current_button(current_button_pins[0], UI_UP_BUTTON);
    ui_map_current_button(current_button_pins[1], UI_RIGHT_BUTTON);
    ui_map_current_button(current_button_pins[2], UI_BOTTOM_BUTTON);
    ui_map_current_button(current_button_pins[3], UI_LEFT_BUTTON);

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(interrupt_pin), ui_interrupt_handler, FALLING);
}

void loop() {
}
