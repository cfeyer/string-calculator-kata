#ifndef MOCK_ADD_OBSEREVER_H
#define MOCK_ADD_OBSEREVER_H

#include "Add_Observer_Interface.h"

class Mock_Add_Observer : public Add_Observer_Interface
{
	public:
		Mock_Add_Observer() :
			call_count( 0 ),
			result( -1 )
		{
		}

		void add_occurred( const std::string & notify_expression, int notify_result ) override
		{
			++call_count;
			expression = notify_expression;
			result = notify_result;
		}

		int call_count;
		std::string expression;
		int result;
};

#endif /*MOCK_ADD_OBSEREVER_H*/

