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

TEST(Add, OneTwoYieldsThree)
{
	EXPECT_EQ( 3, add("1,2") );
}

TEST(Add, TenTwentyYieldsThirty)
{
	EXPECT_EQ( 30, add("10,20") );
}
