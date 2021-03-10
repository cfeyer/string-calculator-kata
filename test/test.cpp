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

