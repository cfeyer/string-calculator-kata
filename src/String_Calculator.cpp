#include "String_Calculator.h"

#include <sstream>


int String_Calculator::add( const std::string & expression )
{
	int accumulator = 0;
	std::ostringstream token_buffer;

	for( char c : expression )
	{
		bool is_delimiter = ((c == ',') || (c == '\n'));

		if( !is_delimiter )
		{
			token_buffer << c;
		}
		else
		{
			accumulator += std::stoi( token_buffer.str() );
			token_buffer.str( "" );
		}
	}

	if( !token_buffer.str().empty() )
	{
		accumulator += std::stoi( token_buffer.str() );
	}

	return accumulator;
}

