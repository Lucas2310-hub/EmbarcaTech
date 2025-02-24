#include "OLED_block.h"

void print_on_display(char **text) // Printa a mensagem no OLED
{
    int y = 0;
    uint8_t reset[ssd1306_buffer_length];

    memset(reset, 0, ssd1306_buffer_length);
    render_on_display(reset, &frame_area);

    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    
    render_on_display(ssd, &frame_area);

}

void write_message(char **text, uint8_t number) //Escreve a mensagem que será alocada no OLED
{
    char first_line[20];

    if (number<=1) {
		sprintf(first_line, " %i dispositivo  ", number);
		strcpy(text[0],first_line);
		strcpy(text[1],"   conectado    ");
	} else {
		sprintf(first_line, "%i dispositivos  ", number);
		strcpy(text[0],first_line);
		strcpy(text[1],"   conectados   ");
	}
}

void OLED_initialization() // Função de inicialização do OLED
{
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();


    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    
}
