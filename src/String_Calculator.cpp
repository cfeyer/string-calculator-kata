#include "String_Calculator.h"

#include <sstream>
#include <stdexcept>

int String_Calculator::add( const std::string & expression )
{
	const std::set<char> delimiters( get_delimiters(expression) );
	const std::string addends_expression( get_addends_expression(expression) );
	const auto addend_tokens( split(addends_expression, delimiters) );
	int accumulator = 0;

	for( const auto & addend_token : addend_tokens )
	{
		int addend = std::stoi( addend_token );

		if( addend >= 0 )
		{
			accumulator += addend;
		}
		else
		{
			throw std::invalid_argument( "" );
		}
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


std::string String_Calculator::get_addends_expression( const std::string & expression ) const
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

