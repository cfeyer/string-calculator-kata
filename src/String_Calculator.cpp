#include "String_Calculator.h"

#include <sstream>
#include <stdexcept>


String_Calculator::String_Calculator() :
	m_add_call_count( 0 )
{
}


int String_Calculator::add( const std::string & expression )
{
	m_add_call_count = 1;

	const std::set<char> delimiters( get_delimiters(expression) );
	const std::vector<int> addends( get_addends(expression, delimiters) );

	throw_if_negative_addends( addends );

	int accumulator = 0;

	for( int addend : addends )
	{
		accumulator += addend;
	}

	return accumulator;
}


int String_Calculator::get_called_count() const
{
	return m_add_call_count;
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


std::vector<int> String_Calculator::get_addends( const std::string & expression, const std::set<char> & delimiters ) const
{
	const std::string addends_expression( get_addends_expression(expression) );
	const auto addend_tokens( split(addends_expression, delimiters) );

	std::vector<int> addends;

	for( const std::string & token : addend_tokens )
	{
		addends.push_back( std::stoi(token) );
	}

	return addends;
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
