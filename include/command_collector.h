#ifndef COMMAND_COLLECTOR_H
#define COMMAND_COLLECTOR_H

#include <string>
#include <vector>

#include "aliases.h"
#include "ibulk_updater.h"

class CommandCollector
{	
	Bulk currentBulk;
	int commandBlockSize;
	
	std::vector<iBulkUpdater *> listeners;
	bool listenersWereNotified;
	int braceCounter;
	bool formingCurrentBulkDynamicly;

	bool isThisOpenningCurlyBrace(String &command);
	bool isThisClosingCurlyBrace(String &command);
	void storeCommandIntoCurrentBulk(String command);
	void openCurlyBrace(void);
	void closeCurlyBrace(void);
	void bulkFormedDynamicly(bool);
	bool doesBulkFormedDynamicly(void);
	void notify_IfAllCurlyBracesAreClosed(void);
	void notify_IfCommandBlockSizeIsReached(void);
	bool isCurrentBulkEmpty(void);
	void notify_ForciblyTerminateCollectionAndNotify(void);
	void prepareCurrentBulkForNewCommands(void);
	void setListenersWereNotified(bool);
	bool wereListenersNotified(void);

public:
	CommandCollector(int bs) :
		commandBlockSize(bs),
		braceCounter(0),
		formingCurrentBulkDynamicly(false),
		listenersWereNotified(false)
	{ 
		currentBulk.reserve(100); 
	};

   ~CommandCollector() = default;	

	void captureCommandAndPerformAnalysis(std::string command);
	void tryToNotifyListenersWithLeftFinishedOrUnfinishedCurrentBulk(void);
	void subscribe(iBulkUpdater *listener);
	void notify(void);
	
};

#endif