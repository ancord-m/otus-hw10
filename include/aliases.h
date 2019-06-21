#ifndef ALIASES_H
#define ALIASES_H

#include <vector>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

using String 				= std::string;
using Bulk   				= std::vector<std::string>;
using BulkStorage 			= std::queue<Bulk>;
using Mutex 				= std::mutex;
using ConditionVariable		= std::condition_variable;

#endif /* ALIASES_H */