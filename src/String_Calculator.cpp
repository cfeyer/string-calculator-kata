#include "String_Calculator.h"

#include <sstream>
#include <regex>

int String_Calculator::add( const std::string & expression )
{
	const std::set<char> delimiters( get_delimiters(expression) );
	const auto tokens = split( expression, delimiters );
	int accumulator = 0;

	for( const auto & token : tokens )
	{
		bool is_delimiter_declaration_token = (token.find("//") == 0);

		if( !is_delimiter_declaration_token && (token != ""))
		{
			accumulator += std::stoi( token );
		}
	}

	return accumulator;
}


std::set<char> String_Calculator::get_delimiters( const std::string & expression ) const
{
	std::set<char> delimiters {',', '\n'};

        if( (expression.size() >= 4) &&
	    (expression.find("//") == 0) && 
	    (expression.at(3) == '\n') )
	{
		char custom_delimiter = expression.at(2);
		delimiters.insert( custom_delimiter );
	}

	return delimiters;
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

