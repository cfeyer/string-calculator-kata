#ifndef TOKENIZER_INTERFACE_H
#define TOKENIZER_INTERFACE_H

#include <string>
#include <vector>

class Tokenizer_Interface
{
	public:

		virtual ~Tokenizer_Interface() {};

		virtual std::vector<std::string> parse_tokens( const std::string & ) const = 0;
};

#endif /*TOKENIZER_INTERFACE_H*/

