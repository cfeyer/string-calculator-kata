#include <iostream>

#include "gtest/gtest.h"


TEST(SomeTest, SomeFailingCase)
{
	EXPECT_TRUE( false );
}


TEST(SomeTest, SomePassingCase)
{
	EXPECT_TRUE( true );
}

