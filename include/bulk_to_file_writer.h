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
	const std::string prefix    = "bulk";
	const std::string extention = ".log";

	String generateFileName(void);

	public:
		BulkToFileWriter(std::shared_ptr<CommandCollector>);
	   ~BulkToFileWriter() = default; 

	   void update(const Bulk &) override;
};

#endif