#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <set>
#include <utility>

#include "Tokenizer_Interface.h"

class Tokenizer : public Tokenizer_Interface
{
	public:

		std::vector<std::string> parse_tokens( const std::string & expression ) const override;

	private:

		std::pair<std::set<std::string>, size_t> parse_delimiter_header( const std::string & expression ) const;
		bool parse_dynamic_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const;
		bool parse_static_delimiter_header( const std::string & expression, std::set<std::string> & delimiters, size_t & header_size ) const;
		std::vector<std::string> split( const std::string & expression, const std::set<std::string> & delimiters ) const;
		std::vector<std::string> split( const std::string & expression, const std::string & delimiter ) const;
		std::string replace_all( const std::string & in_this_str, const std::string & from_value, const std::string & to_value ) const;
		std::string replace_all( const std::string & in_this_str, const std::set<std::string> & from_values, const std::string & to_value ) const;
		std::vector<std::string> sort_longest_first( const std::set<std::string> & unsorted ) const;
		std::string ctos( char c ) const;
};

#endif /*TOKENIZER_H*/

