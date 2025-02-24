#include "libs/utils.h"
#include "libs/HTTP_block.h"
#include "libs/initialization_block.h"
#include "libs/OLED_block.h"
#include "libs/Status_manager.h"

#define ERROR 0
#define STARTING 1
#define WAITING_CONN 2
#define CONNECTED 3

// Variáveis globais
char msg_control[100] = "Sem info."; // Variável de armazenamento da mensagem enviada pela controladora
bool http_on = false; // Flag de conexão http formada
err_i error_id; // ID do erro ocasionado
bool error_flag = false; // Flag de erro
volatile int CONNECTIONS = 0; // Número de conexões totais

// Mensagens de erro baseado no ID
const static char error_msg[4][50] = {"Erro de inicialização do Wi-fi","Erro de inicialização da porta","Falha na conexão da internet", 
    "Falha na inicialização do servidor HTTP"};

// Funções de acionamento dos blocos
void led_control(uint8_t command);
void oled_update();
void print_report();
void send_data();

int main()
{
    // Flags da máquina de estado principal
    bool treated = false, started = false, stay = false,
    btn_A_pressed = false, btn_B_pressed = false;

    // Inicialização do sistema
    stdio_init_all(); // Inicializa a placa
    sleep_ms(10000); // Tempo para permitir a conexão com o USB
    printf("Iniciando...");
    initialization(); // Função de inicialização

    // Máquina de estados principal
    while (true) {
        
        if(error_flag & !treated){
            printf("Ocorreu um erro.\nCódigo do erro: %X\nDescrição do erro: %s\n", error_id,error_msg[error_id-1]);
            printf("Por favor, reinicie o sistema.");
            led_control(ERROR);
            treated = true;
        }else if (!error_flag & !started)
        {
            con_address fst_id;
            fst_id.usb_address = 0x01;
            started = true;
            new_connection(USB_ID,fst_id,1);
            led_control(WAITING_CONN);
            oled_update();
            
        }else if (started & !http_on){
            if (!gpio_get(GPIO_A_BUTTON) && !btn_A_pressed){
                sleep_ms(50);
                print_report();
                btn_A_pressed = !btn_A_pressed;  
              }else if(gpio_get(GPIO_A_BUTTON) && btn_A_pressed){
                btn_A_pressed = !btn_A_pressed;
              }
        }else if (started & http_on & !stay){
            led_control(CONNECTED);
            oled_update();
            stay = true;
            
        }else if (started & http_on & stay)
        {
            if (!gpio_get(GPIO_A_BUTTON) && !btn_A_pressed){
                sleep_ms(50);
                print_report();
                btn_A_pressed = !btn_A_pressed;  
              }else if(!gpio_get(GPIO_B_BUTTON) && !btn_B_pressed){
                sleep_ms(50);
                send_data();
                btn_B_pressed = !btn_B_pressed;
              }else if(gpio_get(GPIO_A_BUTTON) && btn_A_pressed){
                btn_A_pressed = !btn_A_pressed;
              }else if(gpio_get(GPIO_B_BUTTON) && btn_B_pressed){
                btn_B_pressed = !btn_B_pressed;
              }
        }
        
        
        sleep_ms(100);
    }
}

void led_control(uint8_t command)
{
    switch (command)
    {
    case ERROR:
        gpio_put(LED_PIN_BLUE,false);
        gpio_put(LED_PIN_GREEN,false);
        gpio_put(LED_PIN_RED,true);
        break;
    case STARTING:
        gpio_put(LED_PIN_BLUE,false);
        gpio_put(LED_PIN_GREEN,true);
        gpio_put(LED_PIN_RED,true);
        break;
    case WAITING_CONN:
        gpio_put(LED_PIN_BLUE,true);
        gpio_put(LED_PIN_GREEN,false);
        gpio_put(LED_PIN_RED,false);
        break;
    case CONNECTED:
        gpio_put(LED_PIN_BLUE,false);
        gpio_put(LED_PIN_GREEN,true);
        gpio_put(LED_PIN_RED,false);
        break;    
    default:
        gpio_put(LED_PIN_BLUE,false);
        gpio_put(LED_PIN_GREEN,false);
        gpio_put(LED_PIN_RED,false);        
        break;
    }
}

void oled_update()
{
    char *text[2];
    text[0] = malloc(20);
    text[1] = malloc(20);

    write_message(text,CONNECTIONS);
    print_on_display(text);

}

void print_report()
{
	char *report;
	report = malloc(30);
	generate_report(&report);
	printf("%s", report);
}

void send_data()
{
    printf("\nEscreve uma mensagem(máximo 100 caracteres).\n");
    fgets(msg_control,101,stdin);

}
