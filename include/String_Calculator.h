#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <set>

class String_Calculator
{
	public:

		int add( const std::string & expression );

		std::set<char> get_delimiters( const std::string & expression ) const;
		std::string get_addends_expression( const std::string & expression ) const;
		std::vector<std::string> split( const std::string & expression, const std::set<char> & delimiters ) const;
};

#endif /*STRING_CALCULATOR_H*/

