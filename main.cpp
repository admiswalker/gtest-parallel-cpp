#include <iostream>
#include <vector>
#include <numeric>
#include <omp.h>


namespace console_color{
    std::string black   = "\u001b[30m";
    std::string red     = "\u001b[31m";
    std::string green   = "\u001b[32m";
    std::string yellow  = "\u001b[33m";
    std::string blue    = "\u001b[34m";
    std::string magenta = "\u001b[35m";
    std::string cyan    = "\u001b[36m";
    std::string white   = "\u001b[37m";
    
    std::string reset   = "\u001b[0m";
}

int system_stdout_stderr(std::string& ret, const std::string& cmd){
    std::string cmd_out_err = cmd + std::string(R"( 2>&1)"); // 2>&1: redirecting stderr to stdout.
    FILE* fp=popen(cmd_out_err.c_str(), "r");
    if(fp==NULL){ printf("popen() was failed.\n"); return -1; }
    
    char buf[1024];
    while(fgets(buf, 1024, fp) != NULL){ ret+=buf; }
    
    return pclose(fp);
}
void replaced(std::string& replace_target, const std::string& search_word, const std::string& replace_word){
    std::string::size_type pos(replace_target.find(search_word));
    
    while(pos!=std::string::npos){
        replace_target.replace(pos, search_word.length(), replace_word);
        pos = replace_target.find(search_word, pos+replace_word.length());
    }
}
int count_word_num(const std::string& count_target, const std::string& search_word){
    int num=0;
    
    std::string::size_type pos(count_target.find(search_word));
    
    while(pos!=std::string::npos){
        ++num;
        pos = count_target.find(search_word, pos+search_word.length());
    }

    return num;
}
std::vector<std::string> splitByLine(const char* str){

    std::vector<std::string> ret;
    
    std::string buf;
    for(uint r=0; str[r]!=0; ++r){ // r: read place
        buf.clear();
        for(; str[r]!=0; ++r){
            if(str[r]==0x0A){ break; }                        // Uinx
            if(str[r]==0x0D && str[r+1]==0x0A){ ++r; break; } // Windows
            buf += str[r];
        }
        ret.push_back(std::move(buf));
    }
    
    return ret;
}
std::vector<std::string> splitByLine(const std::string& str){
    return splitByLine(str.c_str());
}
std::vector<std::pair<std::string,std::string>> split_test_list(const std::string& s){
    std::vector<std::pair<std::string,std::string>> ret_v;
    std::vector<std::string> line = splitByLine(s);

    std::string first;
    for(uint i=0; i<line.size(); ++i){
        if(line[i].size()>=2 && line[i][0]==' ' && line[i][1]==' '){
            std::pair<std::string,std::string> tmp;
            tmp.first  = first;
            tmp.second = &line[i][2]; // rm the head "  " at test_name
            ret_v.push_back(tmp);
        }else{
            first = line[i];
        }
    }
    return ret_v;
}
int get_test_list(std::vector<std::pair<std::string,std::string>>& ret_v, const std::string& exe_path){
    
    std::string ret_str;
    int ret = system_stdout_stderr(ret_str, exe_path+" --gtest_list_tests");
    if(ret!=0){ return ret; }
    
    ret_v = split_test_list(ret_str);
    
    return ret;
}
void execute_test__old(int& ret_test_num, int& ret_pass_num, int& ret_err_num, std::string& ret_err_file_path, std::string& ret_str, const std::string& exe_path, const std::string& google_test_option){
    
    std::vector<std::pair<std::string,std::string>> vec_TCN_TN;
    int ret = get_test_list(vec_TCN_TN, exe_path);
    if(ret!=0){ ret_err_file_path=exe_path; }
    
    //#pragma omp parallel for schedule(guided)
    for(uint i=0; i<vec_TCN_TN.size(); ++i){
        std::string testCaseName = vec_TCN_TN[i].first;
        std::string testName     = vec_TCN_TN[i].second;
        
        std::string s;
        int ret = system_stdout_stderr(s, exe_path+" "+google_test_option+" --gtest_filter="+testCaseName+testName);
        if(ret==0){ ++ret_pass_num; }else{ ++ret_err_num; }
        
        std::vector<std::string> vStr = splitByLine(s);

        uint ri=0;
        for(; ri<vStr.size(); ++ri){
            if(vStr[ri].find("[ RUN      ]")==std::string::npos){ continue; }
            ret_str += vStr[ri]+"\n";
            ++ri;
            break;
        }
        for(; ri<vStr.size(); ++ri){
            ret_str += vStr[ri]+"\n";
            if(vStr[ri].find("[       OK ]")!=std::string::npos){ break; }
            if(vStr[ri].find("[  FAILED  ]")!=std::string::npos){ break; }
        }
    }
    ret_test_num=vec_TCN_TN.size();
    
    if(ret_err_num!=0){ ret_err_file_path=exe_path; }
}
void print_pass(int passNum, int fileNum){
    printf("%s\n", (console_color::green+"[----------]"+console_color::reset).c_str());
    printf("%s %d tests from %d test files ran.\n", (console_color::green+"[==========]"+console_color::reset).c_str(), passNum, fileNum);
    printf("%s %d tests.\n", (console_color::green+"[  PASSED  ]"+console_color::reset).c_str(), passNum);
    return ;
}
void print_failure(const std::string& base_path, const std::vector<std::string>& vErrPath, int errNum){
    if(vErrPath.size()!=0){
        printf("%s %d test%s of %ld test executor%s, listed below:\n", (console_color::red+"[  FAILED  ]"+console_color::reset).c_str(), errNum, (errNum==1?"":"s"), vErrPath.size(), (vErrPath.size()==1?"":"s"));
    }
    
    for(uint i=0; i<vErrPath.size(); ++i){
        printf("%s %s\n", (console_color::red+"[  FAILED  ]"+console_color::reset).c_str(), vErrPath[i].c_str());
    }
}
std::vector<int> rm_zero_vector(const std::vector<int>& v){
    std::vector<int> ret_v;
    
    for(uint i=0; i<v.size(); ++i){
        if(v[i]==0){ continue; }
        ret_v.push_back(v[i]);
    }
    
    return ret_v;
}
template<typename T>
std::vector<T> rm_empty_vector(const std::vector<T>& v){
    std::vector<std::string> ret_v;
    
    for(uint i=0; i<v.size(); ++i){
        if(v[i].size()==0){ continue; }
        ret_v.push_back(v[i]);
    }
    
    return ret_v;
}
int sum(const std::vector<int>& v){
    return std::accumulate(v.begin(), v.end(), 0);
}

struct execution_settings{
    std::string exePath;
    std::string testCaseName;
    std::string testName;
};
std::vector<struct execution_settings> make_execution_list(const std::vector<std::string>& vExePath){
    std::vector<struct execution_settings> exeList;
    std::vector<int> vErrNum(vExePath.size());
    std::vector<std::vector<struct execution_settings>> v_exeList(vExePath.size());
    
    #pragma omp parallel for schedule(guided)
    for(uint i=0; i<vExePath.size(); ++i){
        std::vector<std::pair<std::string,std::string>> ret_v;
        int ret = get_test_list(ret_v, vExePath[i]);
        if(ret!=0){ vErrNum[i]=1; }
        
        std::vector<struct execution_settings> ret_exeList(ret_v.size());
        for(uint ri=0; ri<ret_exeList.size(); ++ri){
            ret_exeList[ri].exePath      = vExePath[i];
            ret_exeList[ri].testCaseName = ret_v[ri].first;
            ret_exeList[ri].testName     = ret_v[ri].second;
        }
        v_exeList[i] = std::move(ret_exeList);
    }

    int err_num = sum(vErrNum);
    if(err_num!=0){ printf("make_execution_list() is failed.\n"); return exeList; }
    
    for(uint i=0; i<v_exeList.size(); ++i){
        exeList.insert(exeList.end(), v_exeList[i].begin(), v_exeList[i].end());
    }
    
    return exeList;
}
void execute_tests(int& tf_end, int& failed, struct execution_settings& failedTest, std::string& ret_str, struct execution_settings& exeList, const std::string& google_test_option){

    std::string s;
    int ret = system_stdout_stderr(s, exeList.exePath+" "+google_test_option+" --gtest_filter="+exeList.testCaseName+exeList.testName);
    if(ret!=0){
        failed = 1;
        failedTest = exeList;
    }
    
    std::vector<std::string> vStr = splitByLine(s);
    uint ri=0;
    for(; ri<vStr.size(); ++ri){
        if(vStr[ri].find("[ RUN      ]")==std::string::npos){ continue; }
        ret_str += vStr[ri]+"\n";
        ++ri;
        break;
    }
    for(; ri<vStr.size(); ++ri){
        ret_str += vStr[ri]+"\n";
        if(vStr[ri].find("[       OK ]")!=std::string::npos){ break; }
        if(vStr[ri].find("[  FAILED  ]")!=std::string::npos){ break; }
    }

    tf_end=1;
}
void print_results(omp_lock_t& omp_lock, uint& i_end_num, const std::vector<int>& vEnd, const std::vector<std::string>& vRet){
    if(omp_test_lock(&omp_lock)==0){ return; } // return when the lock failed
    
    for(uint i=i_end_num; i<vEnd.size(); ++i){
        if(vEnd[i]==0){ break; }
        printf("%s", vRet[i].c_str());
        i_end_num = i+1;
    }
    
    omp_unset_lock(&omp_lock);
}
void print_results(uint& i_end_num, const std::vector<int>& vEnd, const std::vector<std::string>& vRet){
    
    for(uint i=i_end_num; i<vEnd.size(); ++i){
        if(vEnd[i]==0){ break; }
        printf("%s", vRet[i].c_str());
        i_end_num = i+1;
    }
}

int main(int argc, char** argv){
    printf("\n");
    printf("+---------------------------------------------------+\n");
    printf("|                                                   |\n");
    printf("|             Welcome to the Sample of              |\n");
    printf("|        Separate Compilation of googletest         |\n");
    printf("|                                                   |\n");
    printf("+---------------------------------------------------+\n");
    printf("\n");

    std::string base_path = "./tmpMake/test";
    std::string google_test_option = "--gtest_color=yes";
    
    
    // Testing binaries
    std::vector<std::string> vExePath;
    vExePath.push_back(base_path+"/"+"example_math.exe "   );
    vExePath.push_back(base_path+"/"+"example_strEdit.exe ");
    
    
    int fileNum = vExePath.size();
    std::vector<int> vTestNum(fileNum), vPassNum(fileNum), vErrNum(fileNum);
    std::vector<std::string> vErrPath(fileNum), vRetStr(fileNum);
    
    std::vector<struct execution_settings> exeList = make_execution_list(vExePath);
    for(uint i=0; i<exeList.size(); ++i){
        printf("%s %s%s\n", exeList[i].exePath.c_str(), exeList[i].testCaseName.c_str(), exeList[i].testName.c_str());
    }

    uint i_end_num=0;
    std::vector<int> vEnd(exeList.size(), 0);
    std::vector<int> vFailed(exeList.size(), 0);
    std::vector<struct execution_settings> vFailedTest(exeList.size());
    std::vector<std::string> vRet(exeList.size());
    
    omp_lock_t omp_lock;
    omp_init_lock(&omp_lock); // init
    #pragma omp parallel for schedule(guided)
    for(uint i=0; i<exeList.size(); ++i){
        execute_tests(vEnd[i], vFailed[i], vFailedTest[i], vRet[i], exeList[i], google_test_option);
        print_results(omp_lock, i_end_num, vEnd, vRet);
    }
    omp_destroy_lock(&omp_lock); // destroy
    print_results(i_end_num, vEnd, vRet); // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    
    //vErrPath = rm_empty_list(vFailedTest);
    
    //for(uint i=0; i<vRetStr.size(); ++i){ printf("%s\n", vRetStr[i].c_str()); }
    //print_pass(sum(vPassNum), fileNum);
    //print_failure(base_path, vErrPath, sum(vErrNum));
    printf("\n");
    
    if(vErrPath.size()!=0){ return -1; }
    return 0;
}
