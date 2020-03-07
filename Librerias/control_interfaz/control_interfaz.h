#ifndef CTRL_INTERFAZ
#define CTRL_INTERFAZ

enum ButtonType {
    UI_UP_BUTTON = 0,
    UI_RIGHT_BUTTON = 1,
    UI_BOTTOM_BUTTON = 2,
    UI_LEFT_BUTTON = 3
};

void ui_init_buttons(const unsigned char* button_pins, const int button_number);
void ui_map_button(const unsigned char button_pin, const ButtonType type);
void ui_interrupt_handler();
void ui_init_display(const unsigned char data_in, const unsigned char clk, const unsigned char load_cs);

#endif