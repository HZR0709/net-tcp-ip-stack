
#include "xserver_datetime.h"
#include <string.h>
#include <time.h>

#define TIME_STR_SIZE       128         // 时间字符串存储长度

static xnet_err_t datetime_handler (xudp_t * udp, xipaddr_t * src_ip, uint16_t src_port, xnet_packet_t * packet) {
    xnet_packet_t * tx_packet;
    time_t rawtime;
    const struct tm * timeinfo;

    tx_packet = xnet_alloc_for_send(TIME_STR_SIZE);
    if (tx_packet == (xnet_packet_t *)0) {
        return XNET_ERR_MEM;
    }


    time (&rawtime);
    timeinfo = localtime (&rawtime);

    // Weekday, Month Day, Year Time-Zone
    strftime((char *)tx_packet->data, TIME_STR_SIZE, "%A, %B %d, %Y %T-%z", timeinfo);
    truncate_packet(tx_packet, (uint16_t)strlen((char *)tx_packet->data));

    return xudp_out(udp, src_ip, src_port, tx_packet);
}

xnet_err_t xserver_datetime_create(uint16_t port) {
    xnet_err_t err;

    xudp_t* udp = xudp_open(datetime_handler);
    if (udp == (xudp_t*)0) {
        return -1;
    }

    err = xudp_bind(udp, port);
    if (err < 0) {
        xudp_close(udp);
        return err;
    }
    return 0;
}
