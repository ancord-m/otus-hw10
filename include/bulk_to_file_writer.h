#ifndef BULK_TO_FILE_WRITER_H
#define BULK_TO_FILE_WRITER_H

#include <string>
#include <memory>

#include "aliases.h"
#include "ibulk_updater.h"
#include "command_collector.h"
#include "resulting_bulk_formatter.h"

class BulkToFileWriter : public iBulkUpdater, public ResultingBulkFormatter
{
	public:
		BulkToFileWriter(std::shared_ptr<CommandCollector>, int number_of_threads);
	   ~BulkToFileWriter(); 

	   void update(const Bulk &) override;
	   void stop(void) override;

	   void write(void);

	private:
		const String prefix    = "bulk";
		const String extention = ".log";

		String generateFileName(void);
   		Queue<Bulk> bulkStorage;
		Mutex bulkStorageMutex;
		ConditionVariable cv;
		Thread write_thread;
		Atomic<bool> stop_thread;
};

#endif