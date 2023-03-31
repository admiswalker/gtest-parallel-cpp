#include "../gtest_parallel/test_main.hpp"
#include "../src/example_math.cpp"


TEST(example_math, plus_a_b){
    ASSERT_EQ(plus_a_b(1, 2), 3);
}
TEST(example_math, minus_a_b){
    ASSERT_EQ(minus_a_b(2, 1), 1);
}
TEST(example_math, multi_a_b){
    ASSERT_EQ(multi_a_b(2, 3), 6);
}


EXECUTE_TESTS();

