#include <string>
#include "gmock/gmock.h"

#include "String_Calculator.h"
#include "Add_Observer_Interface.h"
#include "Tokenizer_Interface.h"
#include "Tokenizer.h"

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

static int add( const std::string & str )
{
	Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	return calculator.add( str );
}

TEST(Add, EmptyStringYieldsZero)
{
	EXPECT_EQ( 0, add("") );
}

TEST(Add, OneYieldsOne)
{
	EXPECT_EQ( 1, add("1") );
}

TEST(Add, TenYieldsTen)
{
	EXPECT_EQ( 10, add("10") );
}

TEST(Add, ZeroYieldsZero)
{
	EXPECT_EQ( 0, add("0") );
}

TEST(Add, OneCommaTwoYieldsThree)
{
	EXPECT_EQ( 3, add("1,2") );
}

TEST(Add, TenCommaTwentyYieldsThirty)
{
	EXPECT_EQ( 30, add("10,20") );
}

TEST(Add, AddThreeCommaDelimitedNumbers)
{
	EXPECT_EQ( 6, add("1,2,3") );
}

TEST(Add, AddFourCommaDelimitedNumbers)
{
	EXPECT_EQ( 48, add("1,2,3,42") );
}

TEST(Add, OneNewlineTwoYieldsThree)
{
	EXPECT_EQ( 3, add("1\n2") );
}

TEST(Add, OneNewlineTwoCommaThreeYieldsSix)
{
	EXPECT_EQ( 6, add("1\n2,3") );
}

TEST(Add, DynamicallyDeclareSemicolonAsDelimiter)
{
	EXPECT_EQ( 3, add("//;\n1;2") );
}

TEST(Add, DynamicallyDeclareDollarAsDelimiter)
{
	EXPECT_EQ( 3, add("//$\n1$2") );
}

TEST(Add, DynamicallyDeclareSpaceAsDelimiter)
{
	EXPECT_EQ( 3, add("// \n1 2") );
}

TEST(Add, DynamicallyDeclareSlashAsDelimiter)
{
	EXPECT_EQ( 3, add("///\n1/2") );
}

TEST(Add, DynamicallyDeclareCommaAsDelimiter)
{
	EXPECT_EQ( 3, add("//,\n1,2") );
}

TEST(Add, DynamicallyDeclareNewlineAsDelimiter)
{
	EXPECT_EQ( 3, add("//\n\n1\n2") );
}

TEST(Add, DynamicallyDeclareNumberAsDelimiter)
{
	EXPECT_EQ( 3, add("//0\n102") );
}

TEST(Add, ThrowsExceptionForNegativeAddend)
{
	EXPECT_ANY_THROW( add("-1") );
}

static void test_add_expect_exception( const std::string & input, const std::string & expected_exception_message )
{
	try
	{
		add( input );
		FAIL() << "Expected exception";
	}
	catch( const std::exception & e )
	{
		EXPECT_EQ( expected_exception_message, e.what() );
	}
}

TEST(Add, ThrowsDescriptiveExceptionForNegativeAddend)
{
	test_add_expect_exception( "-1", "negatives not allowed: -1" );
}

TEST(Add, ThrowsDescriptiveExceptionForAnotherNegativeAddend)
{
	test_add_expect_exception( "-42", "negatives not allowed: -42" );
}

TEST(Add, ThrowsDescriptiveExceptionWithAllNegativeAddends)
{
	test_add_expect_exception( "-1,1,-2", "negatives not allowed: -1 -2" );
}

TEST(Add, ThrowsDescriptiveExceptionWithAllDifferentNegativeAddends)
{
	test_add_expect_exception( "1,-2,-4,8", "negatives not allowed: -2 -4" );
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
	Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( "1,2,3,4", observer.expression );
}

TEST(AddObserver, ObserverCalledBackWithCorrectResult)
{
	Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( 10, observer.result );
}

TEST(AddObserver, ObserverCalledBackTwiceWhenAddCalledTwice)
{
	test_observer_call_count( 2 );
}

TEST(AddIgnoresNumbersOverOneThousand, AddThousandOneYieldsZero)
{
	EXPECT_EQ( 0, add("1001") );
}

TEST(AddIgnoresNumbersOverOneThousand, AddThousandTwoHundredTwentyTwoYieldsZero)
{
	EXPECT_EQ( 0, add("2222") );
}

TEST(AddIgnoresNumbersOverOneThousand, AddExpressionWithLargeAndSmallNumbers)
{
	EXPECT_EQ( 2, add("2,1001") );
}

TEST(AddIgnoresNumbersOverOneThousand, OneThousandIsWithinBoundary)
{
	EXPECT_EQ( 1000, add("1000") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[***]\n1***2***3") );
}

TEST(Delimiters, AsteriskAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[*]\n1*2*3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterUsingDifferentSymbol)
{
	EXPECT_EQ( 6, add("//[abc]\n1abc2abc3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterStartingWithComma)
{
	EXPECT_EQ( 6, add("//[,**]\n1,**2,**3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterEndingWithComma)
{
	EXPECT_EQ( 6, add("//[**,]\n1**,2**,3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterWithCommaInMiddle)
{
	EXPECT_EQ( 6, add("//[*,*]\n1*,*2*,*3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterStartingWithNewline)
{
	EXPECT_EQ( 6, add("//[\n**]\n1\n**2\n**3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterEndingWithNewline)
{
	EXPECT_EQ( 6, add("//[**\n]\n1**\n2**\n3") );
}

TEST(Delimiters, MulticharacterDynamicallyDeclaredDelimiterWithNewlineInMiddle)
{
	EXPECT_EQ( 6, add("//[*\n*]\n1*\n*2*\n*3") );
}

TEST(Delimiters, SlashAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[/]\n1/2/3") );
}

TEST(Delimiters, DoubleSlashAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[//]\n1//2//3") );
}

TEST(Delimiters, DoubleSlashLeftBracketAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[//[]\n1//[2//[3") );
}

TEST(Delimiters, RightBracketAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[]]\n1]2]3") );
}

TEST(Delimiters, AsteriskAndPercentAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[*][%]\n1*2%3") );
}

TEST(Delimiters, PercentAsLoneMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[%]\n1%2%3") );
}

TEST(Delimiters, EmptyMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[]\n1,2,3") );
}

TEST(Delimiters, DoubleAsteriskAndDoublePercentAsDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[**][%%]\n1**2%%3") );
}

TEST(Delimiters, LeftBracketRightBracketAsDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[[]]\n1[]2[]3") );
}

TEST(Delimiters, TwoEmptyMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[][]\n1,2,3") );
}

TEST(Delimiters, ACommaZAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[A,Z]\n1A,Z2A,Z3") );
}

TEST(Delimiters, CommaAZAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[,AZ]\n1,AZ2,AZ3") );
}

TEST(Delimiters, AZCommaAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[AZ,]\n1AZ,2AZ,3") );
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
