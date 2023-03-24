#include "thread_pool/thread_pool.h"
#include <iostream>
#include <ostream>
#include <string>

int main() {
  thread_pool::ThreadPool pool{10};
  for (int i = 0; i < 10; ++i) {
    pool.execute([i] {
      std::string s = "Thread: ";
      s += std::to_string(i);
      s += "\n";
      std::cout << s << std::flush;
    });
  }
  return 0;
}
