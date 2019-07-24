#ifndef IBULK_UPDATER_H
#define IBULK_UPDATER_H

#include "AppTypes.h"
#include "aliases.h"

class iBulkUpdater {
public:
	virtual ~iBulkUpdater() {};

	virtual void update(const Bulk &) 	= 0;
	virtual void stop(void)				= 0;
};

#endif