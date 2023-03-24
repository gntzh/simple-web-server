#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // POSIX 系统 API 访问
// 读
ssize_t read(int fd, void *buf, size_t count);
// TCP socket：0 意味着连接断开

// 写
ssize_t write(int fd, const void *buf, size_t count);

// 读-非阻塞
// flags 默认 0, 常用的有
// MSG_PEEK 只看不拿
// MAG_WAITALL 在尚未读取输入的nbytes数目的数据之前，不返回
ssize_t recv(int fd, void *buf, size_t n, int flags);
// 写-非阻塞
ssize_t send(int fd, const void *buf, size_t n, int flags);
