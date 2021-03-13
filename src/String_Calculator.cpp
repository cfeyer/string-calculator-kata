#include "String_Calculator.h"

#include <stdexcept>
#include <sstream>

#include "Add_Observer_Interface.h"
#include "Tokenizer_Interface.h"


String_Calculator::String_Calculator( Tokenizer_Interface & tokenizer ) :
	m_add_call_count( 0 ),
	m_tokenizer( tokenizer ),
	mp_observer( nullptr )
{
}


String_Calculator::String_Calculator( Tokenizer_Interface & tokenizer, Add_Observer_Interface & observer ) :
	m_add_call_count( 0 ),
	m_tokenizer( tokenizer ),
	mp_observer( &observer )
{
}


int String_Calculator::add( const std::string & expression )
{
	++m_add_call_count;

	const std::vector<int> addends( parse_numbers(expression) );

	throw_if_has_negative_number( addends );
	const std::vector<int> filtered_addends( filter_out_large_numbers(addends) );

	int total = sum( filtered_addends );

	notify_add_occurred( expression, total );

	return total;
}


int String_Calculator::get_called_count() const
{
	return m_add_call_count;
}


std::vector<int> String_Calculator::parse_numbers( const std::string & expression ) const
{
	const std::vector<std::string> tokens( m_tokenizer.parse_tokens(expression) );
	return strings_to_ints( tokens );
}


int String_Calculator::sum( const std::vector<int> & addends ) const
{
	int accumulator = 0;

	for( int addend : addends )
	{
		accumulator += addend;
	}

	return accumulator;
}


std::vector<int> String_Calculator::strings_to_ints( const std::vector<std::string> & strs ) const
{
	std::vector<int> ints;

	for( const std::string & str : strs )
	{
		ints.push_back( std::stoi(str) );
	}

	return ints;
}

void String_Calculator::throw_if_has_negative_number( const std::vector<int> & numbers ) const
{
	bool has_negatives = false;
	std::ostringstream message;
	message << "negatives not allowed:";

	for( int number : numbers )
	{
		if( number < 0 )
		{
			has_negatives = true;
			message << " " << number;
		}
	}

	if( has_negatives )
	{
		throw std::invalid_argument( message.str() );
	}
}


void String_Calculator::notify_add_occurred( const std::string & expression, int result ) const
{
	if( mp_observer != nullptr )
	{
		mp_observer->add_occurred( expression, result );
	}
}


std::vector<int> String_Calculator::filter_out_large_numbers( const std::vector<int> & numbers ) const
{
	const int max_allowable_number = 1000;

	std::vector<int> accepted_numbers;

	for( int number : numbers )
	{
		if( number <= max_allowable_number )
		{
			accepted_numbers.push_back( number );
		}
	}

	return accepted_numbers;
}


