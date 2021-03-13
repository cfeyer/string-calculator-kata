#include "Tokenizer.h"

#include <algorithm>

std::vector<std::string> Tokenizer::parse_tokens( const std::string & expression ) const
{
	const auto [delimiters, header_size] = parse_delimiter_header( expression );
	const std::string body( expression.substr(header_size) );
	return split(body, delimiters);
}


std::vector<std::string> Tokenizer::split( const std::string & expression, const std::set<std::string> & delimiters ) const
{
	const std::string standard_delimiter( "," );
	std::string simpler_expression( replace_all(expression, delimiters, standard_delimiter) );
	return split( simpler_expression, standard_delimiter );
}


std::vector<std::string> Tokenizer::split( const std::string & expression, const std::string & delimiter ) const
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


std::pair<std::set<std::string>, size_t> Tokenizer::parse_delimiter_header( const std::string & expression ) const
{
	std::set<std::string> delimiters {",", "\n"};
	size_t header_size = 0;

	parse_dynamic_delimiter_header( expression, delimiters, header_size ) ||
		parse_static_delimiter_header( expression, delimiters, header_size );

	return std::make_pair( delimiters, header_size );
}


bool Tokenizer::parse_dynamic_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const
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


bool Tokenizer::parse_static_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const
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


std::string Tokenizer::replace_all( const std::string & in_this_str, const std::string & from_value, const std::string & to_value ) const
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


std::string Tokenizer::replace_all( const std::string & in_this_str, const std::set<std::string> & from_values, const std::string & to_value ) const
{
	std::vector<std::string> sorted_from_values( sort_longest_first(from_values) );

	std::string buffer( in_this_str );

	for( const std::string & from_value : sorted_from_values )
	{
		buffer = replace_all( buffer, from_value, "," );
	}

	return buffer;
}


std::vector<std::string> Tokenizer::sort_longest_first( const std::set<std::string> & unsorted ) const
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


std::string Tokenizer::ctos( char c ) const
{
	return std::string( 1, c );
}


