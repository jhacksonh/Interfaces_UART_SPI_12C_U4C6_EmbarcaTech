#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

#include "interrupt.h"
#include "oled.h"
#include "leds.h"
#include "numbers0_9.h"
// #include "font.h"



#define I2C_PORT i2c1
#define endereco 0x3C
#define I2C_SDA 14
#define I2C_SCL 15
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#define PIN_LEDS 7
#define LED_COUNT 25

static uint PINS_RGB[3] = {13,11,12};
static uint32_t LAST_TIME = 0;
static int index = 0;
static void config_pin(uint pin, bool outPut, bool pullup) {
  gpio_init(pin);
  gpio_set_dir(pin, outPut);
  if(pullup)gpio_pull_up(pin);
  if(!pullup)gpio_put(pin, 0);
}

void Interface_UART_USB(){
  char c;
  if (stdio_usb_connected()){ // Certifica-se de que o USB está conectado
    if (scanf("%c", &c) == 1){ // Lê caractere da entrada padrão
        printf("Recebido: '%c'\n", c);
        //DETERMINAR O QUE FAZER AO RECEBER O CARACTERE
        switch(c){
          case 'r':
            index = 0;
          break;
          case 'g':
            index = 1;
          break;
          case 'b':
            index = 2;
          break;
          default:
            index = 3;
        }
        for (int i = 0; i < 3; i++)gpio_put(PINS_RGB[i], i==index);
    }
  }
}

void mudar_cor(uint gpio, uint32_t events){
  switch(gpio){
    case PIN_BUTTON_A:
    printf("A pressionada\n");
        index++;
    break;
    case PIN_BUTTON_B:
    printf("B pressionada\n");
        index--;
    break;
  }
  printf("index: GPIO%d%d\n", index,gpio);
  index = index == 3 ? 0 : index == -1 ? 2 : index; 
  for (int i = 0; i < 3; i++)gpio_put(PINS_RGB[i], i==index);
}

int main(){
  stdio_init_all();

  for (int i = 0; i < 3; i++)config_pin(PINS_RGB[i],GPIO_OUT, false);
  for (int i = 0; i < 3; i++)gpio_put(PINS_RGB[i], 1);
  config_pin(PIN_BUTTON_A,GPIO_IN, true);
  config_pin(PIN_BUTTON_B,GPIO_IN, true);


  itr_SetCallbackFunction(mudar_cor);
  itr_Interruption(PIN_BUTTON_A);
  itr_Interruption(PIN_BUTTON_B);

  oled_Init(I2C_SDA, I2C_SCL);
  ws2812_init(PIN_LEDS, LED_COUNT);
  ColorsGenerate();
  int in_leds = 0;

  while (true){
    oled_Clear(); //limpa o oled
    oled_Draw_Rectangle(0, 0, 126, 62, false); // Desenha um retângulo no na borda do oled
    oled_Write_String("CEPEDI L TIC37", 8, 10);// Desenha uma string no oled
    oled_Write_String("EMBARCATECH", 20, 30);// Desenha uma string no oled
    oled_Write_String("JHACKSONH", 15, 48);// Desenha uma string no oled
    oled_Update(); // atualiza o oled com novas as informações passadas
    ws2812_Map_leds_ON(framesNums0_9[in_leds], colorsFrames[in_leds], framesNums0_9[lenNumFrames-1][in_leds], true);
    in_leds = in_leds == 9 ? 0 : in_leds+1;
    sleep_ms(500);
    // Interface_UART_USB(PINS_RGB[0], PINS_RGB[1], PINS_RGB[2]);
  }
}