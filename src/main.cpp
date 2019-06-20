#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "command_collector.h"
#include "console_printer.h"
#include "bulk_to_file_writer.h"

using namespace std::chrono_literals;

bool data_ready(false);
std::atomic<bool> stop(false);
std::mutex mtx;
std::condition_variable cv, cv_stop;

void foo(void)
{
	int counter = 0;

	while(true && !stop.load())
	{
		std::unique_lock<std::mutex> lk(mtx);
		cv.wait(lk, []{return data_ready;});
		if(data_ready)
		{
			std::cout << "Calculating..." << std::endl;
			data_ready = false;
			++counter;
		}	
		lk.unlock();
		
		if(counter == 10)
		{
			stop = true;
			cv_stop.notify_one();
		}
	}
}

int main(int argc, char const *argv[])
{
	
	std::thread console_printer_thread(foo);

	while(true && !stop.load())
	{		
		std::this_thread::sleep_for(1s);

		{
			std::lock_guard<std::mutex> lk(mtx);
			data_ready = true;
			std::cout << "Preparing data..." << std::endl;
		}
		cv.notify_one();

		{
			std::unique_lock<std::mutex> lk(mtx);
			cv_stop.wait_for(lk, 200ms, [] { return stop.load(); });	
		}		
	}


	console_printer_thread.join();

	/*
	int bulkCapacity = 3;

	if(argc == 2)
	{
		bulkCapacity = std::atoi(argv[1]);
	}

	auto commandCollector = std::make_shared<CommandCollector>(bulkCapacity);

	ConsolePrinter consolePrinter(commandCollector);
	BulkToFileWriter bulkFileWriter(commandCollector);
	
	for(std::string line; std::getline(std::cin, line); ) 
	{
		commandCollector->captureCommandAndPerformAnalysis(line);	
	}

	commandCollector->tryToNotifyListenersWithLeftFinishedOrUnfinishedCurrentBulk();
	*/

	return 0;	
}

auto measure_func_work_time = [](auto&& func, auto&&... params)
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	std::forward<decltype(func)>(func)
		(std::forward<decltype(params)>(params)...);

	auto result =
		std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::system_clock::now() - start).count();

	std::cout << "Function call took " << result << " us" << std::endl;	
};