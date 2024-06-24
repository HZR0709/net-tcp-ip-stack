
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pcap_device.h"
#include "xnet_tiny.h"

static pcap_t * pcap;

// pcap所用的网卡
static char ip_str[20] = "192.168.137.1";      // 根据实际电脑上存在的网卡地址进行修改
const char my_mac_addr[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

extern xipaddr_t netif_ipaddr;
/**
 * 初始化网络驱动
 * @return 0成功，其它失败
 */
xnet_err_t xnet_driver_open (uint8_t * mac_addr) {
    memcpy(mac_addr, my_mac_addr, sizeof(my_mac_addr));
    //printf("netif_ipaddr:%s\n", netif_ipaddr.array);
    while (1)
    {
        pcap = pcap_device_open(ip_str, mac_addr, 1);
        if (pcap != (pcap_t *)0) {
            //exit(-1);
            break;
        }
        printf("请输入网卡IP: ");
        scanf("%s", ip_str);

        char ip_addr[16] = { 0 };
        printf("请输入你要使用的IP: ");
        scanf("%15s", ip_addr);
        int parsed_ip[4];
        if (sscanf(ip_addr, "%d.%d.%d.%d", &parsed_ip[0], &parsed_ip[1], &parsed_ip[2], &parsed_ip[3]) == 4) {
            for (int i = 0; i < 4; i++) {
                netif_ipaddr.array[i] = (uint8_t)parsed_ip[i];
            }

            // 打印解析后的IP地址
            printf("Parsed IP address: %d.%d.%d.%d\n",
                netif_ipaddr.array[0], netif_ipaddr.array[1], netif_ipaddr.array[2], netif_ipaddr.array[3]);
            printf("IP address as 32-bit integer: 0x%08X\n", netif_ipaddr.addr);
        }
        else {
            printf("Invalid IP address format.\n");
        }
        
        //sprintf();
    }

    return XNET_ERR_OK;
}

/**
 * 发送数据
 * @param frame 数据起始地址
 * @param size 数据长度
 * @return 0 - 成功，其它失败
 */
xnet_err_t xnet_driver_send (xnet_packet_t * packet) {
    return pcap_device_send(pcap, packet->data, packet->size);
}

/**
 * 读取数据
 * @param frame 数据存储位置
 * @param size 数据长度
 * @return 0 - 成功，其它失败
 */
xnet_err_t xnet_driver_read (xnet_packet_t ** packet) {
    uint16_t size;
    xnet_packet_t * r_packet = xnet_alloc_for_read(XNET_CFG_PACKET_MAX_SIZE);

    size = pcap_device_read(pcap, r_packet->data, XNET_CFG_PACKET_MAX_SIZE);
    if (size) {
        r_packet->size = size;
        *packet = r_packet;
        return XNET_ERR_OK;
    }

    return XNET_ERR_IO;
}

/**
 * 获取自程序启动以来，过去了多长时间
 * @return 程序的系统时间
 */
const xnet_time_t xsys_get_time(void) {
    static uint32_t pre = 0;

    // 以下部分仅供调试时使用
#if 0
    uint32_t c = clock() / CLOCKS_PER_SEC;
    if (c != pre) {
        printf("sec: %d, 100ms: %d\n", c, (xnet_time_t)(clock()  * 10 / CLOCKS_PER_SEC));
        pre = c;
    }
#endif
    return (xnet_time_t)(clock() / CLOCKS_PER_SEC);
}
