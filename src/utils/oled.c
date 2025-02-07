#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font6x7.h"
#include "oled.h"

#define I2C_PORT i2c1
#define ADDR 0x3C
uint8_t WIDTH = 128;  // Define a largura do display OLED
uint8_t HEIGHT = 64;  // Define a altura do display OLED

static ssd1306_t ssd; // Inicializa a estrutura do display
static uint8_t font_width = 6;   // Defina a largura da fonte
static uint8_t font_height = 7;  // Defina a altura da fonte
static int FONT_START_0_9 = 16;
static int FONT_START_ABC = 27;
static int FONT_START_abc = 53;

void oled_Init(uint pin_i2c_sda, uint pin_i2c_scl){
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(pin_i2c_sda, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(pin_i2c_scl, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(pin_i2c_sda); // Pull up the data line
  gpio_pull_up(pin_i2c_scl); // Pull up the clock line
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDR, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display
  oled_Clear();
}
// Função para desenhar um caractere no display
void oled_Write_Char(char c, uint8_t x, uint8_t y){
  uint16_t index = 0;
  if (c >= 'A' && c <= 'Z')index = (c - 'A' + FONT_START_ABC) * font_height; // Letras maiúsculas
  else if (c >= 'a' && c <= 'z')index = (c - 'a' + FONT_START_abc) * font_height; // Letras minúsculas
  else if (c >= '0' && c <= '9')index = (c - '0' + FONT_START_0_9) * font_height; // números

  for (uint8_t i = 0; i < font_height; i++){  // Percorre as linhas do caractere
    uint8_t line = font[index + i];  // Obtém a linha correspondente do caractere
    for (uint8_t j = 0; j < font_width; j++){  // Percorre os pixels na horizontal
      //váriável para mudar o setido da linha pegar do utimo bit pro primeiro (faça)
      uint rotX = x+(font_width-1) - j;
      ssd1306_pixel(&ssd, rotX, y + i, line & (1 << j));  // Define o pixel no display
    }
  }
}

// Função para desenhar uma string de caracteres no display
void oled_Write_String(const char *str, uint8_t x, uint8_t y){
  ssd1306_t *ssd_p = &ssd;
  uint8_t spacing = font_width + 1; // Espaço entre caracteres (1 pixel de espaçamento)
  while (*str){  // Enquanto houver caracteres na string
    oled_Write_Char(*str++, x, y);  // Desenha o próximo caractere
    x += spacing;  // Avança a posição horizontal
    if (x + spacing >= ssd_p->width){  // Se ultrapassar a largura do display
      x = 0;  // Reseta a posição horizontal
      y += font_height + 1;  // Avança para a próxima linha (com espaçamento)
    }
    if (y + font_height + 1 >= ssd_p->height){  // Se ultrapassar a altura do display
      break;  // Sai do loop
    }
  }
}
void oled_Draw_Rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill){
    ssd1306_rect(&ssd, x, y, width, height, true, fill);// Desenha um retângulo no display
}
void oled_Update(){
    ssd1306_send_data(&ssd);// Envia os dados para o display
}
void oled_Clear(){
    ssd1306_fill(&ssd, false);// Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_send_data(&ssd);// Atualiza os dados para o display
}
