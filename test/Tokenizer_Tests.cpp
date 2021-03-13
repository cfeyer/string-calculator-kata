#include <string>
#include <vector>

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


TEST(TokenizerInterface, HasParseTokensMethodThatTakesStringArg)
{
	Tokenizer concrete_tokenizer;
	Tokenizer_Interface & tokenizer( concrete_tokenizer );

	tokenizer.parse_tokens( std::string() );
}

static const std::vector<std::string> empty_vector;

static void test_parse_tokens( const std::string & expression, const std::vector<std::string> & expected_tokens )
{
	Tokenizer tokenizer;
	EXPECT_EQ( expected_tokens, tokenizer.parse_tokens(expression) );
}

TEST(Tokenizer, ParseTokensDefaultDelimiters)
{
	test_parse_tokens( "", {} );
	test_parse_tokens( "1", {"1"} );
	test_parse_tokens( "10", {"10"} );
	test_parse_tokens( "0", {"0"} );
	test_parse_tokens( "1,2", {"1", "2"} );
	test_parse_tokens( "10,20", {"10", "20"} );
	test_parse_tokens( "1,2,3", {"1", "2", "3"} );
	test_parse_tokens( "1,2,3,42", {"1", "2", "3", "42"} );
	test_parse_tokens( "1\n2", {"1", "2"} );
	test_parse_tokens( "1\n2,3", {"1", "2", "3"} );
}

TEST(Tokenizer, ParseTokenStaticDelimiterDeclaration)
{
	test_parse_tokens( "//;\n1;2", {"1", "2"} );
	test_parse_tokens( "//$\n1$2", {"1", "2"} );
	test_parse_tokens( "// \n1 2", {"1", "2"} );
	test_parse_tokens( "///\n1/2", {"1", "2"} );
	test_parse_tokens( "// \n1 2", {"1", "2"} );
	test_parse_tokens( "///\n1/2", {"1", "2"} );
	test_parse_tokens( "//,\n1,2", {"1", "2"} );
	test_parse_tokens( "//\n\n1\n2", {"1", "2"} );
	test_parse_tokens( "//0\n102", {"1", "2"} );
}

TEST(Tokenizer, ParseTokensSingleDynamicDelimiterDeclaration)
{
	test_parse_tokens( "//[***]\n1***2***3", {"1", "2", "3"} );
	test_parse_tokens( "//[*]\n1*2*3", {"1", "2", "3"} );
	test_parse_tokens( "//[abc]\n1abc2abc3", {"1", "2", "3"} );
	test_parse_tokens( "//[,**]\n1,**2,**3", {"1", "2", "3"} );
	test_parse_tokens( "//[**,]\n1**,2**,3", {"1", "2", "3"} );
	test_parse_tokens( "//[*,*]\n1*,*2*,*3", {"1", "2", "3"} );
	test_parse_tokens( "//[\n**]\n1\n**2\n**3", {"1", "2", "3"} );
	test_parse_tokens( "//[**\n]\n1**\n2**\n3", {"1", "2", "3"} );
	test_parse_tokens( "//[*\n*]\n1*\n*2*\n*3", {"1", "2", "3"} );
	test_parse_tokens( "//[/]\n1/2/3", {"1", "2", "3"} );
	test_parse_tokens( "//[//]\n1//2//3", {"1", "2", "3"} );
	test_parse_tokens( "//[//[]\n1//[2//[3", {"1", "2", "3"} );
	test_parse_tokens( "//[]]\n1]2]3", {"1", "2", "3"} );
	test_parse_tokens( "//[%]\n1%2%3", {"1", "2", "3"} );
	test_parse_tokens( "//[]\n1,2,3", {"1", "2", "3"} );
	test_parse_tokens( "//[[]]\n1[]2[]3", {"1", "2", "3"} );
}

TEST(Tokenizer, ParseTokensMultipleDynamicDelimiterDeclaration)
{
	test_parse_tokens( "//[*][%]\n1*2%3", {"1", "2", "3"} );
	test_parse_tokens( "//[**][%%]\n1**2%%3", {"1", "2", "3"} );
	test_parse_tokens( "//[][]\n1,2,3", {"1", "2", "3"} );
	test_parse_tokens( "//[A,Z]\n1A,Z2A,Z3", {"1", "2", "3"} );
	test_parse_tokens( "//[,AZ]\n1,AZ2,AZ3", {"1", "2", "3"} );
	test_parse_tokens( "//[AZ,]\n1AZ,2AZ,3", {"1", "2", "3"} );
}

