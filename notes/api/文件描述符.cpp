#include <unistd.h> // POSIX 系统 API 访问
// 读
ssize_t read(int fd, void *buf, size_t count);
// TCP socket：0 意味着连接断开

// 写
ssize_t write(int fd, const void *buf, size_t count); 
