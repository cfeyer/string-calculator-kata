#include <string>
#include "gmock/gmock.h"

#include "String_Calculator.h"
#include "Add_Observer_Interface.h"
#include "Tokenizer.h"

using namespace testing;

static int add( const std::string & str )
{
	Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	return calculator.add( str );
}

TEST(AcceptanceTestAdd, EmptyStringYieldsZero)
{
	EXPECT_EQ( 0, add("") );
}

TEST(AcceptanceTestAdd, OneYieldsOne)
{
	EXPECT_EQ( 1, add("1") );
}

TEST(AcceptanceTestAdd, TenYieldsTen)
{
	EXPECT_EQ( 10, add("10") );
}

TEST(AcceptanceTestAdd, ZeroYieldsZero)
{
	EXPECT_EQ( 0, add("0") );
}

TEST(AcceptanceTestAdd, OneCommaTwoYieldsThree)
{
	EXPECT_EQ( 3, add("1,2") );
}

TEST(AcceptanceTestAdd, TenCommaTwentyYieldsThirty)
{
	EXPECT_EQ( 30, add("10,20") );
}

TEST(AcceptanceTestAdd, AddThreeCommaDelimitedNumbers)
{
	EXPECT_EQ( 6, add("1,2,3") );
}

TEST(AcceptanceTestAdd, AddFourCommaDelimitedNumbers)
{
	EXPECT_EQ( 48, add("1,2,3,42") );
}

TEST(AcceptanceTestAdd, OneNewlineTwoYieldsThree)
{
	EXPECT_EQ( 3, add("1\n2") );
}

TEST(AcceptanceTestAdd, OneNewlineTwoCommaThreeYieldsSix)
{
	EXPECT_EQ( 6, add("1\n2,3") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareSemicolonAsDelimiter)
{
	EXPECT_EQ( 3, add("//;\n1;2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareDollarAsDelimiter)
{
	EXPECT_EQ( 3, add("//$\n1$2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareSpaceAsDelimiter)
{
	EXPECT_EQ( 3, add("// \n1 2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareSlashAsDelimiter)
{
	EXPECT_EQ( 3, add("///\n1/2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareCommaAsDelimiter)
{
	EXPECT_EQ( 3, add("//,\n1,2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareNewlineAsDelimiter)
{
	EXPECT_EQ( 3, add("//\n\n1\n2") );
}

TEST(AcceptanceTestAdd, DynamicallyDeclareNumberAsDelimiter)
{
	EXPECT_EQ( 3, add("//0\n102") );
}

TEST(AcceptanceTestAdd, ThrowsExceptionForNegativeAddend)
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

TEST(AcceptanceTestAdd, ThrowsDescriptiveExceptionForNegativeAddend)
{
	test_add_expect_exception( "-1", "negatives not allowed: -1" );
}

TEST(AcceptanceTestAdd, ThrowsDescriptiveExceptionForAnotherNegativeAddend)
{
	test_add_expect_exception( "-42", "negatives not allowed: -42" );
}

TEST(AcceptanceTestAdd, ThrowsDescriptiveExceptionWithAllNegativeAddends)
{
	test_add_expect_exception( "-1,1,-2", "negatives not allowed: -1 -2" );
}

TEST(AcceptanceTestAdd, ThrowsDescriptiveExceptionWithAllDifferentNegativeAddends)
{
	test_add_expect_exception( "1,-2,-4,8", "negatives not allowed: -2 -4" );
}

TEST(AcceptanceTestGetCalledCount, SignatureReturnsInt)
{
	Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );
	int n = calculator.get_called_count();
}

static void test_get_called_count( int number_times_called )
{
	Tokenizer tokenizer;
	String_Calculator calculator( tokenizer );

	for( int i = 0; i < number_times_called; ++i )
	{
		calculator.add( "" );
	}

	EXPECT_EQ( number_times_called, calculator.get_called_count() );
}

TEST(AcceptanceTestGetCalledCount, AddNotCalled)
{
	test_get_called_count( 0 );
}

TEST(AcceptanceTestGetCalledCount, AddCalledOnce)
{
	test_get_called_count( 1 );
}

TEST(AcceptanceTestGetCalledCount, AddCalledTwice)
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

static void test_observer_call_count( int number_times_add_called )
{
	Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	for( int i = 0; i < number_times_add_called; ++i )
	{
		calculator.add( "" );
	}

	EXPECT_EQ( number_times_add_called, observer.call_count );
}

TEST(AcceptanceTestAddObserver, ObserverNotCalledBeforeAddCalled)
{
	test_observer_call_count( 0 );
}

TEST(AcceptanceTestAddObserver, ObserverHasCallbackMethod)
{
	Mock_Add_Observer concrete_observer;
	Add_Observer_Interface & observer_interface( concrete_observer );

	std::string expression;
	int value = 0;

	observer_interface.add_occurred( expression, value );
}

TEST(AcceptanceTestAddObserver, ObserverCalledBackOnceWhenAddCalledOnce)
{
	test_observer_call_count( 1 );
}

TEST(AcceptanceTestAddObserver, ObserverCalledBackWithCorrectExpression)
{
	Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( "1,2,3,4", observer.expression );
}

TEST(AcceptanceTestAddObserver, ObserverCalledBackWithCorrectResult)
{
	Tokenizer tokenizer;
	Mock_Add_Observer observer;
	String_Calculator calculator( tokenizer, observer );

	calculator.add( "1,2,3,4" );

	EXPECT_EQ( 10, observer.result );
}

TEST(AcceptanceTestAddObserver, ObserverCalledBackTwiceWhenAddCalledTwice)
{
	test_observer_call_count( 2 );
}

TEST(AcceptanceTestAddIgnoresNumbersOverOneThousand, AddThousandOneYieldsZero)
{
	EXPECT_EQ( 0, add("1001") );
}

TEST(AcceptanceTestAddIgnoresNumbersOverOneThousand, AddThousandTwoHundredTwentyTwoYieldsZero)
{
	EXPECT_EQ( 0, add("2222") );
}

TEST(AcceptanceTestAddIgnoresNumbersOverOneThousand, AddExpressionWithLargeAndSmallNumbers)
{
	EXPECT_EQ( 2, add("2,1001") );
}

TEST(AcceptanceTestAddIgnoresNumbersOverOneThousand, OneThousandIsWithinBoundary)
{
	EXPECT_EQ( 1000, add("1000") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[***]\n1***2***3") );
}

TEST(AcceptanceTestDelimiters, AsteriskAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[*]\n1*2*3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterUsingDifferentSymbol)
{
	EXPECT_EQ( 6, add("//[abc]\n1abc2abc3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterStartingWithComma)
{
	EXPECT_EQ( 6, add("//[,**]\n1,**2,**3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterEndingWithComma)
{
	EXPECT_EQ( 6, add("//[**,]\n1**,2**,3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterWithCommaInMiddle)
{
	EXPECT_EQ( 6, add("//[*,*]\n1*,*2*,*3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterStartingWithNewline)
{
	EXPECT_EQ( 6, add("//[\n**]\n1\n**2\n**3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterEndingWithNewline)
{
	EXPECT_EQ( 6, add("//[**\n]\n1**\n2**\n3") );
}

TEST(AcceptanceTestDelimiters, MulticharacterDynamicallyDeclaredDelimiterWithNewlineInMiddle)
{
	EXPECT_EQ( 6, add("//[*\n*]\n1*\n*2*\n*3") );
}

TEST(AcceptanceTestDelimiters, SlashAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[/]\n1/2/3") );
}

TEST(AcceptanceTestDelimiters, DoubleSlashAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[//]\n1//2//3") );
}

TEST(AcceptanceTestDelimiters, DoubleSlashLeftBracketAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[//[]\n1//[2//[3") );
}

TEST(AcceptanceTestDelimiters, RightBracketAsMulticharacterDynamicallyDeclaredDelimiter)
{
	EXPECT_EQ( 6, add("//[]]\n1]2]3") );
}

TEST(AcceptanceTestDelimiters, AsteriskAndPercentAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[*][%]\n1*2%3") );
}

TEST(AcceptanceTestDelimiters, PercentAsLoneMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[%]\n1%2%3") );
}

TEST(AcceptanceTestDelimiters, EmptyMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[]\n1,2,3") );
}

TEST(AcceptanceTestDelimiters, DoubleAsteriskAndDoublePercentAsDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[**][%%]\n1**2%%3") );
}

TEST(AcceptanceTestDelimiters, LeftBracketRightBracketAsDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[[]]\n1[]2[]3") );
}

TEST(AcceptanceTestDelimiters, TwoEmptyMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[][]\n1,2,3") );
}

TEST(AcceptanceTestDelimiters, ACommaZAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[A,Z]\n1A,Z2A,Z3") );
}

TEST(AcceptanceTestDelimiters, CommaAZAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[,AZ]\n1,AZ2,AZ3") );
}

TEST(AcceptanceTestDelimiters, AZCommaAsMultipleDynamicallyDeclaredDelimiters)
{
	EXPECT_EQ( 6, add("//[AZ,]\n1AZ,2AZ,3") );
}

