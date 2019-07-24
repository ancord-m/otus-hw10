#ifndef APPTYPES_H
#define APPTYPES_H

#include "aliases.h"

struct Bulk
{
	std::vector<std::string> cmd_block;
	time_t creation_time;
};

#endif /* APPTYPES_H */