#include <iostream>
#include <fstream>
#include "cmath"

#define LINE_END "\n"

void create_c_file();
std::string get_file_content();
std::string get_header();
std::string get_method();
std::string get_main();

std::string get_main(){
    std::string res;
    res="int main(){";
    res.append(LINE_END);
    res.append("    double a;");
    res.append(LINE_END);
    res.append("    double b;");
    res.append(LINE_END);
    res.append("    double c;");
    res.append(LINE_END);
    res.append("    klee_make_symbolic(&a, sizeof(a), \"a\");");
    res.append(LINE_END);
    res.append("    klee_make_symbolic(&b, sizeof(b), \"b\");");
    res.append(LINE_END);
    res.append("    klee_make_symbolic(&c, sizeof(c), \"c\");");
    res.append(LINE_END);
    res.append("    g(a,b,c);");
    res.append(LINE_END);
    res.append("}");
    res.append(LINE_END);
    return res;
}

std::string get_method(){
    std::string res;
    res="int g(double x0,double x1,double x2){";
    res.append(LINE_END);
    res.append("    if(1-(x0+x1+x2)>0){return 1;}");
    res.append(LINE_END);
    res.append("    else{return -1;}");
    res.append(LINE_END);
    res.append("}");
    res.append(LINE_END);
    return res;
}

std::string get_header(){
    std::string res;
    res="#include <klee/klee.h>";
    res.append(LINE_END);
    res.append("#include \"math.h\"");
    res.append(LINE_END);
    return res;
}

std::string get_file_content(){
    std::string res;
    res=get_header();
    res.append(get_method());
    res.append(get_main());
    return res;
}

void create_c_file(){
    std::ofstream fs;
    fs.open("test2.c");
    if(fs.is_open()){
        std::string res(get_file_content());
        fs<<res;
        std::cout<<"Success input"<<std::endl;
    }
    else{
        std::cout<<"Failed";
    }
    fs.close();
}

int main()
{
    create_c_file();
    return 0;
}
