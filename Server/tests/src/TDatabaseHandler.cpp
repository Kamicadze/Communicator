#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CDatabaseHandler.h"

TEST(UserCreationTest, DBHandler)
{
    CDatabaseHandler dbh;

    EXPECT_EQ(dbh.createUser("kami", "asd"), false);
    EXPECT_EQ(dbh.createUser("blah", "blah"), true);    
}

TEST(UserDeletionTest, DBHandler)
{
    CDatabaseHandler dbh;

    EXPECT_EQ(dbh.deleteUser("blah", "blah"), true);
}

TEST(AuthenticationTest, DBHandler)
{
    CDatabaseHandler dbh;
    
    EXPECT_EQ(dbh.authenticate("kami", "123456"), true);
    EXPECT_EQ(dbh.authenticate("blah", "blah"), false);
    EXPECT_EQ(dbh.authenticate("kami", "blah"), false);
    EXPECT_EQ(dbh.authenticate("blah", "123456"), false);

}

TEST(UserFindingTest, DBHandler)
{
    CDatabaseHandler dbh;
    EXPECT_EQ(dbh.findUser("kami"), true);
    EXPECT_EQ(dbh.findUser("blah"), false);
}


int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
