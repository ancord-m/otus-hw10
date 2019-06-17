#ifndef RESULTING_BULK_FORMATTER_H
#define RESULTING_BULK_FORMATTER_H

#include "aliases.h"

class ResultingBulkFormatter
{
	public:
		ResultingBulkFormatter() = default;
	   ~ResultingBulkFormatter() = default;

		String generateResultingBulkString(const Bulk &bulk)
		{
			String result = "bulk: "; 

			for(auto b_it = bulk.cbegin(); b_it != bulk.cend(); ++b_it)
			{
				if(b_it != bulk.cbegin())
				{
					result += ", ";
				}

				result += *b_it;		
			}

			return result;
		}
};

#endif