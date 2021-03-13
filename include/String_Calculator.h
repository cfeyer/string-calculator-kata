#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <set>
#include <utility>

class Add_Observer_Interface;

class String_Calculator
{
	public:

		String_Calculator();
		String_Calculator( Add_Observer_Interface & observer );

		int add( const std::string & expression );
		int get_called_count() const;

		std::pair< std::set<std::string>, std::string > get_delimiters_and_body( const std::string & expression ) const;

	private:

		std::vector<int> parse_numbers( const std::string & expression ) const;
		int sum( const std::vector<int> & addends ) const;
		std::vector<std::string> split( const std::string & expression, const std::set<std::string> & delimiters ) const;
		std::vector<std::string> split( const std::string & expression, const std::string & delimiter ) const;
		std::vector<int> strings_to_ints( const std::vector<std::string> & strs ) const;
		void throw_if_has_negative_number( const std::vector<int> & addends ) const;
		void notify_add_occurred( const std::string & expression, int result ) const;
		std::vector<int> filter_out_large_numbers( const std::vector<int> & numbers ) const;
		std::string ctos( char c ) const;
		std::string replace_all( const std::string & in_this_str, const std::string & from_value, const std::string & to_value ) const;
		std::string replace_all( const std::string & in_this_str, const std::set<std::string> & from_values, const std::string & to_value ) const;
		std::vector<std::string> sort_longest_first( const std::set<std::string> & unsorted ) const;

		static const int delimiter_declaration_size = 4;
		static const int custom_delimiter_pos = 2;

		int m_add_call_count;
		Add_Observer_Interface * const mp_observer;
};

#endif /*STRING_CALCULATOR_H*/

