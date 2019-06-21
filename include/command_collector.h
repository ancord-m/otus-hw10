#ifndef COMMAND_COLLECTOR_H
#define COMMAND_COLLECTOR_H

#include <string>
#include <vector>

#include "aliases.h"
#include "ibulk_updater.h"

class CommandCollector
{	
	public:
		CommandCollector(std::shared_ptr<BulkStorage> es, int bs) :
			externalStorage(es),
			commandBlockSize(bs),
			braceCounter(0),
			formingCurrentBulkDynamicly(false),
			currentBulkWasSaved(false)
		{ 
			currentBulk.reserve(100); 
		};

	   ~CommandCollector() = default;	

		void captureCommandAndPerformAnalysis(std::string command);
		Bulk getNextBulk(void);
		
	private:
		std::shared_ptr<BulkStorage> externalStorage;
		Bulk currentBulk;
		size_t commandBlockSize;
		size_t braceCounter;
		bool formingCurrentBulkDynamicly;
		bool currentBulkWasSaved;

		void storeCommandIntoCurrentBulk(String command);
		void saveCurrentBulkInStorage(void);
		void saveCurrentBulk_IfCommandBlockSizeIsReached(void);
		void saveCurrentBulk_IfAllCurlyBracesAreClosed(void);
		void tryToInterruptAndSaveCurrentBulk(void);

		bool isThisOpenningCurlyBrace(String &command);
		bool isThisClosingCurlyBrace(String &command);
		void openCurlyBrace(void);
		void closeCurlyBrace(void);
		void setCurrentBulkFormedDynamicly(bool);
		bool doesCurrentBulkFormedDynamicly(void);
		bool isCurrentBulkEmpty(void);
		void setCurrentBulkWasSavedInStorage(bool);
		bool wasCurrentBulkSavedInStorage(void);
		void prepareCurrentBulkForNewCommands(void);	
};

#endif /* COMMAND_COLLECTOR_H */