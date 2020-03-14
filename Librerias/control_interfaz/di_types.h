#ifndef UI_TYPES
#define UI_TYPES

#include "control_interfaz.h"
#include <Arduino.h>
#include <stdlib.h>

// -------- CLASES --------

#define TIME_TO_DIE 10
class DisplayInfo {
  	public:
	DisplayInfo(const int digits, int offset);

    volatile int cursor; // entre 0 y digits-1
    volatile char* display;
    volatile int* time_to_die;
  	const int length;
    const int offset;
};

DisplayInfo::DisplayInfo(int digits, int offset) 
: cursor(0), display(0), time_to_die(0), length(digits), offset(offset)
{
	display = new char[digits];
  	time_to_die = new int[digits];
  
  	for (int i = 0; i < digits; i++) {
    	display[i] = 0;
    	time_to_die[i] = TIME_TO_DIE;
    }
}

static void di_top_button_handler(DisplayInfo* di);
static void di_bottom_button_handler(DisplayInfo* di);
static void di_left_button_handler(DisplayInfo* di);
static void di_right_button_handler(DisplayInfo* di);

class Button {
	public:
  	bool is_pressed();
  
  	Button(const unsigned char pin, const ButtonType type, DisplayInfo* di);
  	const unsigned char pin;
  	const ButtonType type;
  	DisplayInfo* display_info;
    void (*handler)(DisplayInfo* di);
};

Button::Button(const unsigned char pin, const ButtonType type, DisplayInfo* di)
: pin(pin), type(type), handler(0),  display_info(di)
{
  	int display_length = display_info->length;
    switch (type)
    {
    case UI_UP_BUTTON: {
        handler = di_top_button_handler;
        break;
    }
    
    case UI_RIGHT_BUTTON: {
		handler = di_right_button_handler;
        break;
    }
    case UI_BOTTOM_BUTTON: {
        handler = di_bottom_button_handler;
        break;
    }

    case UI_LEFT_BUTTON: {
        handler = di_left_button_handler;
        break;
    }

    default:
        break;
    }
}
bool Button::is_pressed()	
{
	return (digitalRead(pin) == LOW);
}

// ------ BUTTON HANDLERS -----------

// README: EL di_top_button_handler puede que requiera modificacion, al igual
// que el di_bottom_button_handler
#define GET_MIN_STEP(cursor) 1
#define CANT_INCREMENT(di) \
    (di->display[di->cursor] == 0x09)
#define CANT_DECREMENT(di) \
    (di->display[di->cursor] == 0x00)


#define CURSOR_CAN_MOVE_RIGHT(di) (di->cursor > 0)
#define CURSOR_CAN_MOVE_LEFT(di)  (di->cursor < (di->length - 1) ) 
static void di_right_button_handler(DisplayInfo* di) {
    if (CURSOR_CAN_MOVE_RIGHT(di))
        di->cursor -= 1;
    for (int i = 0; i < di->length; i ++)
    	di->time_to_die[i] = 10;
}
static void di_left_button_handler(DisplayInfo* di) {
    if (CURSOR_CAN_MOVE_LEFT(di))
        di->cursor += 1;
    for (int i = 0; i < di->length; i ++)
    	di->time_to_die[i] = 10;
}

static void di_top_button_handler(DisplayInfo* di) {
    if(CANT_INCREMENT(di)) {}
	// si pasa de 9 que pase a 0 y el digito de la izquierda aumente en 1
    else {
        char step = GET_MIN_STEP(di->cursor);
        di->display[di->cursor] = (di->display[di->cursor] + step)%10;
    }
}
static void di_bottom_button_handler(DisplayInfo* di) {
    if(CANT_DECREMENT(di)) {}
    // homologo
    else {
        char step = GET_MIN_STEP(di->cursor);
        di->display[di->cursor] = (di->display[di->cursor] - step)%10;
    }
}

// -------- Buttons --------------

#define for_each_button(current_button, button_length) \
    for(current_button = 0; current_button < button_length; current_button++)

#endif