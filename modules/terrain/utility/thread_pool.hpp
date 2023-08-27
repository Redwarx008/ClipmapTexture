#ifndef THREAD_POOL_
#define THREAD_POOL_

#include "core/error/error_macros.h"

#include <functional>
#include <future>
#include <intsafe.h>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace terrain {

class ThreadPool
{
public:
  ThreadPool(size_t nThread);
  template<typename F, typename... Args>
  auto Enque(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result_t<F&&, Args&&...>>;
  ~ThreadPool();

private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> _workers;
  // the task queue
  std::queue<std::function<void()>> _tasks;

  // synchronization
  std::mutex _queueMutex;
  std::condition_variable _condition;
  bool _stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t nThread)
  : _stop(false)
{
  for (size_t i = 0; i < nThread; ++i) {
    _workers.emplace_back([this] {
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->_queueMutex);

          this->_condition.wait(
            lock, [this] { return this->_stop || !this->_tasks.empty(); });

          if (this->_stop && this->_tasks.empty()) {
            return;
          }

          task = std::move(this->_tasks.front());
          this->_tasks.pop();
        }

        task();
      }
    });
  }
}

// add new work item to the pool
template<typename F, typename... Args>
auto
ThreadPool::Enque(F&& f, Args&&... args)
  -> std::future<typename std::invoke_result_t<F&&, Args&&...>>
{
  using returnType = typename std::invoke_result_t<F&&, Args&&...>;

  auto task = std::make_shared<std::packaged_task<returnType()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<returnType> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(_queueMutex);

    // don't allow enqueueing after stopping the pool
    if (_stop) {
      // throw std::runtime_error("enqueue on stopped ThreadPool");
      ERR_FAIL_V_MSG(std::future<returnType>(), "enqueue on stopped ThreadPool");
    }

    _tasks.emplace([task]() { (*task)(); });
  }
  _condition.notify_one();
  return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(_queueMutex);
    _stop = true;
  }
  _condition.notify_all();
  for (std::thread& worker : _workers)
    worker.join();
}
}

#endif