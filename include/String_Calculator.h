#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>

#include "Tokenizer.h"

class Add_Observer_Interface;

class String_Calculator
{
	public:

		String_Calculator();
		String_Calculator( Add_Observer_Interface & observer );

		int add( const std::string & expression );
		int get_called_count() const;

	private:

		std::vector<int> parse_numbers( const std::string & expression ) const;
		int sum( const std::vector<int> & addends ) const;
		std::vector<int> strings_to_ints( const std::vector<std::string> & strs ) const;
		void throw_if_has_negative_number( const std::vector<int> & addends ) const;
		void notify_add_occurred( const std::string & expression, int result ) const;
		std::vector<int> filter_out_large_numbers( const std::vector<int> & numbers ) const;

		int m_add_call_count;
		Add_Observer_Interface * const mp_observer;
		Tokenizer m_tokenizer;
};

#endif /*STRING_CALCULATOR_H*/

