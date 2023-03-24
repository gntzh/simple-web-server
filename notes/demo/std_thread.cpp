#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>
#include <vector>

std::mutex cout_mutex;

void foo(int num) {
  cout_mutex.lock();
  std::cout << ">>> "
            << "Thread: " << num << " (" << std::this_thread::get_id() << ")"
            << std::endl;
  cout_mutex.unlock();
  std::this_thread::sleep_for(std::chrono::seconds{std::min(1, 8 - num)});
  cout_mutex.lock();
  std::cout << "Thread: " << num << " <<< " << std::endl;
  cout_mutex.unlock();
}

int main(int argc, char **argv) {
  const int thread_num = 5;
  std::vector<std::thread> threads;

  threads.reserve(thread_num);
  for (int i = 0; i < thread_num; ++i) {
    threads.emplace_back(foo, i);
  }
  for (auto &t : threads) {
    t.join();
  }
  std::cout << "work in main thread" << std::endl;
  std::cout << "" << std::endl;
  return 0;
}
