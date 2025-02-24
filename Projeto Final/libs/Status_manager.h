#ifndef SM_BLOCK
#define SM_BLOCK
#include "utils.h"
// IDs de protocolos
#define HTTP_ID 0x02
#define USB_ID 0x01
// IDs e variáveis de erros
typedef uint8_t err_i; // Tipo atrelado a erro interno
#define WIFI_ERR_INIT 0x01 // Erro de inicialização do Wi-fi
#define HTTP_ERR_PORT_CONN 0x02 // Erro de inicialização da porta
#define WIFI_ERR_CONN 0x03 // Falha na conexão da internet
#define HTTP_ERR_PCB 0x04 // Falha na inicialização do servidor HTTP
extern err_i error_id; // ID do erro ocasionado
extern bool error_flag; // Flag de erro

// Constantes
#define IP_ID_SIZE 4
#define USB_ID_SIZE 1
// Listas de endereço
static volatile uint8_t ip_connections; //Número de conexões IP existentes
static uint8_t usb_connections; // Número de conexões USB existentes
static uint32_t * ip_list; // Lista de endereços IP
static uint8_t * usb_list; // Lista de endereços USB
typedef union con_address{
    uint8_t usb_address;
    uint32_t ip_address;
}con_address; // Definição de tipo de endereço genérico para os dispositivos


// Variáveis de conexões
extern volatile int CONNECTIONS; // Número de conexões totais

struct status_connection
{
    uint8_t id_connection;
    uint8_t protocol;
    uint8_t status;
    uint8_t addr_size;
    con_address addr;
}; // Struct para armazenamento de dados de uma conexão
static struct status_connection * connections_list; // Lista de conexões existentes

// Funções do bloco de gerenciamento de status
uint8_t new_connection(uint8_t protocol, con_address addr, uint8_t addr_size);
uint8_t detect_connection(uint8_t protocol, con_address addr);
void generate_report(char **rep_addr);
void rise_error(err_i err);
int ip_convert(con_address ip, uint8_t *ip_converted);
#endif