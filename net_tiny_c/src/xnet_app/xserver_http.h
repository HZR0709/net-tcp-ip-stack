
#ifndef XSERVER_HTTP_H
#define XSERVER_HTTP_H

#include "xnet_tiny.h"

// 在VS工程配置->调试->工作目录中有设置起始目录为工程目录的上一层
// 所以下面可直接使用htdocs
#define XHTTP_DOC_DIR               "dist"  // html文档所在的目录

xnet_err_t xserver_http_create(uint16_t port);
void xserver_http_run(void);

#endif // XSERVER_HTTP_H
