#include <string>
#include "gtest/gtest.h"

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
