#include "String_Calculator.h"

#include <sstream>
#include <stdexcept>

#include "Add_Observer_Interface.h"


String_Calculator::String_Calculator() :
	m_add_call_count( 0 ),
	mp_observer( nullptr )
{
}


String_Calculator::String_Calculator( Add_Observer_Interface & observer ) :
	m_add_call_count( 0 ),
	mp_observer( &observer )
{
}


int String_Calculator::add( const std::string & expression )
{
	++m_add_call_count;

	const std::vector<int> addends( parse_numbers(expression) );

	throw_if_negative_addends( addends );
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
	const std::set<char> delimiters( get_delimiters(expression) );
	return get_addends(expression, delimiters);
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


bool String_Calculator::has_delimiter_declaration( const std::string & expression ) const
{
	return (expression.size() >= delimiter_declaration_size) &&
	       (expression.find("//") == 0) && 
	       (expression.at(delimiter_declaration_size-1) == '\n');
}


std::set<char> String_Calculator::get_delimiters( const std::string & expression ) const
{
	std::set<char> delimiters {',', '\n'};

        if( has_delimiter_declaration(expression) )
	{
		char custom_delimiter = expression.at(custom_delimiter_pos);
		delimiters.insert( custom_delimiter );
	}

	return delimiters;
}


std::string String_Calculator::get_addends_substring( const std::string & expression ) const
{
	if( has_delimiter_declaration(expression) )
	{
		return std::string( expression, delimiter_declaration_size );
	}
	else
	{
		return expression;
	}
}


std::vector<std::string> String_Calculator::split( const std::string & expression, const std::set<char> & delimiters ) const
{
	std::vector<std::string> tokens;
	std::ostringstream token_buffer;

	for( char c : expression )
	{
		bool is_delimiter = (delimiters.find(c) != delimiters.end());

		if( !is_delimiter )
		{
			token_buffer << c;
		}
		else
		{
			tokens.push_back( token_buffer.str() );
			token_buffer.str( "" );
		}
	}

	if( !token_buffer.str().empty() )
	{
		tokens.push_back( token_buffer.str() );
	}

	return tokens;
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


std::vector<int> String_Calculator::get_addends( const std::string & expression, const std::set<char> & delimiters ) const
{
	const std::string addends_expression( get_addends_substring(expression) );
	const auto addend_tokens( split(addends_expression, delimiters) );
	return strings_to_ints( addend_tokens );
}

void String_Calculator::throw_if_negative_addends( const std::vector<int> & addends ) const
{
	bool has_negatives = false;
	std::ostringstream message;
	message << "negatives not allowed:";

	for( int addend : addends )
	{
		if( addend < 0 )
		{
			has_negatives = true;
			message << " " << addend;
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

