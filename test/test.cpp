#include <string>
#include "gtest/gtest.h"

#include "String_Calculator.h"


TEST(DefaultConstructor, CanDefaultConstruct)
{
	String_Calculator calculator;
}

TEST(Add, SignatureCanTakeString)
{
	String_Calculator calculator;
	const std::string str;
	calculator.add( str );
}

TEST(Add, SignatureCanReturnInt)
{
	String_Calculator calculator;
	const std::string str;
	int result = calculator.add( str );
}

static int add( const std::string & str )
{
	String_Calculator calculator;
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
	String_Calculator calculator;
	calculator.get_called_count();
}

TEST(GetCalledCount, SignatureReturnsInt)
{
	String_Calculator calculator;
	int n = calculator.get_called_count();
}

void test_get_called_count( int number_times_called )
{
	String_Calculator calculator;

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
		Mock_Add_Observer() : call_count( 0 )
		{
		}

		int call_count;
};

TEST(AddObserver, CanInstantiate)
{
	Mock_Add_Observer observer;
}

TEST(AddObserver, CanPassToStringCalculatorConstructor)
{
	Mock_Add_Observer concrete_observer;
	Add_Observer_Interface & observer_interface( concrete_observer );
	String_Calculator calculator( observer_interface );
}

TEST(AddObserver, ObserverNotCalledBeforeAddCalled)
{
	Mock_Add_Observer observer;
	String_Calculator calculator( observer );

	EXPECT_EQ( 0, observer.call_count );
}

TEST(AddObserver, ObserverHasCallbackMethod)
{
	Mock_Add_Observer concrete_observer;
	Add_Observer_Interface & observer_interface( concrete_observer );

	std::string expression;
	int value = 0;

	observer_interface.add_occurred( expression, value );
}
