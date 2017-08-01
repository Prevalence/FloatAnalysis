#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream> 
#include <string>
#include <vector>
#include <map>
using namespace std;

void changeParseAndOut(string& s);
void move();
bool contains(char c);
string& containsvar(string& c);
bool containscompare(char c);
string& mapvar(string c);
bool containsbetter(char c);
bool containsop(char c);
void outToFile();
string judge(string s);
void out_to_checking();

vector<string> a;
vector<string> var;
vector<string> aftermove;
vector<string> afterchange;
typedef map<string,string> Var_map;
Var_map var_map;
Var_map::const_iterator it;

int compute()
{
	// read file line by line
	char buffer[256];
	char buffer1[256];
	bool flag=true;//flag to check weather is the condition parse
    ifstream examplefile("example.txt");
    if (! examplefile.is_open())
    {
        cout << "Error opening file"; exit (1);
    }
    while (!examplefile.eof())
    {
        examplefile.getline(buffer,100);
		for(int i=0,b=0;i<100;i++){
			if(buffer[i]==' '){}
			else{
				buffer1[b]=buffer[i];
				b++;
			}
		}
        if(flag){
			int mark=0;//mark the char I deal with
			stringstream ss;
			string str;
			ss<<buffer1;
			ss>>str;
			for(int i=0;i<str.length();i++){
				if(str.at(i)=='&'){
					string s = str.substr(mark,i-mark);
					mark = i+2;
					a.push_back(s);
					i++;
				}
			}
			string s = str.substr(mark,str.length()-mark);
			a.push_back(s);
			for(int m=0;m<a.size();m++){
				string& s1 = a[m];
			}
			flag=false;
		}else{
			if(buffer1[0]=='\0'){
			}else{
		  		 string s;
    	  		 stringstream ss;
		  		 ss<<buffer1;
		  		 ss>>s;
		 		 var.push_back(s);
			}
		}	
    }
	for(int m=0;m<var.size();m++){
		string varparse = var[m];
		string strId;
		string strVal;
		for(int i=0;i<varparse.length();i++){
			if(varparse.at(i)=='='){
				strId = varparse.substr(0,i);
				strVal = "("+varparse.substr(i+1,varparse.length()-i-1)+")";
			}
		}
		var_map.insert(map<string,string>::value_type(strId,strVal)); 
	}
	move();	
	for(int i=0;i<aftermove.size();i++){
		string sm= aftermove[i];
		changeParseAndOut(sm);
	}
	outToFile();
	out_to_checking();
    return 0;
}

void changeParseAndOut(string &s){
	char buffer2[200];
	char var[20] ="";
	int buf = 0;
	int va = 0;
	for(int i=0;i<200;i++){
		buffer2[i]='\0';
	}
	bool flag1 = false;// to mark a var is end;
	for(int i=0;i<s.length();i++){
		//every var begin with a letter;
		if(!flag1){
			if(containsbetter(s.at(i))){
				flag1=true;
				//means the begin of var
				var[va]=s.at(i);
				va++;
			}else{
				//no var and not begin;
				buffer2[buf]=s.at(i);
				buf++;
			}
		}else{
			if(containsop(s.at(i))){
				flag1=false;
				//means var end now var is the var name;
				stringstream ss;
				ss<<var;
				string s2;
				ss>>s2;
				string& val = mapvar(s2);
				for(int i=0;i<val.length();i++){
					buffer2[buf]=val.at(i);
					buf++;
				}
				for(int j=0;j<20;j++){
					var[j]='\0';				
				}
				va=0;
				buffer2[buf] = s[i];
				buf++;
			}else{
				var[va]=s.at(i);
				va++;
			}
		}
	}
	//now we get the parse in buffer
	string afters;
	stringstream sstream;
	sstream<<buffer2;
	sstream>>afters;
	afterchange.push_back(afters);
}

string& mapvar(string c){
    string& result = var_map[c];
    return result;
}

bool contains(char c){
	string s = "!-+*/()";
	return true;
}

void move(){
	for(int i=0;i<a.size();i++){
		string s = a[i];
		for(int m=0;m<s.length();m++){
			if(s.at(0)=='!'){
				//we need to remove !and () before move
				if(containscompare(s.at(m))){
					if(containscompare(s.at(m+1))){
						string sleft = "("+s.substr(2,m-2)+")";
						string sright = "("+s.substr(m+2,s.length()-m-3)+")";
						string condition = "("+sleft+"-"+sright+")"+s.at(m)+s.at(m+1)+"0";
						aftermove.push_back(condition);
						break;
					}else{
						string sleft = "("+s.substr(2,m-2)+")";
						string sright = "("+s.substr(m+1,s.length()-m-2)+")";
						string condition = "!(("+sleft+"-"+sright+")"+s.at(m)+"0"+")";
						aftermove.push_back(condition);	
						break;				
					}
				}
			}else{
				if(containscompare(s.at(m))){
					if(containscompare(s.at(m+1))){
						string sleft = "("+s.substr(0,m)+")";
						string sright = "("+s.substr(m+2,s.length()-m-2)+")";
						string condition = "("+sleft+"-"+sright+")"+s.at(m)+s.at(m+1)+"0";
						aftermove.push_back(condition);
						break;
					}else{
						string sleft = "("+s.substr(0,m)+")";
						string sright = "("+s.substr(m+1,s.length()-m-1)+")";
						string condition = "("+sleft+"-"+sright+")"+s.at(m)+"0";
						aftermove.push_back(condition);	
						break;				
					}
				}
			}
		}		
	}
	return;
}

bool containscompare(char c){
	string s="<>=";
	for(int i=0;i<s.length();i++){
		if(c==s.at(i))
		   return true;
	}
	return false;
}

bool containsbetter(char c){
	string s = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKZXCVBNML";
	for(int i=0;i<s.length();i++){
		if(c==s.at(i))
			return true;
	}
	return false;
}

bool containsop(char c){
	string s = "-+/*><=)";
	for(int i=0;i<s.length();i++){
		if(c==s.at(i)){
			return true;
		}
	}
	return false;
}

void outToFile(){
	//the write operate;
    ofstream examplefile("example.c");
    if (examplefile.is_open())
    {
		examplefile << "#include <klee/klee.h>\n";
        //examplefile << "using namespace std;\n";
		examplefile << "int main(){\n";
		examplefile << "   double result;"<<endl;
		for(it=var_map.begin();it!=var_map.end();it++){
			examplefile << "   double "<<it->first<<";\n";
			examplefile << "   klee_make_symbolic(&"+it->first+",sizeof("+it->first+"),\""+it->first+"\");\n";
		}
		
		for(int i=0;i<afterchange.size();i++){
			examplefile <<"   if("<<judge(afterchange[i])<<")"<<"\n";
			examplefile <<"      result = 0.0;"<<"\n";
		}
		examplefile << "   return 0;\n";
		examplefile << "}\n";
        	examplefile.close();
    }else{
		cout<<"error"<<endl;
    }
    return;
}

string judge(string s){
	string sleft;
	string a="";
	for(int i=0;i<s.length();i++){
		if(containscompare(s.at(i))){
			if(s.at(0)=='!'){
				sleft = s.substr(2,i-2);
			}else{
				sleft = s.substr(0,i);
			}
			cout<<sleft<<endl;			
			if(containscompare(s.at(i+1))){
				//two ops
				if(s.at(i)=='='){
					string result = a+"(("+sleft+"<0.001)&&("+sleft+">0))||(("+sleft+"<0)&&("+sleft+">-0.001))";			
					return result;
				}else if(s.at(i)=='<'){
					string result =a+"(("+sleft+"<0)&&("+sleft+">-0.001))";
					return result;
				}else{
					string result = a+"(("+sleft+"<0.001)&&("+sleft+">0))";
					return result;
				}
			}else{
				//one op
				if(s.at(i)=='<'){
					string result = a+"(("+sleft+"<0.001)&&("+sleft+">0))";
					return result;
				}else{
					string result =a+"(("+sleft+"<0)&&("+sleft+">-0.001))";
					return result;
				}
			}
		}
	}
	return a;
}

void out_to_checking(){
	//the write operate;
    ofstream examplefile("conditions.txt");
    if (examplefile.is_open())
    {
		
		for(int i=0;i<afterchange.size();i++){
			examplefile <<"   if("<<judge(afterchange[i])<<")"<<"\n";
		}

        	examplefile.close();
    }else{
		cout<<"error"<<endl;
    }
    return;
}


