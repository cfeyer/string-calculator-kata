#include "String_Calculator.h"

#include <sstream>
#include <set>


int String_Calculator::add( const std::string & expression )
{
	const auto tokens = split( expression );
	int accumulator = 0;

	for( const auto & token : tokens )
	{
		accumulator += std::stoi( token );
	}

	return accumulator;
}


std::vector<std::string> String_Calculator::split( const std::string & expression ) const
{
	std::set<char> delimiters = {',', '\n'};
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

