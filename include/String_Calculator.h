#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>

class String_Calculator
{
	public:

		int add( const std::string & expression );

		std::vector<std::string> split( const std::string & expression ) const;
};

#endif /*STRING_CALCULATOR_H*/

