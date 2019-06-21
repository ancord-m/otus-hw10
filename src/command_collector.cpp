#include <iostream>

#include "command_collector.h"

void CommandCollector::captureCommandAndPerformAnalysis(std::string command)
{
	if(isThisOpenningCurlyBrace(command))
	{
		tryToInterruptAndSaveCurrentBulk();

		openCurlyBrace();
		setCurrentBulkFormedDynamicly(true);
	}
	else if(isThisClosingCurlyBrace(command))
	{
		closeCurlyBrace();
	}
	else
	{
		storeCommandIntoCurrentBulk(command);
	}

	if(doesCurrentBulkFormedDynamicly())
	{
		saveCurrentBulk_IfAllCurlyBracesAreClosed();
	}
	else
	{
		saveCurrentBulk_IfCommandBlockSizeIsReached();
	}

	if(wasCurrentBulkSavedInStorage())
	{
		prepareCurrentBulkForNewCommands();	
		setCurrentBulkWasSavedInStorage(false);
	}
}

/* Internal functions */

void CommandCollector::storeCommandIntoCurrentBulk(String command)
{
	currentBulk.push_back(command);
}

void CommandCollector::saveCurrentBulkInStorage(void)
{
	externalStorage->push(currentBulk);
	setCurrentBulkWasSavedInStorage(true);
}

void CommandCollector::saveCurrentBulk_IfCommandBlockSizeIsReached(void)
{
	if(currentBulk.size() == commandBlockSize)
	{
		saveCurrentBulkInStorage();	
	}
}

void CommandCollector::saveCurrentBulk_IfAllCurlyBracesAreClosed(void)
{
	if(0 == braceCounter)
	{
		setCurrentBulkFormedDynamicly(false);
		saveCurrentBulkInStorage();
	}
}

void CommandCollector::tryToInterruptAndSaveCurrentBulk(void)
{
	if(!isCurrentBulkEmpty() && !doesCurrentBulkFormedDynamicly())
	{
		saveCurrentBulkInStorage();
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

void CommandCollector::openCurlyBrace(void)
{
	++braceCounter;
}

void CommandCollector::closeCurlyBrace(void)
{
	--braceCounter;
}

void CommandCollector::setCurrentBulkFormedDynamicly(bool v)
{
	formingCurrentBulkDynamicly = v;
}

bool CommandCollector::doesCurrentBulkFormedDynamicly(void)
{
	return formingCurrentBulkDynamicly;
}

bool CommandCollector::isCurrentBulkEmpty(void)
{
	return currentBulk.empty();
}

void CommandCollector::setCurrentBulkWasSavedInStorage(bool v)
{
	currentBulkWasSaved = v;
}

bool CommandCollector::wasCurrentBulkSavedInStorage(void)
{
	return currentBulkWasSaved;
}

void CommandCollector::prepareCurrentBulkForNewCommands(void)
{
	currentBulk.clear();
}