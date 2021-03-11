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
