#include <arpa/inet.h>

// 网络字节序整数  转换为 点分十进制字符串
// 确保字节序：进TCP前，为
// short 转换端口
uint16_t htons(uint16_t hostshort); // 主机字节序 - 网络字节序
uint16_t ntohs(uint16_t netshort);  //  网络字节序 - 主机字节序
// long 转IP
uint32_t htonl(uint32_t hostlong); // 主机字节序 - 网络字节序
uint32_t ntohl(uint32_t netlong);  //  网络字节序 - 主机字节序

// IP地址 字符串与整数转换
int inet_pton(int af, const char *src,
              void *dst); // 点分十进制字符串  转换为 网络字节序整数
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
