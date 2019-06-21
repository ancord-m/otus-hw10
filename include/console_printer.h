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

class ConsolePrinter : public ResultingBulkFormatter
{
	public:
		ConsolePrinter(std::shared_ptr<Bulk> bulk, std::shared_ptr<Mutex> mtx, std::shared_ptr<ConditionVariable> cv);		

	   ~ConsolePrinter() = default;	

		void update(void);

	private:
		std::shared_ptr<Bulk> bulk_to_be_printed;
		std::shared_ptr<Mutex> bulk_mtx;
		std::shared_ptr<ConditionVariable> bulk_updated;
};

#endif