#ifndef INIT_BLOCK
#define INIT_BLOCK
// Bibliotecas
#include "utils.h"
#include "Status_manager.h"
#include "HTTP_block.h"
#include "OLED_block.h"

// Configurações de GPIO do sistema
#define GPIO_A_BUTTON 5
#define GPIO_B_BUTTON 6
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define LED_PIN_RED 13

// Senha e SSID da Internet
#define WIFI_SSID "SSID"  // Substitua pelo nome da sua rede Wi-Fi
#define WIFI_PASS "SENHA" // Substitua pela senha da sua rede Wi-Fi

// Funções de inicialização
void initialization();
static bool start_wifi(void);
#endif