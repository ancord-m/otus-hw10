#include <iostream>
#include <typeinfo>

#include "command_collector.h"
#include "console_printer.h"
#include "bulk_to_file_writer.h"

int main(int argc, char const *argv[])
{
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

	return 0;	
}