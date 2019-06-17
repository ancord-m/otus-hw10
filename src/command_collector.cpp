#include <iostream>

#include "command_collector.h"

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
	for(auto l : listeners) {
		l->update(currentBulk);
	}

	setListenersWereNotified(true);
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
	currentBulk.push_back(command);
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
	if(currentBulk.size() == commandBlockSize)
	{
		notify();	
	}
}

bool CommandCollector::isCurrentBulkEmpty(void)
{
	return currentBulk.empty();
}

void CommandCollector::notify_ForciblyTerminateCollectionAndNotify(void)
{
	notify();
}

void CommandCollector::prepareCurrentBulkForNewCommands(void)
{
	currentBulk.clear();
}

void CommandCollector::setListenersWereNotified(bool v)
{
	listenersWereNotified = v;
}

bool CommandCollector::wereListenersNotified(void)
{
	return listenersWereNotified;
}