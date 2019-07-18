#include "console_printer.h"

ConsolePrinter::ConsolePrinter(std::shared_ptr<CommandCollector> cc)
{
	cc->subscribe(this);
	print_thread = std::thread(&ConsolePrinter::print, this);
}

void ConsolePrinter::update(const Bulk &receivedBulk)
{
	std::lock_guard<std::mutex> lk(bulkStorageMutex);
	bulkStorage.push(receivedBulk);
	cv.notify_one();
}

void ConsolePrinter::print(void)
{
	while(true)
	{
		std::unique_lock<std::mutex> lk(bulkStorageMutex);
		cv.wait(lk, [&] { return !bulkStorage.empty(); } );
		Bulk bulk = bulkStorage.front();
		bulkStorage.pop();
		std::cout << generateResultingBulkString(bulk) << std::endl;
	}
}