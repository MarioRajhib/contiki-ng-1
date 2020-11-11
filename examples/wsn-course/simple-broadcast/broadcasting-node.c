#include "contiki.h"        //  Sistema operativo
#include "netstack.h"       //  Liberia para interactuar con la red
#include "net/nullnet/nullnet.h"    //  Libreria para que funcione sin capa de red(sin protocolos de la capa de aplicacion UDP, sin direcccionamiento)
#include "string.h"         //  Libreria para poder incluir informacion en los mensajes de broadcast
 
#include "sys/log.h"        //  Bitacoras
#define LOG_MODULE "Broadcasting Node App" // Nombre
#define LOG_LEVEL LOG_LEVEL_INFO           // Definir nivel de bitacora

#define SEND_INTERVAL (5 * CLOCK_SECOND)    // Definir intervalo tiempo para envio de mensajes

PROCESS(broadcasting_node, "Broadcasting Node App");    // Proceso (variable, Nombre)
AUTOSTART_PROCESSES(&broadcasting_node);                // Proceso a iniciar puntero a la variable

void input_callback(const void *data,                   // funcion callback para cuando llegue mensaje al nodo
    uint16_t length,
    const linkaddr_t *source,
    const linkaddr_t *destination)
{
    if (length == sizeof(unsigned))                     // Mostrar mensaje broadcast
    {
        unsigned counter;
        memcpy(&counter, data, sizeof(counter));        // copiar(direccion, contenido, tamaño)
        LOG_INFO(" Received %u\n", counter);            
    }
    
}

PROCESS_THREAD(broadcasting_node, ev, data)  // Declaracion del programa principal
{
    static struct etimer timer;                 // Temporarizador
    static unsigned counter = 0;                //contador

    PROCESS_BEGIN();

    /* Initialize NullNet: capa de red para hablar con el radio sin ip*/
    nullnet_buf = (uint8_t *) &counter;
    nullnet_len = sizeof(counter);
    nullnet_set_input_callback(input_callback); // Mandar llamar la funcion cada que llegue mensaje y lo imprime

    etimer_set(&timer, SEND_INTERVAL);          // indicar intervalo de tiempo
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); // cuando se expire el tiempo, mandar el broadcast
        LOG_INFO("sending %u\n", counter++);

        memcpy(nullnet_but, &counter, sizeof(counter)); // copiar el numero que hay en nullnet
        nullnet_len = sizeof(counter);                  
        NETSTACK_NETWORK.output(NULL);                  // enviar paquete con el contenido de las variables
    }
    PROCESS_END();                                      
}