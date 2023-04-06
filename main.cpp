#include "gtest_parallel/gtest_parallel.hpp"


int main(int argc, char** argv){
    printf("\n");
    printf("+---------------------------------------------------+\n");
    printf("|                                                   |\n");
    printf("|             Welcome to the Sample of              |\n");
    printf("|    Separate Compilation and Parallel Execution    |\n");
    printf("|                 of the GoogleTest                 |\n");
    printf("|                                                   |\n");
    printf("+---------------------------------------------------+\n");
    printf("\n");

    std::string base_path = "./tmpMake/test";
    
    // Testing binaries
    std::vector<std::string> vExePath;
    vExePath.push_back(base_path+"/"+"example_math.exe "   );
    vExePath.push_back(base_path+"/"+"example_strEdit.exe ");

    int ret = gtest_parallel::run_tests(vExePath);

    return ret;
}
