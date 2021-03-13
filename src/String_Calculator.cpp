#include "String_Calculator.h"

#include <sstream>
#include <stdexcept>
#include <algorithm>

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
	const auto [delimiters, tail] = get_delimiters_and_body( expression );
	return strings_to_ints( split( tail, delimiters ) );
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


std::vector<std::string> String_Calculator::split( const std::string & expression, const std::set<std::string> & delimiters ) const
{
	std::string simpler_expression( replace_all(expression, delimiters, ",") );
	return split_on_comma( simpler_expression );
}


std::vector<std::string> String_Calculator::split_on_comma( const std::string & expression ) const
{
	std::vector<std::string> tokens;
	std::ostringstream token_strm;
	for( char c : expression )
	{
		if( c == ',' )
		{
			tokens.push_back( token_strm.str() );
			token_strm.str( "" );
		}
		else
		{
			token_strm << c;
		}
	}

	if( !token_strm.str().empty() )
	{
		tokens.push_back( token_strm.str() );
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


std::pair<std::set<std::string>, std::string> String_Calculator::get_delimiters_and_body( const std::string & expression ) const
{
	std::set<std::string> delimiters {",", "\n"};
	std::string body;

	if( (expression.find("//[") == 0) && (expression.find("]\n") != std::string::npos ) )
	{
		const auto close_tag_pos = expression.find("]\n");
		std::string custom_delimiter( expression.substr(3, close_tag_pos - 3) );
		delimiters.insert( custom_delimiter );

		body = expression.substr( close_tag_pos + 2 );
	}
	else if( (expression.find("//") == 0) &&
	         (expression.size() >= 4) &&
	         (expression[3] == '\n') )
	{
		std::string custom_delimiter( ctos(expression.at(2)) );
		delimiters.insert( custom_delimiter );

		body = expression.substr(4);
	}
	else
	{
		body = expression;
	}

	return std::make_pair( delimiters, body );
}


std::string String_Calculator::ctos( char c ) const
{
	return std::string( 1, c );
}


std::string String_Calculator::replace_all( const std::string & in_this_str, const std::string & from_value, const std::string & to_value ) const
{
	std::string buffer( in_this_str );

	auto pos = buffer.find( from_value );
	while( pos != std::string::npos )
	{
		buffer.replace( pos, from_value.size(), to_value );
		pos = buffer.find( from_value, pos+1 );
	}

	return buffer;
}


std::string String_Calculator::replace_all( const std::string & in_this_str, const std::set<std::string> & from_values, const std::string & to_value ) const
{
	std::vector<std::string> sorted_from_values( sort_longest_first(from_values) );

	std::string buffer( in_this_str );

	for( const std::string & from_value : sorted_from_values )
	{
		buffer = replace_all( buffer, from_value, "," );
	}

	return buffer;
}


std::vector<std::string> String_Calculator::sort_longest_first( const std::set<std::string> & unsorted ) const
{
	std::vector<std::string> sorted;

	for( const std::string & str : unsorted )
	{
		sorted.push_back( str );
	}

	std::sort( sorted.begin(),
	           sorted.end(),
	           []( const std::string & a, const std::string & b ) { return a.size() > b.size(); }
	);

	return sorted;
}

