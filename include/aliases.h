#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <atomic>

using String 			= std::string;
using Bulk   			= std::vector<std::string>;
using Mutex	 			= std::mutex;
using ConditionVariable = std::condition_variable;
using Thread 			= std::thread;

template <typename T>
using Atomic 			= typename std::atomic<T>;

template <typename T>
using Queue 			= typename std::queue<T>;