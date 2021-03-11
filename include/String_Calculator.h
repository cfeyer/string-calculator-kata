#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <set>

class String_Calculator
{
	public:

		int add( const std::string & expression );
		int get_called_count() const;

		bool has_delimiter_declaration( const std::string & expression ) const;
		std::set<char> get_delimiters( const std::string & expression ) const;
		std::string get_addends_expression( const std::string & expression ) const;
		std::vector<std::string> split( const std::string & expression, const std::set<char> & delimiters ) const;
		std::vector<int> get_addends( const std::string & expression, const std::set<char> & delimiters ) const;
		void throw_if_negative_addends( const std::vector<int> & addends ) const;

		static const int delimiter_declaration_size = 4;
		static const int custom_delimiter_pos = 2;
};

#endif /*STRING_CALCULATOR_H*/

