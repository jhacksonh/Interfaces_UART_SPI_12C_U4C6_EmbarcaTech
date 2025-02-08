#include <stdio.h>  // Inclui a biblioteca padrão de entrada e saída
#include <stdlib.h> // Inclui a biblioteca padrão para funções como malloc e free
#include <string.h> // Inclui a biblioteca para manipulação de strings
#include <ctype.h>  // Inclui a biblioteca para funções de manipulação de caracteres

#include "pico/stdlib.h" // Inclui a biblioteca padrão do Raspberry Pi Pico
#include "hardware/timer.h" // Inclui a biblioteca para manipulação de timers
#include "hardware/i2c.h" // Inclui a biblioteca para comunicação I2C

#include "interrupt.h" // Inclui o cabeçalho para funções de interrupção
#include "oled.h" // Inclui o cabeçalho para funções do display OLED
#include "leds.h" // Inclui o cabeçalho para funções de controle dos LEDs
#include "numbers0_9.h" // Inclui o cabeçalho para os números de 0 a 9

#define I2C_SDA 14 // Define o pino SDA para comunicação I2C
#define I2C_SCL 15 // Define o pino SCL para comunicação I2C
#define PIN_BUTTON_A 5 // Define o pino para o botão A
#define PIN_BUTTON_B 6 // Define o pino para o botão B
#define PIN_LEDS 7 // Define o pino para os LEDs
#define LED_COUNT 25 // Define o número de LEDs

static uint PINS_RGB[3] = {13,11,12}; // Define os pinos para os LEDs RGB
static int led = 0; // Variável para controlar o estado dos LEDs
static char c = '\0'; // Variável para armazenar o caractere lido
char *word; // Ponteiro para armazenar a palavra formada
static int charCount = 0; // Contador de caracteres na palavra

void Initialization(); // Declaração da função de inicialização
void default_display(); // Declaração da função para exibir o estado padrão no OLED
void Interface_UART_USB(); // Declaração da função para interface UART/USB
void leds_control(uint gpio, uint32_t events); // Declaração da função para controle dos LEDs

static void config_pin(uint pin, bool outPut, bool pullup) {
  gpio_init(pin); // Inicializa o pino
  gpio_set_dir(pin, outPut); // Configura a direção do pino (entrada ou saída)
  if(pullup)gpio_pull_up(pin); // Habilita o resistor de pull-up se necessário
  if(!pullup)gpio_put(pin, 0); // Define o pino como LOW se não for pull-up
}
// Função Principal++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(){
  stdio_init_all(); // Inicializa todas as funcionalidades de I/O

  for (int i = 0; i < 3; i++)config_pin(PINS_RGB[i],GPIO_OUT, false); // Configura os pinos RGB como saída
  config_pin(PIN_BUTTON_A,GPIO_IN, true); // Configura o pino do botão A como entrada com pull-up
  config_pin(PIN_BUTTON_B,GPIO_IN, true); // Configura o pino do botão B como entrada com pull-up
  
  oled_Init(I2C_SDA, I2C_SCL); // Inicializa o display OLED
  ws2812_init(PIN_LEDS, LED_COUNT); // Inicializa os LEDs WS2812
  ColorsGenerate(); // Gera as cores para os LEDs
  
  Initialization(); // Chama a função de inicialização

  ws2812_Map_leds_ON(framesNums0_9[0], colorsFrames[0], framesNums0_9[lenNumFrames-1][0], true); // Liga os LEDs com o padrão do número 0
  default_display(); // Exibe o estado padrão no OLED
  itr_SetCallbackFunction(leds_control); // Configura a função de callback para as interrupções
  itr_Interruption(PIN_BUTTON_A); // Habilita a interrupção para o botão A
  itr_Interruption(PIN_BUTTON_B); // Habilita a interrupção para o botão B
  
  while (true){
    sleep_ms(50); // Aguarda 50ms
    Interface_UART_USB(PINS_RGB[0], PINS_RGB[1], PINS_RGB[2]); // Chama a função de interface UART/USB
  }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Initialization(){
  oled_Clear(); // Limpa o display OLED
  oled_Draw_Rectangle(0, 0, 128, 64, false); // Desenha um retângulo na borda do OLED
  oled_Draw_draw((uint8_t[]){0x7E,0x81,0xA5,0x81,0xA5,0x99,0x42,0x3C},(128-11),3,8,8); // Desenha um desenho no OLED
  oled_Write_String("CEPEDI TIC37", 8, 10); // Escreve a string "CEPEDI TIC37" no OLED
  oled_Write_String("EMBARCATECH", 8, 30); // Escreve a string "EMBARCATECH" no OLED
  oled_Write_String("Ligando", 8, 48); // Escreve a string "Ligando" no OLED
  oled_Update(); // Atualiza o OLED com as novas informações
  sleep_ms(700); // Aguarda 700ms
  for (int i = 0; i < 3; i++){
    if(i==0)oled_Write_String("Ligando.", 8, 48); // Escreve "Ligando." no OLED
    if(i==1)oled_Write_String("Ligando..", 8, 48); // Escreve "Ligando.." no OLED
    if(i==2)oled_Write_String("Ligando...", 8, 48); // Escreve "Ligando..." no OLED
    oled_Update(); // Atualiza o OLED com as novas informações  
    sleep_ms(700); // Aguarda 700ms
  }
}
void default_display(){
  oled_Clear(); // Limpa o display OLED
  oled_Draw_Rectangle(0, 0, 128, 64, false); // Desenha um retângulo na borda do OLED
  oled_Draw_draw((uint8_t[]){0x7E,0x81,0xA5,0x81,0xA5,0x99,0x42,0x3C},(128-11),3,8,8); // Desenha um desenho no OLED
  // Mostrando o caractere pressionado
  oled_Write_String("Caractere:", 3, 10); // Escreve "Caractere:" no OLED
  oled_Write_Char(c, 75, 10); // Escreve o caractere lido no OLED
  // Mostrando palavra formada
  oled_Write_String("Palavra:", 3, 30); // Escreve "Palavra:" no OLED
  oled_Write_String(word, 60, 30); // Escreve a palavra formada no OLED
  printf("%s\n", word); // Imprime a palavra formada no console
  // Mostrando o led selecionado
  oled_Write_String("Leds:",3, 47); // Escreve "Leds:" no OLED
  char *ledOn = led == 1? "verde" : led == 2? "azul" : led == 3? "verde/azul":"desligados"; // Determina o estado dos LEDs
  oled_Write_String(ledOn, 40, 47); // Escreve o estado dos LEDs no OLED
  oled_Update(); // Atualiza o OLED com as novas informações
}

void Interface_UART_USB(){
  if (stdio_usb_connected()){ // Verifica se o USB está conectado
    if (scanf(" %c", &c) == 1){ // Lê um caractere da entrada padrão
      if (isdigit(c)){ // Verifica se o caractere é um dígito
        int i = c - '0'; // Converte o caractere para um número inteiro
        ws2812_Map_leds_ON(framesNums0_9[i], colorsFrames[i], framesNums0_9[lenNumFrames-1][i], true); // Liga os LEDs com o padrão do número correspondente
      }
      if(charCount == 8 || charCount == 0){word = malloc(9*sizeof(char)); memset(word, '\0', 9);} // Aloca memória para a palavra se necessário
      if(charCount < 8){ // Verifica se o contador de caracteres é menor que 8
        printf("Caractere digitado: %c\n", c); // Imprime o caractere digitado no console
        word[charCount] = c; // Adiciona o caractere à palavra
        word[charCount+1] = '\0'; // Adiciona o terminador de string
        printf("Palavra formada: %s\n", word); // Imprime a palavra formada no console
      }else{
        printf("Reiniciando a palavra: %c\n", c); // Imprime que a palavra está sendo reiniciada
      }
      default_display(); // Atualiza o display OLED
      charCount = charCount<8?charCount+1:0; // Incrementa o contador de caracteres ou reinicia
    }
  }
}
void leds_control(uint gpio, uint32_t events){
  switch(gpio){ // Verifica qual pino gerou a interrupção
    case PIN_BUTTON_A:
      gpio_put(PINS_RGB[1], !gpio_get(PINS_RGB[1])); // Alterna o estado do LED verde
    break;
    case PIN_BUTTON_B:
      gpio_put(PINS_RGB[2], !gpio_get(PINS_RGB[2])); // Alterna o estado do LED azul
    break;
    default:
    led == 0; // Define o estado dos LEDs como desligado
    break;
  }
  if(gpio_get(PINS_RGB[1]) && gpio_get(PINS_RGB[2])){ // Verifica se ambos os LEDs estão ligados
    printf("leds verde e azul ligados!\n"); // Imprime que ambos os LEDs estão ligados
    led = 3; // Define o estado dos LEDs como verde e azul ligados
  }else if(gpio_get(PINS_RGB[1]) && !gpio_get(PINS_RGB[2])){ // Verifica se apenas o LED verde está ligado
    printf("led verde ligado!\n"); // Imprime que o LED verde está ligado
    led = 1; // Define o estado dos LEDs como verde ligado
  }else if(!gpio_get(PINS_RGB[1]) && gpio_get(PINS_RGB[2])){ // Verifica se apenas o LED azul está ligado
    printf("led azul ligado!\n"); // Imprime que o LED azul está ligado
    led = 2; // Define o estado dos LEDs como azul ligado
  }else{ // Caso contrário
    printf("leds desligados!\n"); // Imprime que os LEDs estão desligados
    led = 0; // Define o estado dos LEDs como desligado
  }
  default_display(); // Atualiza o display OLED
}