## Projeto: Interfaces_UART_SPI_12C_U4C6_EmbarcaTech
## Controle de LEDs, Display OLED e Interrupções com Raspberry Pi Pico

Este projeto foi desenvolvido para consolidar os conceitos de comunicação serial, controle de LEDs, uso de interrupções e manipulação de displays OLED utilizando o microcontrolador **Raspberry Pi Pico**. O projeto foi implementado no ambiente de desenvolvimento **Visual Studio Code (VSCode)** com a extensão **Raspberry Pi Pico** e o simulador **Wokwi** integrado.

## Vídeo de Demonstração

Um vídeo de demonstração do projeto pode ser encontrado [aqui](https://youtube.com/shorts/U87BQHIA9uk?feature=share). O vídeo mostra o funcionamento do projeto na placa BitDogLab, com explicações sobre as funcionalidades implementadas.

## Descrição Geral

O projeto consiste em um sistema que permite a interação com LEDs, botões e um display OLED através de comunicação serial (UART) e I2C. O usuário pode enviar caracteres via UART, que são exibidos no display OLED. Além disso, ao pressionar botões, o estado dos LEDs RGB é alterado, e as informações são exibidas no display e enviadas para o console via UART.

### Componentes Utilizados

- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Conectado às GPIOs 11 (Verde), 12 (Azul) e 13 (Vermelho).
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.
- **Display OLED SSD1306**: Conectado via I2C (GPIO 14 - SDA, GPIO 15 - SCL).

### Funcionalidades

1. **Entrada de Caracteres via UART**:
   - O usuário pode digitar caracteres no Serial Monitor do VSCode.
   - Cada caractere digitado é exibido no display OLED.
   - Se o caractere for um número entre 0 e 9, o símbolo correspondente é exibido na matriz de LEDs WS2812.

2. **Interação com Botões**:
   - **Botão A**: Alterna o estado do LED RGB Verde (ligado/desligado).
   - **Botão B**: Alterna o estado do LED RGB Azul (ligado/desligado).
   - O estado dos LEDs é exibido no display OLED e enviado ao Serial Monitor.

3. **Debouncing**:
   - O projeto implementa o tratamento de bouncing dos botões via software, garantindo que cada pressionamento seja registrado corretamente.

4. **Controle de LEDs**:
   - O projeto demonstra o controle de LEDs comuns (RGB) e LEDs endereçáveis (WS2812), utilizando diferentes técnicas de controle.

5. **Uso de Interrupções**:
   - Todas as funcionalidades relacionadas aos botões são implementadas utilizando interrupções (IRQ), garantindo uma resposta rápida e eficiente.

## Estrutura do Projeto

O projeto é composto por vários arquivos, cada um com uma função específica:

### 1. **main.c**
   - **Função Principal**: Inicializa os componentes e entra em um loop infinito onde verifica a entrada de caracteres via UART e atualiza o display OLED.
   - **Funções**:
     - `Initialization()`: Inicializa o display OLED e exibe uma mensagem de inicialização.
     - `default_display()`: Atualiza o display OLED com o estado atual dos LEDs e o caractere digitado.
     - `Interface_UART_USB()`: Lê caracteres da UART e atualiza o display e os LEDs.
     - `leds_control()`: Controla os LEDs RGB com base nas interrupções dos botões.

### 2. **interrupt.c**
   - **Funções**:
     - `itr_Debounce()`: Implementa o debouncing para evitar múltiplas leituras de um único pressionamento de botão.
     - `itr_Button_Callback()`: Callback executado quando ocorre uma interrupção no botão.
     - `itr_SetCallbackFunction()`: Define a função de callback que será chamada quando ocorrer uma interrupção.
     - `itr_Interruption()`: Configura a interrupção em um pino específico.

### 3. **leds.c**
   - **Funções**:
     - `ws2812_rgb_to_grb()`: Converte cores de RGB para GRB, formato necessário para os LEDs WS2812.
     - `ws2812_init()`: Inicializa o controlador WS2812.
     - `ws2812_Map_leds_ON()`: Ativa LEDs específicos com cores específicas.
     - `ws2812_Clear_leds()`: Limpa o estado dos LEDs, apagando-os.

### 4. **oled.c**
   - **Funções**:
     - `oled_Init()`: Inicializa o display OLED via I2C.
     - `oled_Draw_draw()`: Desenha uma matriz de pixels no display.
     - `oled_Write_Char()`: Escreve um caractere no display.
     - `oled_Write_String()`: Escreve uma string de caracteres no display.
     - `oled_Draw_Rectangle()`: Desenha um retângulo no display.
     - `oled_Update()`: Atualiza o display após alterações.
     - `oled_Clear()`: Limpa o display, apagando todos os pixels.

### 5. **numbers0_9.c**
   - **Funções**:
     - `ColorsGenerate()`: Gera uma matriz de cores para os números de 0 a 9, que são exibidos na matriz de LEDs WS2812.

## Como Executar o Projeto

1. **Configuração do Ambiente**:
   - Instale o **Visual Studio Code**.
   - Adicione a extensão **Raspberry Pi Pico**.
   - Integre o simulador **Wokwi** ao VSCode.

2. **Compilação e Upload**:
   - Abra o projeto no VSCode.
   - Compile o código usando a extensão Raspberry Pi Pico.
   - Faça o upload do código para o Raspberry Pi Pico.

3. **Execução**:
   - Conecte-se ao Serial Monitor do VSCode para enviar caracteres e visualizar as mensagens.
   - Pressione os botões A e B para alternar o estado dos LEDs RGB.
   - Observe as alterações no display OLED e na matriz de LEDs WS2812.


## Considerações Finais

Este projeto é uma excelente oportunidade para consolidar conhecimentos em programação de microcontroladores, manipulação de hardware e desenvolvimento de interfaces de comunicação. O código está bem estruturado e comentado, facilitando o entendimento e a manutenção. Boa sorte e divirta-se explorando as funcionalidades do Raspberry Pi Pico!