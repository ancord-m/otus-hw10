#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include "bulk_to_file_writer.h"
#include "resulting_bulk_formatter.h"


BulkToFileWriter::BulkToFileWriter(std::shared_ptr<CommandCollector> cc, int number_of_threads)
{
	stop_thread.store(false);
	cc->subscribe(this);
	write_thread = std::thread(&BulkToFileWriter::write, this);
}

BulkToFileWriter::~BulkToFileWriter()
{
	write_thread.join();
}

void BulkToFileWriter::update(const Bulk &receivedBulk)
{
	std::lock_guard<std::mutex> lk(bulkStorageMutex);
	bulkStorage.push(receivedBulk);
	cv.notify_one();
}

void BulkToFileWriter::stop(void)
{
	stop_thread.store(true);
	cv.notify_all();
}

void BulkToFileWriter::write(void)
{	
	bool queue_is_empty = false;
	Bulk bulk;
	std::ofstream output;

	while(!(stop_thread.load() and queue_is_empty))
	{
		std::unique_lock<std::mutex> lk(bulkStorageMutex);		
		cv.wait(lk, [&] { return !bulkStorage.empty() ||  stop_thread.load(); } );

		bulk = bulkStorage.front();
		bulkStorage.pop();
		queue_is_empty = bulkStorage.empty();
		lk.unlock();

		output.open(generateFileName(bulk));
		if(output.is_open())
		{
			output << generateResultingBulkString(bulk);
		}

		output.close();
	}	
}

String BulkToFileWriter::generateFileName(Bulk bulk)
{
	return prefix + std::to_string(bulk.creation_time) + "_" + addUniqueSuffix() + std::to_string(std::rand()) + extention;
}

String BulkToFileWriter::addUniqueSuffix(void)
{
	String suffix;
	std::stringstream sstream;

	sstream << std::hex << std::this_thread::get_id();
	suffix = sstream.str();

	return suffix;
}