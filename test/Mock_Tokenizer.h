#ifndef MOCK_TOKENIZER_H
#define MOCK_TOKENIZER_H

#include "gmock/gmock.h"
#include "Tokenizer_Interface.h"

class Mock_Tokenizer : public Tokenizer_Interface
{
	public:

		MOCK_CONST_METHOD1( parse_tokens, std::vector<std::string>(const std::string &) );
};

#endif /*MOCK_TOKENIZER_H*/

