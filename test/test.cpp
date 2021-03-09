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

TEST(Add, EmptyStringYieldsZero)
{
	String_Calculator calculator;
	const std::string str( "" );
	EXPECT_EQ( 0, calculator.add( str ) );
}

TEST(Add, OneYieldsOne)
{
	String_Calculator calculator;
	const std::string str( "1" );
	EXPECT_EQ( 1, calculator.add( str ) );
}
