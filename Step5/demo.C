// Example ROSE Translator: used within ROSE/tutorial

#include "rose.h"
#include<SgNodeHelper.h>
#include<sstream>
using namespace std;
using namespace rose;
using namespace SageBuilder;
using namespace SageInterface;

// Typical main function for ROSE translator

void insertIntoSwitch(SgStatement* parent);
void insertIntoIf(SgStatement* parent);
void travel(SgStatement* statement);
int containDouble(SgNode* parent);
void print(SgNode* printNode);
bool isMeet(SgDeclarationStatementPtrList& list,int index);
vector<SgFunctionDeclaration*> filter(SgDeclarationStatementPtrList& list);
string NTS(SgNode* node){
	return SgNodeHelper::nodeToString(node);
}

bool isMeet(SgDeclarationStatementPtrList& list,int index){
	string node=SgNodeHelper::nodeToString(list[index]);
	if(list[index]->get_file_info()->isCompilerGenerated()==false&&node[0]!='_'&&node[1]!='_'&&isSgFunctionDeclaration(list[index])!=NULL&&isSgFunctionDeclaration(list[index])->get_definition()!=NULL&&isSgFunctionDeclaration(list[index])->get_definition()->get_body()!=NULL){
		return true;
	}else{
		return false;
	}
}


vector<SgFunctionDeclaration*> filter(SgDeclarationStatementPtrList& list){
	vector<SgFunctionDeclaration*> myList;
	int size=list.size();
	int index;
	bool start=false;
	for(index=size-1;index>=0;index--){
		string node=SgNodeHelper::nodeToString(list[index]);
		if(list[index]->get_file_info()->isCompilerGenerated()==false&&node.find("operator")==-1){
			if(node.size()<2&&isSgFunctionDeclaration(list[index])!=NULL&&isSgFunctionDeclaration(list[index])->get_definition()!=NULL&&isSgFunctionDeclaration(list[index])->get_definition()->get_body()!=NULL){
				start=true;
				myList.push_back(isSgFunctionDeclaration(list[index]));
			}else if(node[0]!='_'&&node[1]!='_'&&isSgFunctionDeclaration(list[index])&&isSgFunctionDeclaration(list[index])->get_definition()!=NULL&&isSgFunctionDeclaration(list[index])->get_definition()->get_body()!=NULL){
				start=true;
				myList.push_back(isSgFunctionDeclaration(list[index]));
			}		
		}else{
			if(start){
				break;
			}
		}
	}
	std::cout << myList.size() << std::endl;
	for(index=0;index<myList.size();index++){
		print(myList[index]);
	}
	return myList;
}

void print(SgNode* printNode){
	if(printNode==NULL){
		std::cout << "NULL" << std::endl;
	}else{
		std::cout << NTS(printNode) << std::endl;
	}
}

int containDouble(SgNode* parent){
	int number=SgNodeHelper::numChildren(parent);
	std::vector<SgNode*> list=parent->get_traversalSuccessorContainer();
	int index;
	for(index=0;index<number;index++){
		if(isSgDoubleVal(list[index])){
			return 1;
		}
		//std::cout << list[index]->sage_class_name() << " ";
		//print(list[index]);
	}
	return 0;
}

SgStatement* build(int line,string stmt) {
	while(stmt.find(" ") != -1) {
		stmt.replace(stmt.find(" "),1,"");
	}
	stringstream ss;
	ss << "std::cout<<\"line:" << (line-1) << ",statement:" << stmt << ",bool:\"<<(" << stmt << ")<<std::endl";
	string s;
	ss >> s;
	SgExpression *exp = buildVarRefExp(s);
	return buildExprStatement(exp);
}

void insertIntoIf(SgStatement* parent){
	SgStatement* statement;
	string info;
	info=NTS(SgNodeHelper::getFirstChild(isSgIfStmt(parent)));
	info=info.substr(0,info.size()-1);
	statement=getFirstStatement(isSgBasicBlock(SgNodeHelper::getTrueBranch(isSgIfStmt(parent))));
	insertStatementBefore(statement,build(statement->get_file_info()->get_line(),info));//insertBefore
	statement=SageInterface::getNextStatement(statement);
	travel(statement);
	if(SgNodeHelper::getFalseBranch(isSgIfStmt(parent))){
		SgStatement* statement2;
		if(isSgIfStmt(SgNodeHelper::getFalseBranch(isSgIfStmt(parent)))){
			info=NTS(SgNodeHelper::getFirstChild(SgNodeHelper::getFalseBranch(isSgIfStmt(parent))));
			info=info.substr(0,info.size()-1);
			statement2=getFirstStatement(isSgBasicBlock(SgNodeHelper::getTrueBranch(SgNodeHelper::getFalseBranch(isSgIfStmt(parent)))));
			insertStatementBefore(statement2,build(statement2->get_file_info()->get_line(),info));//insertBefore
			statement2=SageInterface::getNextStatement(statement2);
		}else{
			statement2=getFirstStatement(isSgBasicBlock(SgNodeHelper::getFalseBranch(isSgIfStmt(parent))));
		}
		travel(statement2);
	}
}

void insertIntoSwitch(SgStatement* parent){
	SgStatement* statement=getFirstStatement(isSgBasicBlock((isSgSwitchStatement(parent))->get_body()));
	string info=NTS(SgNodeHelper::getFirstChild(parent));
	info=info.substr(0,info.size()-1);
	while(statement!=NULL){
		if(isSgCaseOptionStmt(statement)){
			SgStatement* innerStatement=isSgStatement(SgNodeHelper::getFirstChild((isSgCaseOptionStmt(statement))->get_body()));
			if(innerStatement!=NULL){
				string temp="";
				temp.append(info);
				temp.append("==");
				temp.append(NTS((isSgCaseOptionStmt(statement))->get_key()));
				insertStatementBefore(innerStatement,build(innerStatement->get_file_info()->get_line(),temp));
			}
			travel(innerStatement);
		}else if(isSgDefaultOptionStmt(statement)){
			SgStatement* innerStatement=getFirstStatement(isSgBasicBlock(isSgDefaultOptionStmt(statement)->get_body()));
			travel(innerStatement);
		}
		statement=SageInterface::getNextStatement(statement);
	}
}

void travel(SgStatement* statement){
	while(statement!=NULL){
		if(isSgBasicBlock(statement)){
			travel(isSgStatement(SgNodeHelper::getFirstChild(isSgBasicBlock(statement))));
		}else if(SgNodeHelper::isCondStmt(statement)){
			if(isSgIfStmt(statement)){
				insertIntoIf(statement);
			}else if(isSgSwitchStatement(statement)){
				insertIntoSwitch(statement);
			}else if(isSgForStatement(statement)){
				travel(getFirstStatement(isSgBasicBlock(isSgForStatement(statement)->get_loop_body())));
			}else if(isSgWhileStmt(statement)){
				travel(getFirstStatement(isSgBasicBlock(isSgWhileStmt(statement)->get_body())));
			}else if(isSgDoWhileStmt(statement)){
				travel(getFirstStatement(isSgBasicBlock(isSgDoWhileStmt(statement)->get_body())));
			}
		}else if(isSgTryStmt(statement)){
			travel(getFirstStatement(isSgBasicBlock(isSgTryStmt(statement)->get_body())));
			int size=(isSgTryStmt(statement))->get_catch_statement_seq_root()->get_traversalSuccessorContainer().size();
			int index=0;
			for(index=0;index<size;index++){
				travel(getFirstStatement(isSgBasicBlock(isSgCatchOptionStmt((isSgTryStmt(statement))->get_catch_statement_seq_root()->get_traversalSuccessorContainer()[index])->get_body())));
			}
		}else if(isSgCatchOptionStmt(statement)){
			travel(isSgStatement(SgNodeHelper::getFirstChild(isSgBasicBlock(statement))));
		}
		statement=SageInterface::getNextStatement(statement);
	}
}

int main( int argc, char * argv[] )
   {
  // Initialize and check compatibility. See rose::initialize
	ROSE_INITIALIZE;

  // Build the AST used by ROSE
	SgProject* project = frontend(argc,argv);
	
	vector<SgFunctionDeclaration*> list=filter(getFirstGlobalScope(project)->get_declarations());
	int size=list.size();
	int index;
	SgStatement* statement;
	for(index=0;index<size;index++){
		if(list[index]->get_definition()!=NULL){
			SgBasicBlock* body=list[index]->get_definition()->get_body();
			pushScopeStack(body);
			if(isSgScopeStatement(topScopeStack())){
				statement=getFirstStatement(topScopeStack());
				travel(statement);
			}
			popScopeStack();
		}else{
			print(list[index]);
		}
	}
     return backend(project);
   }


