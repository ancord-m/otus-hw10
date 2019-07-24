#include <iostream>
#include <ctime>

#include "command_collector.h"

CommandCollector::CommandCollector(int bs) :
		commandBlockSize(bs),
		braceCounter(0),
		formingCurrentBulkDynamicly(false),
		listenersWereNotified(false)
{ 
		currentBulk.cmd_block.reserve(100); 
};

void CommandCollector::captureCommandAndPerformAnalysis(std::string command)
{
	if(isThisOpenningCurlyBrace(command))
	{
		tryToNotifyListenersWithLeftFinishedOrUnfinishedCurrentBulk();

		openCurlyBrace();
		bulkFormedDynamicly(true);
	}
	else if(isThisClosingCurlyBrace(command))
	{
		closeCurlyBrace();
	}
	else
	{
		storeCommandIntoCurrentBulk(command);
	}

	if(doesBulkFormedDynamicly())
	{
		notify_IfAllCurlyBracesAreClosed();
	}
	else
	{
		notify_IfCommandBlockSizeIsReached();
	}

	if(wereListenersNotified())
	{
		prepareCurrentBulkForNewCommands();	
		setListenersWereNotified(false);
	}
}

void CommandCollector::tryToNotifyListenersWithLeftFinishedOrUnfinishedCurrentBulk(void)
{
	if(!isCurrentBulkEmpty() && !doesBulkFormedDynamicly())
	{
		notify_ForciblyTerminateCollectionAndNotify();
	}
}

void CommandCollector::subscribe(iBulkUpdater *listener)
{
	listeners.push_back(listener);
}

void CommandCollector::notify(void)
{
	for(auto l : listeners)
	{
		l->update(currentBulk);
	}

	setListenersWereNotified(true);
}

void CommandCollector::stopAuxThreads(void)
{
	for(auto l : listeners)
	{
		l->stop();
	}
}

bool CommandCollector::isThisOpenningCurlyBrace(String &command)
{
	return ( 0 == command.compare("{") );
}

bool CommandCollector::isThisClosingCurlyBrace(String &command)
{
	return ( 0 == command.compare("}") );
}

void CommandCollector::storeCommandIntoCurrentBulk(String command)
{
	if(isCurrentBulkEmpty())
	{
		currentBulk.creation_time = std::time(nullptr);
	}

	currentBulk.cmd_block.push_back(command);
}

void CommandCollector::openCurlyBrace(void)
{
	++braceCounter;
}

void CommandCollector::closeCurlyBrace(void)
{
	--braceCounter;
}

void CommandCollector::bulkFormedDynamicly(bool v)
{
	formingCurrentBulkDynamicly = v;
}

bool CommandCollector::doesBulkFormedDynamicly(void)
{
	return formingCurrentBulkDynamicly;
}

void CommandCollector::notify_IfAllCurlyBracesAreClosed(void)
{
	if(0 == braceCounter)
	{
		bulkFormedDynamicly(false);
		notify();
	} 
}

void CommandCollector::notify_IfCommandBlockSizeIsReached(void)
{
	if(currentBulk.cmd_block.size() == commandBlockSize)
	{
		notify();	
	}
}

bool CommandCollector::isCurrentBulkEmpty(void)
{
	return currentBulk.cmd_block.empty();
}

void CommandCollector::notify_ForciblyTerminateCollectionAndNotify(void)
{
	notify();
}

void CommandCollector::prepareCurrentBulkForNewCommands(void)
{
	currentBulk.cmd_block.clear();
}

void CommandCollector::setListenersWereNotified(bool v)
{
	listenersWereNotified = v;
}

bool CommandCollector::wereListenersNotified(void)
{
	return listenersWereNotified;
}