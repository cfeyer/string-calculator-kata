#include <string>
#include "gmock/gmock.h"

#include "String_Calculator.h"
#include "Add_Observer_Interface.h"
#include "Tokenizer_Interface.h"

using namespace testing;

class Mock_Tokenizer : public Tokenizer_Interface
{
	public:

		MOCK_CONST_METHOD1( parse_tokens, std::vector<std::string>(const std::string &) );
};

TEST(StringCalculator, CanConstruct)
{
	String_Calculator calculator( Mock_Tokenizer() );
}

TEST(Add, SignatureCanTakeString)
{
	Mock_Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	const std::string str;
	calculator.add( str );
}

TEST(Add, SignatureCanReturnInt)
{
	Mock_Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	const std::string str;
	int result = calculator.add( str );
}

TEST(Add, CallsTokenizerInterfaceParseTokens)
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(_) );

	String_Calculator calculator( tokenizer );
	calculator.add( "" );
}

static void test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( const std::string & expression )
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(expression) )
		.Times(1);

	String_Calculator calculator( tokenizer );
	calculator.add( expression );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithEmptyExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "" );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithZeroExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "0" );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithOneCommaTwoExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "1,2" );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithStaticDelimiterDeclarationExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "//;\n1;2" );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithDynamicDelimiterDeclarationExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "//[***]\n1***2***3" );
}

TEST(Add, CallsTokenizerInterfaceParseTokensWithMultipleDynamicDelimiterDeclarationExpression)
{
	test_string_calculator_add_calls_tokenizer_parse_tokens_with_expression( "//[*][%]\n1*2%3" );
}

TEST(Add, CallingAddTwiceCallsTokenizerInterfaceParseTokensTwice)
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(_) )
		.Times(2);

	String_Calculator calculator( tokenizer );
	calculator.add( "1" );
	calculator.add( "2" );
}

static const std::string arbitrary_str;

static int add_tokens( const std::vector<std::string> & tokens )
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(_) )
		.Times(1)
		.WillOnce(Return( tokens ));

	String_Calculator calculator( tokenizer );
	return calculator.add( arbitrary_str );
}

TEST(Add, AddTokensReturnedByTokenizer)
{
	EXPECT_EQ( 0, add_tokens({}) );
	EXPECT_EQ( 1, add_tokens({"1"}) );
	EXPECT_EQ( 10, add_tokens({"10"}) );
	EXPECT_EQ( 0, add_tokens({"0"}) );
	EXPECT_EQ( 3, add_tokens({"1", "2"}) );
	EXPECT_EQ( 30, add_tokens({"10", "20"}) );
	EXPECT_EQ( 6, add_tokens({"1", "2", "3"}) );
	EXPECT_EQ( 48, add_tokens({"1", "2", "3", "42"}) );
}

static void test_add_expect_exception( const std::vector<std::string> & tokens, const std::string & expected_exception_message )
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(_) )
		.Times(1)
		.WillOnce(Return( tokens ));

	String_Calculator calculator( tokenizer );

	try
	{
		calculator.add( arbitrary_str );
		FAIL() << "Expected exception";
	}
	catch( const std::exception & e )
	{
		EXPECT_EQ( expected_exception_message, e.what() );
	}
}

TEST(Add, ThrowsDescriptiveExceptionForNegativeAddend)
{
	test_add_expect_exception( {"-1"}, "negatives not allowed: -1" );
}

TEST(Add, ThrowsDescriptiveExceptionForAnotherNegativeAddend)
{
	test_add_expect_exception( {"-42"}, "negatives not allowed: -42" );
}

TEST(Add, ThrowsDescriptiveExceptionWithAllNegativeAddends)
{
	test_add_expect_exception( {"-1", "1", "-2"}, "negatives not allowed: -1 -2" );
}

TEST(Add, ThrowsDescriptiveExceptionWithAllDifferentNegativeAddends)
{
	test_add_expect_exception( {"1", "-2", "-4", "8"}, "negatives not allowed: -2 -4" );
}

TEST(GetCalledCount, SignatureTakesNoArgs)
{
	Mock_Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	calculator.get_called_count();
}

TEST(GetCalledCount, SignatureReturnsInt)
{
	Mock_Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	int n = calculator.get_called_count();
}

static void test_get_called_count( int number_times_called )
{
	Mock_Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );

	for( int i = 0; i < number_times_called; ++i )
	{
		calculator.add( "" );
	}

	EXPECT_EQ( number_times_called, calculator.get_called_count() );
}

TEST(GetCalledCount, AddNotCalled)
{
	test_get_called_count( 0 );
}

TEST(GetCalledCount, AddCalledOnce)
{
	test_get_called_count( 1 );
}

TEST(GetCalledCount, AddCalledTwice)
{
	test_get_called_count( 2 );
}

class Mock_Add_Observer : public Add_Observer_Interface
{
	public:
		Mock_Add_Observer() :
			call_count( 0 ),
			result( -1 )
		{
		}

		void add_occurred( const std::string & notify_expression, int notify_result ) override
		{
			++call_count;
			expression = notify_expression;
			result = notify_result;
		}

		int call_count;
		std::string expression;
		int result;
};

TEST(AddObserver, CanInstantiate)
{
	Mock_Add_Observer observer;
}

TEST(AddObserver, CanPassToStringCalculatorConstructor)
{
	Mock_Tokenizer tokenizer;
	Mock_Add_Observer concrete_observer;
	Add_Observer_Interface & observer_interface( concrete_observer );
	String_Calculator calculator( tokenizer, observer_interface );
}

static void test_observer_call_count( int number_times_add_called )
{
	Mock_Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	for( int i = 0; i < number_times_add_called; ++i )
	{
		calculator.add( "" );
	}

	EXPECT_EQ( number_times_add_called, observer.call_count );
}

TEST(AddObserver, ObserverNotCalledBeforeAddCalled)
{
	test_observer_call_count( 0 );
}

TEST(AddObserver, ObserverHasCallbackMethod)
{
	Mock_Add_Observer concrete_observer;
	Add_Observer_Interface & observer_interface( concrete_observer );

	std::string expression;
	int value = 0;

	observer_interface.add_occurred( expression, value );
}

TEST(AddObserver, ObserverCalledBackOnceWhenAddCalledOnce)
{
	test_observer_call_count( 1 );
}

TEST(AddObserver, ObserverCalledBackWithCorrectExpression)
{
	Mock_Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( "1,2,3,4", observer.expression );
}

TEST(AddObserver, ObserverCalledBackWithCorrectResult)
{
	Mock_Tokenizer tokenizer;
	EXPECT_CALL( tokenizer, parse_tokens(_) )
		.Times(1)
		.WillOnce(Return( std::vector<std::string>{"1", "2", "3", "4"} ));

	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( 10, observer.result );
}

TEST(AddObserver, ObserverCalledBackTwiceWhenAddCalledTwice)
{
	test_observer_call_count( 2 );
}

TEST(Add, IgnoresNumbersOverOneThousand)
{
	EXPECT_EQ( 0, add_tokens({"1001"}) );
	EXPECT_EQ( 0, add_tokens({"2222"}) );
	EXPECT_EQ( 2, add_tokens({"2,1001"}) );
	EXPECT_EQ( 1000, add_tokens({"1000"}) );
}

