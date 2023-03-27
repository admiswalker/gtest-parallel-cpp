#include <iostream>
#include <vector>

void execute_test(std::string& ret_err, const std::string& exe_path){
    
    int ret = system(exe_path.c_str());
    printf("\n");
    
    if(ret!=0){
        ret_err = exe_path;
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
template<typename T>
std::vector<T> rm_empty_vector(const std::vector<T>& vErr){
    std::vector<std::string> ret_vErr;
    
    for(uint i=0; i<vErr.size(); ++i){
        if(vErr[i].size()==0){ continue; }
        ret_vErr.push_back(vErr[i]);
    }
    
    return ret_vErr;
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
    std::vector<std::string> vExePath;
    

    // Testing binaries
    vExePath.push_back(base_path+"/"+"example_math.exe"   );
    vExePath.push_back(base_path+"/"+"example_strEdit.exe");
    std::vector<std::string> vErr(vExePath.size());
    
    #pragma omp parallel for schedule(guided)
    for(uint i=0; i<vExePath.size(); ++i){
        execute_test(vErr[i], vExePath[i]);
    }

    vErr = rm_empty_vector(vErr);
    print_error_files(base_path, vErr);
    if(vErr.size()!=0){ return -1; }
    
    return 0;
}
