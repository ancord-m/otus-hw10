#ifndef CONSOLE_PRINTER_H
#define CONSOLE_PRINTER_H

#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "aliases.h"
#include "ibulk_updater.h"
#include "command_collector.h"
#include "resulting_bulk_formatter.h"

class ConsolePrinter : public iBulkUpdater, public ResultingBulkFormatter
{
	public:
		ConsolePrinter(const Bulk &bulkToBeShown, std::shared_ptr<std::mutex> mtx, std::condition_variable cv);

		ConsolePrinter(std::shared_ptr<CommandCollector>);

	   ~ConsolePrinter() = default;	

		void update(const Bulk &receivedBulk) override;

		void test(void);
};

#endif