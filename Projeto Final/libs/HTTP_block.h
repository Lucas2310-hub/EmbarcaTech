#ifndef HTTP_BLOCK
#define HTTP_BLOCK
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "utils.h"

#define PCB_PORT 80 // Porta de conexão do PCB


static char http_response[1024]; // Buffer de resposta HTTP

extern char msg_control[100]; // Variável de armazenamento da mensagem enviada pela controladora
extern bool http_on; // Flag de conexão http formada

// Funções do bloco HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
void start_http_server(void);
void create_http_response();

#endif