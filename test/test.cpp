#include <string>
#include "gtest/gtest.h"

#include "String_Calculator.h"


TEST(DefaultConstructor, CanDefaultConstruct)
{
	String_Calculator calculator;
}

TEST(Add, MethodHasSignatureIntString)
{
	int (String_Calculator::* the_method)(const std::string &) const = String_Calculator::add;
}
