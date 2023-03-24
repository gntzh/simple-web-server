#include <arpa/inet.h> // POSIX IP地址 字符串与整数转换
#include <iostream>
#include <string>
#include <sys/socket.h> // POSIX socket 库
#include <sys/types.h>  // POSIX 类型 原语
#include <unistd.h>     // POSIX 系统 API 访问

void error_assert(bool v, std::string msg) {
  if (!v) {
    std::perror(msg.c_str());
    std::exit(-1);
  }
}

int main(int argc, char **argv) {
  int _rst_code;
  // 初始化: IPv4 - TCP - 0
  auto socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  error_assert(socket_fd != -1, "创建 socket 失败");

  // IPv4: Socket 地址
  sockaddr_in address;
  address.sin_family = AF_INET; // IPv4 地址族
  //   address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
  address.sin_port = htons(8080); // 绑定到

  // 绑定/命名 socket-fd - 地址 - 地址长度
  // 强转 + 指针 +  // 大小，以兼容不同地址协议）
  _rst_code = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
  error_assert(_rst_code != -1, "绑定/命名 socket 失败");

  // 监听 socket-fd - 最大连接数（含正在建立连接的）
  _rst_code = listen(socket_fd, 8);
  error_assert(_rst_code != -1, "监听 socket 失败");

  //   struct sockaddr_in client_addr;
  //   int client_addr_len = sizeof(client_addr);
  //   int conn_socket_fd =
  //       accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

  // 等待连接
  struct sockaddr_in client_addr;
  unsigned int len = sizeof(client_addr);
  int conn_socket_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &len);
  error_assert(conn_socket_fd != -1, "接收客户端连接失败");

  // 打印连接信息
  char client_ip[16];
  inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip,
            sizeof(client_ip));
  auto clientPort = ntohs(client_addr.sin_port);
  std::cout << "客户端IP: " << client_ip << std::endl
            << "客户端端口: " << clientPort << std::endl;

  // 获取客户端数据
  auto recv_buffer = new char[64];
  while (true) {
    // 获取客户端的数据
    int recv_size = read(conn_socket_fd, recv_buffer, sizeof(recv_buffer));
    error_assert(recv_size != -1, "接收数据失败");

    if (recv_size > 0) {
      std::cout << "接收到的数据: " << recv_buffer << std::endl;
    } else if (recv_size == 0) { // 表示客户端断开连接
      std::cout << "客户端断开连接" << recv_buffer << std::endl;
      break;
    }

    std::string data{"Hello\b"};
    // 给客户端发送数据
    write(socket_fd, data.c_str(), data.size());
  }

  // 关闭文件描述符
  close(conn_socket_fd);
  close(socket_fd);
  return 0;
}
