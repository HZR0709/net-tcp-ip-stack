#include <stdio.h>
#include "xnet_tiny.h"
#include "xserver_datetime.h"
#include "xserver_http.h"

int main (void) {
    xnet_init();

    xserver_datetime_create(13);
    xserver_http_create(80);

    printf("xnet running\n");
    while (1) {
        xserver_http_run();
        xnet_poll();
    }

    return 0;
}
