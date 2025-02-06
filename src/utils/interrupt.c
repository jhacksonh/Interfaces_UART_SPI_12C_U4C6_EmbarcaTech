#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "interrupt.h"

static uint32_t LAST_TIME = 0;
// Definindo a estrutura que contém ponteiros para funções
typedef struct{
    void (*func)(uint, uint32_t);  // Ponteiro para uma função que recebe dois inteiros e retorna um inteiro
} CallbackFunction;
CallbackFunction callback_function;

static bool itr_Debounce(){
  uint32_t current_time = to_us_since_boot(get_absolute_time());
  bool valid_time = current_time - LAST_TIME > 200000; 
  if(valid_time)LAST_TIME = current_time;
  return valid_time;
}
static void itr_Button_Callback(uint gpio, uint32_t events) {
  static int index = 0;
  if(itr_Debounce()){
    callback_function.func(gpio, events);
  }
}
void itr_SetCallbackFunction(void (*func)(uint, uint32_t)){
    callback_function.func = func;
}
void itr_Interruption(uint pin){
  gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &itr_Button_Callback);
}