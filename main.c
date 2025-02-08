#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

#include "interrupt.h"
#include "oled.h"
#include "leds.h"
#include "numbers0_9.h"

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
static int led = 0;
static char c = '\0';
char *word;
static int charCount = 0;

void default_display();
void Interface_UART_USB();
void leds_control(uint gpio, uint32_t events);

static void config_pin(uint pin, bool outPut, bool pullup) {
  gpio_init(pin);
  gpio_set_dir(pin, outPut);
  if(pullup)gpio_pull_up(pin);
  if(!pullup)gpio_put(pin, 0);
}
// Função Principal++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(){
  stdio_init_all();

  for (int i = 0; i < 3; i++)config_pin(PINS_RGB[i],GPIO_OUT, false);
  config_pin(PIN_BUTTON_A,GPIO_IN, true);
  config_pin(PIN_BUTTON_B,GPIO_IN, true);
  
  oled_Init(I2C_SDA, I2C_SCL);
  ws2812_init(PIN_LEDS, LED_COUNT);
  ColorsGenerate();
  
  oled_Clear(); //limpa o oled
  oled_Draw_Rectangle(0, 0, 128, 64, false); // Desenha um retângulo no na borda do oled
  oled_Draw_draw((uint8_t[]){0x7E,0x81,0xA5,0x81,0xA5,0x99,0x42,0x3C},(128-11),3,8,8);
  oled_Write_String("CEPEDI TIC37", 8, 10);// Desenha uma string no oled
  oled_Write_String("EMBARCATECH", 8, 30);// Desenha uma string no oled
  oled_Write_String("Ligando", 8, 48);
  oled_Update(); // atualiza o oled com novas as informações passadas
  sleep_ms(700);
  for (int i = 0; i < 3; i++){
    if(i==0)oled_Write_String("Ligando.", 8, 48);
    if(i==1)oled_Write_String("Ligando..", 8, 48);
    if(i==2){
      oled_Write_String("Ligando...", 8, 48);
    }
    oled_Update(); // atualiza o oled com novas as informações passadas  
    sleep_ms(700);
  }
  

  ws2812_Map_leds_ON(framesNums0_9[0], colorsFrames[0], framesNums0_9[lenNumFrames-1][0], true);
  default_display();
  itr_SetCallbackFunction(leds_control);
  itr_Interruption(PIN_BUTTON_A);
  itr_Interruption(PIN_BUTTON_B);
  
  while (true){
    sleep_ms(50);
    Interface_UART_USB(PINS_RGB[0], PINS_RGB[1], PINS_RGB[2]);
  }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void default_display(){
  oled_Clear(); //limpa o oled
  oled_Draw_Rectangle(0, 0, 128, 64, false); // Desenha um retângulo no na borda do oled
  oled_Draw_draw((uint8_t[]){0x7E,0x81,0xA5,0x81,0xA5,0x99,0x42,0x3C},(128-11),3,8,8);
  // mostrando o caractere pressionado
  oled_Write_String("Caractere:", 3, 10);// Desenha uma string no oled
  oled_Write_Char(c, 75, 10);// Desenha uma string no oled
  // mostrando palavra formada
  oled_Write_String("Palavra:", 3, 30);
  oled_Write_String(word, 60, 30);
  printf("%s\n", word);
  // mostrando o led selecionado
  oled_Write_String("Leds:",3, 47);
  char *ledOn = led == 1? "verde" : led == 2? "azul" : led == 3? "verde/azul":"desligados";
  oled_Write_String(ledOn, 40, 47);
  oled_Update(); // atualiza o oled com novas as informações passadas
}

void Interface_UART_USB(){
  if (stdio_usb_connected()){ // Certifica-se de que o USB está conectado
    if (scanf(" %c", &c) == 1){ // Lê caractere da entrada padrão
      if (isdigit(c)){
        int i = c - '0'; // Converte char para int
        ws2812_Map_leds_ON(framesNums0_9[i], colorsFrames[i], framesNums0_9[lenNumFrames-1][i], true);
      }
      if(charCount == 8 || charCount == 0){word = malloc(9*sizeof(char)); memset(word, '\0', 9);}
      if(charCount < 8){
        printf("Caractere digitado: %c\n", c);
        word[charCount] = c;
        word[charCount+1] = '\0';
        printf("Palavra formada: %s\n", word);
      }else{
        printf("Reiniciando a palavra: %c\n", c);
      }
      default_display();
      charCount = charCount<8?charCount+1:0;
    }
  }
}
void leds_control(uint gpio, uint32_t events){
  switch(gpio){
    case PIN_BUTTON_A:
    gpio_put(PINS_RGB[1], !gpio_get(PINS_RGB[1]));
    break;
    case PIN_BUTTON_B:
        gpio_put(PINS_RGB[2], !gpio_get(PINS_RGB[2]));
    break;
    default:
    led == 0;
    break;
  }
  if(gpio_get(PINS_RGB[1]) && gpio_get(PINS_RGB[2])){
    printf("leds verde e azul ligados!\n");
    led = 3;
  }else if(gpio_get(PINS_RGB[1]) && !gpio_get(PINS_RGB[2])){
    printf("led verde ligado!\n");
    led = 1;
  }else if(!gpio_get(PINS_RGB[1]) && gpio_get(PINS_RGB[2])){
    printf("led azul ligado!\n");
    led = 2;
  }else{
    printf("leds desligados!\n");
    led = 0;
  }
  default_display();
}