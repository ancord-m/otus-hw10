#include "console_printer.h"

ConsolePrinter::ConsolePrinter(std::shared_ptr<CommandCollector> cc)
{
	stop_thread.store(false);
	cc->subscribe(this);
	print_thread = std::thread(&ConsolePrinter::print, this);
}

ConsolePrinter::~ConsolePrinter()
{
	print_thread.join();
}

void ConsolePrinter::update(const Bulk &receivedBulk)
{
	std::lock_guard<std::mutex> lk(bulkStorageMutex);
	bulkStorage.push(receivedBulk);
	cv.notify_one();
}

void ConsolePrinter::stop(void)
{
	stop_thread.store(true);
	cv.notify_all();
}

void ConsolePrinter::print(void)
{
	bool queue_is_empty = false;
	Bulk bulk;

	while(!(stop_thread.load() and queue_is_empty))
	{
		std::unique_lock<std::mutex> lk(bulkStorageMutex);		
		cv.wait(lk, [&] { return !bulkStorage.empty() ||  stop_thread.load(); } );

		bulk = bulkStorage.front();
		bulkStorage.pop();
		queue_is_empty = bulkStorage.empty();
		lk.unlock();

		std::cout << generateResultingBulkString(bulk) << std::endl;
	}
}