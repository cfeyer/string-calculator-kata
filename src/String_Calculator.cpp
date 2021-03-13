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
	const std::vector<std::string> tokens( parse_tokens(expression) );
	return strings_to_ints( tokens );
}


std::vector<std::string> String_Calculator::parse_tokens( const std::string & expression ) const
{
	const auto [delimiters, header_size] = parse_delimiter_header( expression );
	const std::string body( expression.substr(header_size) );
	return split(body, delimiters);
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
	std::string simpler_expression( replace_all(expression, delimiters, ",") ); //TODO Generalize: Replace commma with a delimiter element
	return split( simpler_expression, "," );
}


std::vector<std::string> String_Calculator::split( const std::string & expression, const std::string & delimiter ) const
{
	std::vector<std::string> tokens;

	if( expression.empty() )
	{
		return tokens;
	}

	const size_t expression_size = expression.size();
	const size_t delimiter_size = delimiter.size();

	size_t start_pos = 0;
	size_t delimiter_pos = 0;
	do
	{
		delimiter_pos = expression.find( delimiter, start_pos );
		size_t length = (delimiter_pos == std::string::npos) ? std::string::npos : (delimiter_pos - start_pos);

		if( length > 0 )
		{
			const std::string token( expression.substr(start_pos, length) );
			tokens.push_back( token );
		}

		start_pos = delimiter_pos + delimiter_size;
	}
	while( (delimiter_pos != std::string::npos) && (start_pos < expression.size()) );

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


std::pair<std::set<std::string>, size_t> String_Calculator::parse_delimiter_header( const std::string & expression ) const
{
	std::set<std::string> delimiters {",", "\n"};
	size_t header_size = 0;

	parse_dynamic_delimiter_header( expression, delimiters, header_size ) ||
		parse_static_delimiter_header( expression, delimiters, header_size );

	return std::make_pair( delimiters, header_size );
}


bool String_Calculator::parse_dynamic_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const
{
	const std::string begin_tag( "//[" );
	const std::string delimiter_delimiter( "][" );
	const std::string end_tag( "]\n" );

	if( (expression.find(begin_tag) == 0) &&
	    (expression.find(end_tag) != std::string::npos) )
	{
		const auto end_tag_pos = expression.find( end_tag );
		const size_t blob_length = end_tag_pos - begin_tag.size();
		std::string blob( expression.substr(begin_tag.size(), blob_length) );
		std::vector<std::string> custom_delimiters = split( blob, delimiter_delimiter);
		for( const std::string & custom_delimiter : custom_delimiters )
		{
			delimiters.insert( custom_delimiter );
		}

		header_size = end_tag_pos + end_tag.size();

		return true;
	}
	else
	{
		return false;
	}
}


bool String_Calculator::parse_static_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const
{
	const std::string begin_tag( "//" );
	const std::string end_tag( "\n" );
	const size_t blob_size = 1;

	const size_t hypothetical_header_size = begin_tag.size() + blob_size + end_tag.size();

	if( (expression.find(begin_tag) == 0) &&
	    (expression.size() >= hypothetical_header_size) &&
	    (expression[hypothetical_header_size-1] == '\n') )
	{
		std::string custom_delimiter( ctos(expression.at(begin_tag.size())) );
		delimiters.insert( custom_delimiter );

		header_size = hypothetical_header_size;

		return true;
	}
	else
	{
		return false;
	}
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

