#ifndef OLED_BLOCK // Verifica a definição do header do bloco, considerando seu uso em dois blocos
#define OLED_BLOCK
// Bibliotecas necessárias
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "utils.h"


// GPIOS da conexão do OLED
static const uint I2C_SDA = 14;
static const uint I2C_SCL = 15;

// Matriz para print na OLED
static uint8_t ssd[ssd1306_buffer_length];

// Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
static struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};


// Funções para edição do OLED
void print_on_display(char **text);
void write_message(char **text,uint8_t number);
void OLED_initialization();
#endif