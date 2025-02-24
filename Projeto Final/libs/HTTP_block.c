#include "HTTP_block.h"
#include "Status_manager.h"


void create_http_response()
{
    snprintf(http_response, sizeof(http_response),
    "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "  <meta charset=\"UTF-8\">"
    "  <title>Comunicação com a controladora</title>"
    "</head>"
    "<body>"
    "  <h1>Mensagem da controladora:</h1>"
    "  <p><a href=\"/update\">Atualizar Estado</a></p>"
    "  <h2>Mensagem:</h2>"
    "  <p>%s</p>"
    "</body>"
    "</html>\r\n",
    msg_control);
}

// Função de callback para processar requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Cliente fechou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a requisição HTTP
    char *request = (char *)p->payload;

    // Cria a resposta HTTP
    create_http_response();

    // Envia a resposta HTTP
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

// Callback de conexão: associa o http_callback à conexão
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    ip_addr_t remote_ip;
    con_address remote_ip_addr;
    uint8_t remote_ip_converted[4];
    remote_ip = newpcb->remote_ip;
    
    remote_ip_addr.ip_address = remote_ip.addr;
    new_connection(HTTP_ID,remote_ip_addr,4);
    http_on = true;
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

// Função de setup do servidor TCP
void start_http_server(void){
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        rise_error(HTTP_ERR_PCB);
        return;
    }

    // Liga o servidor na porta 80
    if (tcp_bind(pcb, IP_ADDR_ANY, PCB_PORT) != ERR_OK) {
        rise_error(HTTP_ERR_PORT_CONN);
        return;
    }

    pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
    tcp_accept(pcb, connection_callback);  // Associa o callback de conexão
    printf("Servidor rodando na porta %d.\n", PCB_PORT);
}