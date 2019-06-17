#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

#include "bulk_to_file_writer.h"
#include "resulting_bulk_formatter.h"


BulkToFileWriter::BulkToFileWriter(std::shared_ptr<CommandCollector> cc)
{
	cc->subscribe(this);
}

void BulkToFileWriter::update(const Bulk &receivedBulk)
{
	std::ofstream output;

	output.open(generateFileName());

	if(output.is_open())
	{
		output << generateResultingBulkString(receivedBulk);
	}

	output.close();

	/*
	В логике данной функции существенная ошибка. Если ее вызывать "неспеша", под отладчиком,
	или в ручном режиме подавая данные на стандартный ввод, то она прекрасно отрабатывает
	и каждый bulk пишет в свой log файл. Однако, если запустить seq 0 9 | ./bulk, т.е быстро
	подать кучу данных на вход, запись в файлы работает некорретко. Пишется самый последний 
	log файл. В лучшем случае, парачка промежуточных появится.
	Не помогает даже задержка потока, хотя я сомневаюсь, что на диск что-то таак долго может писаться.
	Нужна Ваша помощь, Дмитрий.
	*/

	std::chrono::milliseconds delay(100);

	std::this_thread::sleep_for(delay);
}

String BulkToFileWriter::generateFileName(void)
{
	return prefix + std::to_string(time(nullptr)) + extention;
}