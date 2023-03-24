#include "thread_pool/thread_pool.h"
#include <arpa/inet.h> // POSIX IP地址 字符串与整数转换
#include <iostream>
#include <string>
#include <sys/socket.h> // POSIX socket 库
#include <sys/types.h>  // POSIX 类型 原语
#include <thread>
#include <unistd.h> // POSIX 系统 API 访问
#include <vector>

void error_assert(bool v, std::string msg) {
  if (!v) {
    std::perror(msg.c_str());
    std::exit(-1);
  }
}

int init_socket(std::string ip, int port) {

  int _rst_code;
  auto socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  error_assert(socket_fd != -1, "创建 socket 失败");

  sockaddr_in address;
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip.c_str(), &address.sin_addr.s_addr);
  address.sin_port = htons(port); // 绑定到
  _rst_code = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
  error_assert(_rst_code != -1, "绑定/命名 socket 失败");

  _rst_code = listen(socket_fd, 8);
  error_assert(_rst_code != -1, "监听 socket 失败");

  return socket_fd;
}

void handle_connection(int client_fd) {
  const int BUFFER_SIZE = 1024;
  char read_buffer[BUFFER_SIZE];
  int readed_size;
  readed_size = recv(client_fd, read_buffer, BUFFER_SIZE, 0);
  if (readed_size <= 0) {
    close(client_fd);
    return;
  }
  std::string req(read_buffer);
  std::string response;
  std::string content;
  if (req.rfind("GET / HTTP/1.1\r\n") == 0) {
    response = "HTTP/1.1 200 OK\r\n";
    content = "Hello";
  } else {
    response = "HTTP/1.1 404 NOT FOUND\r\n";
    content = "404";
  }
  response += "Content-Length: ";
  response += std::to_string(content.size());
  response += "\r\n\r\n";
  response += content;
  send(client_fd, response.c_str(), response.size(), 0);
  close(client_fd);
}

int main(int argc, char **argv) {
  auto server_fd = init_socket("127.0.0.1", 9090);
  thread_pool::ThreadPool pool(4);
  while (true) {
    // 等待连接
    struct sockaddr_in client_addr;
    unsigned int len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);

    if (client_fd == -1) {
      std::cerr << "接收客户端连接失败" << std::endl;
      break;
    }
    // 打印连接信息
    char client_ip[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip,
              sizeof(client_ip));
    auto client_port = ntohs(client_addr.sin_port);

    std::string s{"客户端IP: "};
    s += client_ip;
    s += "客户端端口: ";
    s += std::to_string(client_port);
    s += "\n";
    std::cout << s << std::flush;
    pool.execute([=] { handle_connection(client_fd); });
  }
  close(server_fd);
  return 0;
}
