#ifndef CONSOLE_PRINTER_H
#define CONSOLE_PRINTER_H

#include <iostream>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

#include "aliases.h"
#include "ibulk_updater.h"
#include "command_collector.h"
#include "resulting_bulk_formatter.h"

class ConsolePrinter : public iBulkUpdater, public ResultingBulkFormatter
{
	public:
		ConsolePrinter(std::shared_ptr<CommandCollector>);
	   ~ConsolePrinter();	

		void update(const Bulk &receivedBulk) override;
		void stop(void) override;

		void print(void);

	private:
		std::queue<Bulk> bulkStorage;
		std::mutex bulkStorageMutex;
		std::condition_variable cv;
		std::thread print_thread;
		std::atomic<bool> stop_thread;
};

#endif