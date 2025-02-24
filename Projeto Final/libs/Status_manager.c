#include "Status_manager.h"
#include "libs/utils.h"

uint8_t detect_connection(uint8_t protocol, con_address addr){
    int i = 0, j = 0;
    bool exist = false;
    uint8_t id_conn = 0x00;
    con_address test_addr;
  
    
    for(i=0;i<CONNECTIONS;i++){
        if(protocol==connections_list[i].protocol)
        {
            switch (protocol)
            {
            case HTTP_ID:
                exist = connections_list[i].addr.ip_address==addr.ip_address;
                if(exist) id_conn = connections_list[i].id_connection;
                break;
            case USB_ID:
                exist = connections_list[i].addr.usb_address==addr.usb_address;
                if(exist) id_conn = connections_list[i].id_connection;
                break;
            default:
                break;
            }
        }
    }

    return id_conn;
}

// Função de conversão de endereço
int ip_convert(con_address ip, uint8_t *ip_converted){
    if((sizeof(ip_converted)/sizeof(ip_converted[0]))!=4.f){
        return 1;
    }else{
        ip_converted[0] = ip.ip_address & 0x000000FF;
        ip_converted[1] = (ip.ip_address & 0x0000FF00)>>8;
        ip_converted[2] = (ip.ip_address & 0x00FF0000)>>16;
        ip_converted[3] = (ip.ip_address & 0xFF000000)>>24;
    }

    return 0;
}


void generate_report(char **rep_addr)
{
	char *report, line_usb[50] = "ID: %u || Protocolo: %s || EndereC'o: 0x%02X\n",line_http[50] = "ID: %u || Protocolo: %s || EndereC'o: %d.%d.%d.%d\n", temp_line[50];
    uint8_t ip_converted[4];

	if(CONNECTIONS==0) {
		*rep_addr = realloc(*rep_addr, 30);
		strcpy(*rep_addr,"Sem conexões ativas.");
		return;
	}
	report = malloc(CONNECTIONS*50);
	*rep_addr = realloc(*rep_addr, sizeof(report));

	for (int i = 0 ; i < CONNECTIONS; i++)
	{

		switch (connections_list[i].protocol)
		{
		case HTTP_ID:
			ip_convert(connections_list[i].addr,ip_converted);
            sprintf(temp_line,line_http,connections_list[i].id_connection,"HTTP",ip_converted[0],ip_converted[1],ip_converted[2],ip_converted[3]);
			strcat(report,temp_line);
			break;
		case USB_ID:
			sprintf(temp_line,line_usb,connections_list[i].id_connection,"USB",connections_list[i].addr.usb_address);
			strcat(report,temp_line);
		default:
			break;
		}
	}

	strcpy(*rep_addr,report);
	return;
}

uint8_t new_connection(uint8_t protocol, con_address addr, uint8_t addr_size){
    uint8_t id_con;
    struct status_connection stcon;

    // Detecta a existência do endereço e retorna o id caso a conexão já exista
    if(id_con=detect_connection(protocol,addr)) return id_con;

    // Atualiza as listas de endereço de protocolos
    if(protocol==HTTP_ID){
        if(ip_connections==0){
            ip_list = malloc(sizeof(uint32_t));
        }else{
            ip_list = realloc(ip_list,(CONNECTIONS+1)*sizeof(uint32_t));
        }
        ip_connections++;
        *(ip_list+ip_connections-1) = addr.ip_address;

    }else if (protocol==USB_ID)
    {
        if(!usb_connections){
            usb_list = malloc(sizeof(uint8_t));
        }else{
            usb_list = realloc(usb_list,(CONNECTIONS+1)*sizeof(uint8_t));
        }
        usb_connections++;
        *(usb_list+usb_connections-1) = addr.usb_address;
    }
    

    // Atualiza a lista de conexões ativas
    if(CONNECTIONS==0){
        connections_list = malloc(sizeof(struct status_connection));
        id_con = 1;
    }else{
        connections_list = realloc(connections_list,(CONNECTIONS+1)*sizeof(struct status_connection));
        id_con = CONNECTIONS+1;
    }
    CONNECTIONS++;
    
    connections_list[id_con-1].protocol = protocol;
    connections_list[id_con-1].addr_size = addr_size;
    connections_list[id_con-1].id_connection = id_con;
    connections_list[id_con-1].status = 1;
    connections_list[id_con-1].addr = addr;

    return id_con;
};

void rise_error(err_i err){
    error_id = err;
    error_flag = true;
}