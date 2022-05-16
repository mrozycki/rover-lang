#include <gtest/gtest.h>

#include <lexer.h>
#include <token.h>

class lexer_test : public ::testing::Test {
protected:
    virtual void SetUp() { }

    virtual void TearDown() { }
};

TEST_F(lexer_test, test_lexer_peek) {
    std::istringstream input("+");
    rover::lexer lexer(input);

    auto token = lexer.peek();
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::PLUS);
    EXPECT_FALSE(token->payload);
}

TEST_F(lexer_test, test_lexer_consume) {
    std::istringstream input("+");
    rover::lexer lexer(input);

    auto token = lexer.consume();
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::PLUS);
    EXPECT_FALSE(token->payload);

    token = lexer.peek();
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::END_OF_FILE);
    EXPECT_FALSE(token->payload);
}

TEST_F(lexer_test, test_lexer_consume_if) {
    std::istringstream input("+");
    rover::lexer lexer(input);

    auto token = lexer.consume_if(rover::token_type::PLUS);
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::PLUS);
    EXPECT_FALSE(token->payload);

    token = lexer.peek();
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::END_OF_FILE);
    EXPECT_FALSE(token->payload);
}

TEST_F(lexer_test, test_lexer_consume_if_not) {
    std::istringstream input("+");
    rover::lexer lexer(input);

    auto token = lexer.consume_if(rover::token_type::MINUS);
    ASSERT_FALSE(token);

    token = lexer.peek();
    ASSERT_TRUE(token);
    EXPECT_EQ(token->type, rover::token_type::PLUS);
    EXPECT_FALSE(token->payload);
}
