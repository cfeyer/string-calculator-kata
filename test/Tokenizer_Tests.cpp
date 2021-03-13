#include <string>
#include "gmock/gmock.h"

#include "Tokenizer.h"
#include "Tokenizer_Interface.h"

TEST(TokenizerDefaultConstructor, CanDefaultConstruct)
{
	Tokenizer tokenizer;
}


TEST(TokenizerImplementsTokenizerInterface, CanCastToTokenizerInterface)
{
	Tokenizer * p_tokenizer = nullptr;
	Tokenizer_Interface * p_interface = p_tokenizer;
}


TEST(TokenizerInteface, HasParseTokensMethodThatTakesStringArg)
{
	Tokenizer concrete_tokenizer;
	Tokenizer_Interface & tokenizer( concrete_tokenizer );

	tokenizer.parse_tokens( std::string() );
}
