#include <iostream>
#include <typeinfo>

#include "command_collector.h"
#include "console_printer.h"
#include "bulk_to_file_writer.h"

/*
- формировать уникальное имя файла
- запустить несколько файловых потоков
- запустить метрики
- исправить время - начальное поставить
*/

int main(int argc, char const *argv[])
{
	int bulkCapacity = 3;

	if(argc == 2)
	{
		bulkCapacity = std::atoi(argv[1]);
	}

	auto commandCollector = std::make_shared<CommandCollector>(bulkCapacity);

	ConsolePrinter consolePrinter(commandCollector);
	BulkToFileWriter bulkFileWriter(commandCollector, 1);
	
	for(std::string line; std::getline(std::cin, line); ) 
	{
		commandCollector->captureCommandAndPerformAnalysis(line);	
	}

	commandCollector->tryToNotifyListenersWithLeftFinishedOrUnfinishedCurrentBulk();

	commandCollector->stopAuxThreads();

	return 0;	
}