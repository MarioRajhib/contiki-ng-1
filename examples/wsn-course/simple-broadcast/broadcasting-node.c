#include "contiki.h"
#include "netstack.h"
#include "net/nullnet.h"
#include "string.h"
 
#include "sys/log.h"
#define LOG_MODULE "Broadcasting Node App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(broadcasting_node, "Broadcasting Node App");
AUTOSTART_PROCESSES(&broadcasting_node);

void input_callback(const void *data,
    uint16_t length,
    const linkaddr_t *source,
    const linkaddr_t *destination)
{
    if (length == sizeof(unsigned))
    {
        unsigned counter;
        memcpy(&counter, data, sizeof(counter));
        LOG_INFO(" Received %u\n", counter);
    }
    
}

PROCESS_THREAD(broadcasting_node, ev, data)
{
    static struct etimer timer;
    static unsigned counter = 0;

    PROCESS_BEGIN();

    /* Initialize NullNet */
    nullnet_buf = (uint8_t *) &counter;
    nullnet_len = sizeof(counter);
    nullnet_set_input_callback(input_callback);

    etimer_set(&timer, SEND_INTERVAL);
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        LOG_INFO("sending %u\n", counter++);

        memcpy(nullnet_but, &counter, sizeof(counter));
        nullnet_len = sizeof(counter);
        NETSTACK_NETWORK.output(NULL);
    }
    PROCESS_END();    
}