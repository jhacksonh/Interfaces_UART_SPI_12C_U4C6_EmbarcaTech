#ifndef OLED_H
#define OLED_H
#include <pico/stdlib.h>

void oled_Init(uint pin_i2c_sda, uint pin_i2c_scl);
void oled_Write_Char(char c, uint8_t x, uint8_t y);
void oled_Write_String(const char *str, uint8_t x, uint8_t y);
void oled_Draw_Rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill);
void oled_Update();
void oled_Clear();

#endif
