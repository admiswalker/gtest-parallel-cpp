#include <iostream>
#include <vector>

void execute_test(std::vector<std::string>& ret_vErr, const std::string& exe_path){
    
    int ret = system(exe_path.c_str());
    printf("\n");
    
    if(ret!=0){
        ret_vErr.push_back(exe_path);
    }
}
void print_error_files(const std::string& base_path, const std::vector<std::string>& vErr){
    std::string red   = "\u001b[31m";
    std::string reset = "\u001b[0m";
    
    if(vErr.size()!=0){ printf("%s %ld test files return failure.\n", (red+"[  ERROR   ]"+reset).c_str(), vErr.size()); }
    for(uint i=0; i<vErr.size(); ++i){
        printf("  %s/%s\n", base_path.c_str(), vErr[i].c_str());
    }
    if(vErr.size()!=0){ printf("\n"); }
}


int main(int argc, char** argv){
    printf("\n");
    printf("+---------------------------------------------------+\n");
    printf("|                                                   |\n");
    printf("|    Welcome to the CI/CD sample with googletest    |\n");
    printf("|                                                   |\n");
    printf("+---------------------------------------------------+\n");
    printf("\n");

    std::string base_path = "./tmpMake/test";
    std::vector<std::string> vErr;
    

    // Testing binaries
    execute_test(vErr, base_path+"/"+"example_math.exe");
    execute_test(vErr, base_path+"/"+"example_strEdit.exe");
    

    print_error_files(base_path, vErr);
    if(vErr.size()!=0){ return -1; }
    
    return 0;
}
