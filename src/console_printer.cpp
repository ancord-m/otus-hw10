#include "console_printer.h"

ConsolePrinter::ConsolePrinter(std::shared_ptr<Bulk> bulk, std::shared_ptr<Mutex> mtx, std::shared_ptr<ConditionVariable> cv)
{
	bulk_to_be_printed = bulk;
	bulk_mtx = mtx;
	bulk_updated = cv;
}

void ConsolePrinter::update(void)
{
	std::unique_lock<Mutex> lk(*bulk_mtx);
	(*bulk_updated).wait(lk);
	std::cout << generateResultingBulkString(*bulk_to_be_printed);
	std::cout << std::endl;
}
