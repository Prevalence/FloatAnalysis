//============================================================================
// Name        : analyze.cc
// Author      : 
// Version     :
// Copyright   : Your copyright notice
//============================================================================
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void checking(int);
string value[5];
string var[5];
int test_num = 0;

int main() {
    char buffer[256];
    ifstream rFile("result.txt");
    if (!rFile.is_open()) {
        cout << "Error opening file";
    }
    while (!rFile.eof()) {

        rFile.getline (buffer,100);
	if(buffer[0]!='\0'){        
	int idx = 0;
        int sequence=0;
        while(buffer[idx]!='\0'){

        	if(buffer[idx]=='{'){ //get the variable
        		idx++;
        		string temp = "";
        		while( buffer[idx] != ' ' ){
        			temp += buffer[idx];
        			idx++;
        		}
        		var[sequence]=temp;
        		//cout<<var[sequence]<<" ";
        	}
        	//END find the variable
        	if(buffer[idx]==':'){//get the variable's value
        		idx = idx + 2;
        		string temp = "";
        		while(buffer[idx] != '}'){
        			temp += buffer[idx];
        			idx++;
        		}
        		value[sequence]= temp;
        		//cout<<(value[sequence])<<" ";
        		sequence++;
        	}
        	//END find the value
        	idx++;

        }
        //cout<<sequence<<endl;
        checking(sequence);
	}
    }
    return 0;
}
//out to file
void checking(int squence){
	char buff[256];
	string file_name;
	string test_name = "./checking/test";
	string num;
	stringstream sm;
	sm<<test_name<<test_num<<".cc";
	sm>>file_name;
	ofstream examplefile(file_name.c_str());
	    if (examplefile.is_open())
	    {
	    	test_num++;
	    	examplefile << "#include<iostream>\n";
	        examplefile << "using namespace std;\n";
			examplefile << "int main(){\n";
			for(int i=0;i<squence;i++){
				examplefile << "   double "<<var[i]<<" = "+value[i]+";\n";
			}
			ifstream conditionFile("conditions.txt");
			if (!conditionFile.is_open()) {
			        cout << "Error opening file";
			    }
			    while (!conditionFile.eof()) {
			    	conditionFile.getline(buff,256);
			    	examplefile<<buff<<"{\n";
			    	examplefile<<"		cout<<\" "<<file_name<<"\";\n";
			    	examplefile<<"	}\n";
			    }
			examplefile << "   return 0;\n";
			examplefile << "}\n";
	        	examplefile.close();
	    }else{
			cout<<"error"<<endl;
	    }
	    return;
	}





