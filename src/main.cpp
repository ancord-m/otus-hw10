#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "aliases.h"

#include "command_collector.h"
#include "console_printer.h"
#include "bulk_to_file_writer.h"

using namespace std::chrono_literals;

bool data_ready(false);
std::atomic<bool> stop(false);
std::mutex mtx;
std::condition_variable cv;

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
	//	lk.unlock();
		
		if(counter == 10)
		{
			stop = true;
			
		}
	}
}


/*
Набросок архитектуры:
- потоки логгера и записи в файл спят, ждут события
+ главный поток быстро или не очень формирует бульку.
+ главный поток кладет бульку в хранилище булек - в очередь
+ есть булька-буффер, в которую складываются бульки из хранилища
- если другие потоки выставили флаги "прочитано"&&"записано", то бульку-буффер обновляет очередное значение из хранилища
  то есть очередь освобождается
+ как только обновили буфер - шлем notify_all
- здесь будет работать RWLock скорее всего
- все потоки поработали с буффером и выставили свои флаги.
*/

int main(int argc, char const *argv[])
{
	int bulkCapacity = 3;
	auto bulk_storage = std::make_shared<BulkStorage>();
	auto ready_for_output_bulk = std::make_shared<Bulk>();
	auto bulk_mutex = std::make_shared<Mutex>();
	auto bulk_updated = std::make_shared<ConditionVariable>();

	if(argc == 2)
	{
		bulkCapacity = std::atoi(argv[1]);
	}

	CommandCollector commandCollector(bulk_storage, bulkCapacity);
	std::thread console_printer_thread(&ConsolePrinter::update, ConsolePrinter(ready_for_output_bulk, bulk_mutex, bulk_updated));

	for(std::string line; std::getline(std::cin, line); ) 
	{
		commandCollector.captureCommandAndPerformAnalysis(line);

		if(!bulk_storage->empty())
		{
			{
				std::lock_guard<Mutex> lk(*bulk_mutex);
				*ready_for_output_bulk = commandCollector.getNextBulk();
				//todo: set bool flags have_written/have_printed
			}

			(*bulk_updated).notify_all();
		}
	}

	//fixme: commandCollector.tryToInterruptAndSaveCurrentBulk(); ????


	
	//std::thread console_printer_thread(&ConsolePrinter::test, ConsolePrinter());

	/*
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
			std::lock_guard<std::mutex> lk(mtx);
			if(stop.load()) break;
		}		
	}*/


	//console_printer_thread.join();

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