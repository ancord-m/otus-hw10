#ifndef CONSOLE_PRINTER_H
#define CONSOLE_PRINTER_H

#include <iostream>
#include <memory>

#include "aliases.h"
#include "ibulk_updater.h"
#include "command_collector.h"
#include "resulting_bulk_formatter.h"

class ConsolePrinter : public iBulkUpdater, public ResultingBulkFormatter
{
	public:
		ConsolePrinter(std::shared_ptr<CommandCollector>);

	   ~ConsolePrinter() = default;	

		void update(const Bulk &receivedBulk) override;
};

#endif