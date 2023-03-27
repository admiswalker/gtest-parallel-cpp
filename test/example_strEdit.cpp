#include "test_main.hpp"
#include "../src/example_strEdit.cpp"


TEST(example, joint_a_b){
    ASSERT_STREQ(joint_a_b("a", "b").c_str(), "ab");
}
TEST(example, joint_a_b_with_x){
    ASSERT_STREQ(joint_a_b_with_x("a", "b", ", ").c_str(), "a, b");
}

EXECUTE_TESTS();

