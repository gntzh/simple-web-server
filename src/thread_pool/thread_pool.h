#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace thread_pool {

class ThreadPool {
private:
  using Task = std::function<void()>;
  //   using TaskQueue = std::queue<Task>;

  class TaskQueue {
  private:
    std::mutex mutex;
    std::queue<Task> queue;
    std::condition_variable cv;

  public:
    TaskQueue(){};

    void add(Task &&task) {
      {
        // 单个互斥量，lock_guard即可
        // 多个使用scoped_lock（unique_lock也可以实现）
        std::lock_guard<std::mutex> lock{mutex};
        queue.push(std::move(task));
      }
      cv.notify_one();
    }

    Task get() {
      // unique_lock 配合 condition_variable，可暂时释放
      std::unique_lock<std::mutex> lock{mutex};
      // 若队列为空，阻塞进程，释放队列的锁
      // 等待通知，然后再次检查
      cv.wait(lock, [&] { return !queue.empty(); });
      auto task = std::move(queue.front());
      queue.pop();
      return task;
    }
  };

  class Worker {
  public:
    int id;
    std::thread thread;

  public:
    explicit Worker(int id, TaskQueue &queue)
        : id(id), thread(std::thread{[&queue]() {
            while (true) {
              auto task = queue.get();
              if (task == nullptr) {
                break;
              } else {
                task();
              }
            }
          }}){};
  };

private:
  TaskQueue queue;
  std::vector<Worker> workers;

public:
  explicit ThreadPool(int size) {
    workers.reserve(size);
    for (int i = 0; i < size; ++i) {
      workers.emplace_back(i, queue);
    }
  }
  bool execute(Task &&task) {
    if (task == nullptr) {
      return false;
    }
    queue.add(std::move(task));
    return true;
  }
  ~ThreadPool() {
    for (int i = 0; i < workers.size(); ++i) {
      queue.add(nullptr);
    }
    for (auto &worker : workers) {
      worker.thread.join();
    }
  };
};
} // namespace thread_pool
#endif
