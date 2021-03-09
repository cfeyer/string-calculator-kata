#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>

class String_Calculator
{
	public:

		int add( const std::string & str ) { return str.empty() ? 0 : std::stoi(str); }
};

#endif /*STRING_CALCULATOR_H*/

