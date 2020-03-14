#include "di_types.h"
#include <LedControl.h>
#include <TimerOne.h>

#define MIN_INTERRUPT_TIME_INTERVAL 200
#define BUTTON_NUMBER 8
#define VOLTAGE_LENGTH 3
#define CURRENT_LENGTH 2
#define VOLTAGE_OFFSET 5
#define CURRENT_OFFSET 0

static DisplayInfo voltage_display_info(VOLTAGE_LENGTH, VOLTAGE_OFFSET);
static DisplayInfo current_display_info(CURRENT_LENGTH, CURRENT_OFFSET);
static Button* buttons[BUTTON_NUMBER];
static int empty_button_index = 0;
static LedControl* p_display;
static DisplayInfo* active_display;

static void update_display(DisplayInfo* di) {
    for (int i = 0; i < di->length; i++) {
        p_display->setDigit(0, i + di->offset, di->display[i], i == 1);
    }
}


static void blink_callback() {

    static const char space = ' ';
    update_display(&voltage_display_info);
    update_display(&current_display_info);

    if (active_display->time_to_die[active_display->cursor] > 0) {
        if (active_display->time_to_die[active_display->cursor] % 2 == 0)
            p_display->setDigit(0, 
                                active_display->cursor + active_display->offset, 
                                active_display->display[active_display->cursor], 
                                active_display->cursor == 1);
        else
            p_display->setChar(0, 
                               active_display->cursor + active_display->offset,
                               space,
                               active_display->cursor == 1);
        
        active_display->time_to_die[active_display->cursor] -= 1;
    }
}




// ------------------ Definicion Interfaz UI ------------------------
void ui_init_display(const unsigned char data_in, const unsigned char clk, const unsigned char load_cs) {
    static LedControl display = LedControl(data_in, clk, load_cs, 1);
    display.shutdown(0, false);
    display.setIntensity(0, 8);
    display.clearDisplay(0);

    p_display = &display;

    Timer1.initialize(100000);
    Timer1.attachInterrupt(blink_callback);
}

void ui_init_buttons(const unsigned char* button_pins, const int button_number) {
    int current_button;
    for_each_button(current_button, button_number) {
        pinMode(current_button, INPUT);
    }
}

void ui_map_voltage_button(const unsigned char button_pin, const ButtonType button_type) {
    Button* new_button = new Button(button_pin, button_type, &voltage_display_info);
    buttons[empty_button_index] = new_button;
    empty_button_index += 1;
}

void ui_map_current_button(const unsigned char button_pin, const ButtonType button_type) {
    Button* new_button = new Button(button_pin, button_type, &current_display_info);
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
    //Serial.println("boton presionado");
    // Comienza zona de interrupcion
    Button* active_button = get_pressed_button();
/*     Serial.print("Boton ");
    Serial.print(active_button->pin);
    Serial.println(" Presionado."); */
    active_button->handler(active_button->display_info);
    update_display(active_button->display_info);
    active_display = active_button->display_info;

    float voltaje = voltage_display_info.display[0]*0.1f \
                  + voltage_display_info.display[1]*1.0f \
                  + voltage_display_info.display[2]*10.0f;
    float corriente = current_display_info.display[0]*0.1f \
                    + current_display_info.display[1]*1.0f;
    Serial.print("Voltaje: ");
    Serial.println(voltaje);
    Serial.print("Corriente: ");
    Serial.println(corriente);
    start_time = millis();
    interrupts();
}
#define BUTTON_NOT_FOUND 1
Button* get_pressed_button() {
    int current_button = 0;
    while(BUTTON_NOT_FOUND) {
        if (buttons[current_button]->is_pressed()) {
            /* logging 
            Serial.println("Button pressed:");
            Serial.println(buttons[current_button].pin);
            Serial.println(" "); */
            return buttons[current_button]; 
        }
        current_button += 1;
        if (current_button == BUTTON_NUMBER)
            current_button = 0;
    }
}