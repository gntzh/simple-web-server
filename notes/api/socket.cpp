#include <sys/socket.h> // POSIX socket 库
#include <sys/types.h>  // POSIX 类型 原语

// 服务端
int socket(int domain, int type, int protocol); // 文件描述符
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int fd, int n);
// n 是指已连接但未进行 accept 处理的SOCKET，是SYN队列和ACCEPT队列之和

// 等待连接
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); // 文件描述符


// 客户端
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
