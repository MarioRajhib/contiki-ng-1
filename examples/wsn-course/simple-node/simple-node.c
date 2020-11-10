#include "contiki.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(simple_node, "Simple Node");
AUTOSTART_PROCESSES(&simple_node);

PROCESS_THREAD(simple_node, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, 5 * CLOCK_SECOND);
    while(1){
        LOG_INFO("Hello World!\n");

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}
