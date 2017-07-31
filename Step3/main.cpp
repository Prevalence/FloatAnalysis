#include <iostream>
#include <fstream>
#include <sstream>
#include "cmath"

#define LINE_END "\n"

void create_c_file();
std::string get_file_content();
std::string get_header();

std::string get_g_function();
std::string get_main();
std::string get_expression();
std::string get_vars();
std::string get_declare();
std::string get_make_symbolic();
std::string get_param(std::string gap);
std::string get_log_function();
std::string get_vars_input();

std::string get_vars_input(){

    std::string res(get_vars());
    size_t i=0;
    while((i = res.find(':', i)) != std::string::npos) {
        res.replace(i, 1, " ");
        i += 1;
    }
    return res;
}

std::string get_log_function(){
    std::string res;

    res="void expr_log(char* expr,"+get_vars_input()+"){";
    res.append(LINE_END);
    res.append("    FILE *fp;");
    res.append(LINE_END);
    res.append("    fp=fopen(\"log\",\"a\");");
    res.append(LINE_END);

    std::istringstream iss(get_param(" "));

    std::string str("");
    while(iss){
        std::string temp;
        iss>>temp;
        if(temp.length()>=1)
            str.append(temp+"=%f ");
    }

    res.append("    fprintf(fp,\"%s "+
                       str+
                       "\",expr,"+
                       get_param(",")+
                       ");");

    res.append(LINE_END);
    res.append("    fclose(fp);");
    res.append(LINE_END);

    res.append("}");
    res.append(LINE_END);
    return res;
}

std::string get_param(std::string gap){
    std::string res("");
    std::string store("");//for duplicate
    std::ifstream fs("C:\\Users\\Jerry\\CLionProjects\\Composer\\expression.txt");
    if(fs){
        std::string line;
        while(std::getline(fs,line)){
            std::istringstream iss(line);
            std::string temp;
            iss>>temp;
            while(iss){
                iss>>temp;
                if (store.find(temp) == std::string::npos) {
                    store.append(temp+" ");

                    temp=temp.substr(temp.find(":")+1);
                    if(res.length()>=1){
                        res.append(gap);
                    }
                    res.append(temp);
                }
            }
        }
    }
    return res;
}

std::string get_make_symbolic(){
    std::string res("");
    std::string store("");//for duplicate
    std::ifstream fs("C:\\Users\\Jerry\\CLionProjects\\Composer\\expression.txt");
    if(fs){
        std::string line;
        while(std::getline(fs,line)){
            std::istringstream iss(line);
            std::string temp;
            iss>>temp;
            while(iss){
                iss>>temp;
                if (store.find(temp) == std::string::npos) {
                    store.append(temp+" ");

                    temp=temp.substr(temp.find(":")+1);
                    res.append("    klee_make_symbolic(&"+temp+", sizeof("+temp+"), \""+temp+"\");");
                    res.append(LINE_END);
                }
            }
        }
    }
    return res;
}

std::string get_declare(){
    std::string res;
    std::ifstream fs("C:\\Users\\Jerry\\CLionProjects\\Composer\\expression.txt");
    if(fs){
        std::string line;
        while(std::getline(fs,line)){
            std::istringstream iss(line);
            std::string temp;
            iss>>temp;
            while(iss){
                iss>>temp;
                if(temp.find(':')!=std::string::npos) {//in case newline of iss makes temp=<type> name rather than <type>:name;
                    temp.replace(temp.find(":"), 1, " ");
                    if (res.find(temp) == std::string::npos) {
                        res.append("    ");
                        res.append(temp);
                        res.append(";");
                        res.append(LINE_END);
                    }
                }
            }
        }
    }
    return res;
}

std::string get_vars(){
    std::string res("");
    std::ifstream fs("C:\\Users\\Jerry\\CLionProjects\\Composer\\expression.txt");
    if(fs){
        std::string line;
        while(std::getline(fs,line)){
            std::istringstream iss(line);
            std::string temp;
            iss>>temp;
            while(iss){
                iss>>temp;
                if(res.find(temp) == std::string::npos){
                    if(res.length()>=1){
                        res.append(",");
                    }
                    res.append(temp);
                }
            }
        }
    }
    return res;
}

std::string get_expression(){
    std::string res("");
    std::ifstream fs("C:\\Users\\Jerry\\CLionProjects\\Composer\\expression.txt");
    if(fs){
        std::string line;
        while(std::getline(fs,line)) {
            std::istringstream iss(line);
            std::string expr;
            iss >> expr;
            int pos = expr.find_first_of('=');
            expr = expr.substr(pos + 1);

            if (res.length() >= 1) {
                res.append("    else if(1-" + expr + ">0){ \n"
//                        "        expr_log(\"" + expr + "\"," + get_param(",") + ");\n"
                                   "        return 1;\n"
                                   "    }");
            }else{
                res.append("    if(1-" + expr + ">0){ \n"
//                        "        expr_log(\"" + expr + "\"," + get_param(",") + ");\n"
                                   "        return 1;\n"
                                   "    }");
            }

            res.append(LINE_END);
        }
        res.append("    return -1;");
        res.append(LINE_END);
    }
    return res;
}

std::string get_main(){
    std::string res;
    res="int main(){";
    res.append(LINE_END);
    res.append(get_declare());
    res.append(get_make_symbolic());
    res.append(LINE_END);
    res.append("    return g("+get_param(",")+");");
    res.append(LINE_END);
    res.append("}");
    res.append(LINE_END);
    return res;
}

std::string get_g_function(){
    std::string res;
    res="int g(";

    std::string temp(get_vars_input());
    res.append(temp);
    res.append("){");
    res.append(LINE_END);
    res.append(get_expression());
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
    res.append("#include \"stdio.h\"");
    res.append(LINE_END);
    return res;
}

std::string get_file_content(){
    std::string res;
    res=get_header();
    res.append(LINE_END);
    res.append(get_log_function());
    res.append(LINE_END);
    res.append(get_g_function());
    res.append(LINE_END);
    res.append(get_main());
    return res;
}

void create_c_file(){
    std::ofstream fs;
    fs.open("C:\\Users\\Jerry\\CLionProjects\\Composer\\test1.c");
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
//    std::cout<<
//             get_param();
    return 0;
}