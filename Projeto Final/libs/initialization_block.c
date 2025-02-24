#include "initialization_block.h"

// Função de inicialização de todos os dispositivos
void initialization(){

    // Configura os GPIOS
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE,GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED,GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN,GPIO_OUT);

    gpio_init(GPIO_A_BUTTON);
    gpio_set_dir(GPIO_A_BUTTON, GPIO_IN);
    gpio_pull_up(GPIO_A_BUTTON);
    
    gpio_init(GPIO_B_BUTTON);
    gpio_set_dir(GPIO_B_BUTTON, GPIO_IN);
    gpio_pull_up(GPIO_B_BUTTON);

    gpio_put(LED_PIN_GREEN,true);
    gpio_put(LED_PIN_RED,true);

    // Inicia Wi-fi e OLED
    printf("Iniciando WI-FI");
    if(start_wifi()) return;
    start_http_server();
    OLED_initialization();

}

bool start_wifi(void) // Função de inicialização do Wi-fi e protocolo HTTP
{
    
    if (cyw43_arch_init()) {
        rise_error(WIFI_ERR_INIT);
        return true;
    }
    
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        rise_error(WIFI_ERR_CONN);
        return true;
    }

    uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("Internet conectada!\nIP da placa: %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    return false;
}

