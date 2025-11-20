#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/dm.h"

int DMselected = 0;

void DMscreen() {
    NewRectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    NewText("DM", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    
    
}

void DMreset() {
    DMselected = 0;
}