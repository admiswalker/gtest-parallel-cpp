# gtest-parallel-cpp

gtest-parallel-cpp is a the single header library to the separate Compilation and parallel execution of the [GoogleTest](https://github.com/google/googletest) written in C++ and OpenMP.

gtest-parallel-cpp は，[GoogleTest](https://github.com/google/googletest) を並列コンパイル・並列実行するための C++ で書かれたシングルヘッダライブラリです．

## Usage

1. Copy the `gtest_parallel` directory
2. Add the following header and execution definition to the test code written in Googletest.
   ```cpp
   #include "../gtest_parallel/test_main.hpp"
   ```
   ```cpp
   EXECUTE_TESTS(); // Write in the bottom of test code.
   ```
3. Compile your test code
4. Write the entry point of gtest-parallel-cpp and write the executing binary path.
   ```cpp
   #include "gtest_parallel/gtest_parallel.hpp"
   
   int main(int argc, char** argv){
       printf("\n");
       printf("+---------------------------------------------------+\n");
       printf("|                                                   |\n");
       printf("|             Welcome to the Sample of              |\n");
       printf("|    Separate Compilation and Parallel Execution    |\n");
       printf("|                 of the googletest                 |\n");
       printf("|                                                   |\n");
       printf("+---------------------------------------------------+\n");
       printf("\n");
   
       std::string base_path = "./tmpMake/test";
       
       // Testing binaries
       std::vector<std::string> vExePath;
       vExePath.push_back(base_path+"/"+"example_math.exe "   ); // replace your binary path
       vExePath.push_back(base_path+"/"+"example_strEdit.exe "); // replace your binary path
   
       int ret = gtest_parallel::run_tests(vExePath);
   
       return ret;
      }
   ```
5. Compile entry point of gtest-parallel-cpp and execute.  
   Note: Adding a `-fopenmp` option is required.

## Execution view

See GitHub Actions.

## Appendix

### Get TestCaseName and TestName in GoogleTest

```cpp
TEST(testCaseName, testName){
    const auto* test_info = testing::UnitTest::GetInstance()->current_test_info();
    printf("%s\n", test_info->test_case_name());
    printf("%s\n", test_info->name()          );
}
```
OR
```cpp
TEST(testCaseName, testName){
    printf("%s\n", test_info_->test_case_name());
    printf("%s\n", test_info_->name()          );
}
```

