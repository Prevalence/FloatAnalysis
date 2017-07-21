// Example ROSE Translator: used within ROSE/tutorial

#include "rose.h"
#include<SgNodeHelper.h>
#include<sstream>
using namespace std;
using namespace rose;
using namespace SageBuilder;
using namespace SageInterface;

// Typical main function for ROSE translator

int insertIntoSwitch(SgStatement* parent);
int insertIntoIf(SgStatement* parent);
void travel(SgStatement* statement);
string NTS(SgNode* node){
	return SgNodeHelper::nodeToString(node);
}


void print(SgNode* printNode){
	std::cout << NTS(printNode) << std::endl;
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

int insertIntoIf(SgStatement* parent){
	SgStatement* statement=getFirstStatement(isSgBasicBlock(SgNodeHelper::getTrueBranch(isSgIfStmt(parent))));
	string info=NTS(SgNodeHelper::getFirstChild(isSgIfStmt(parent)));
	info=info.substr(0,info.size()-1);
	insertStatementBefore(statement,build(statement->get_file_info()->get_line(),info));//insertBefore
	statement=SageInterface::getNextStatement(statement);
	travel(statement);
	if(SgNodeHelper::getFalseBranch(isSgIfStmt(parent))){
		SgStatement* statement2=getFirstStatement(isSgBasicBlock(SgNodeHelper::getTrueBranch(SgNodeHelper::getFalseBranch(isSgIfStmt(parent)))));
		info=NTS(SgNodeHelper::getFirstChild(SgNodeHelper::getFalseBranch(isSgIfStmt(parent))));
		info=info.substr(0,info.size()-1);
		insertStatementBefore(statement2,build(statement2->get_file_info()->get_line(),info));//insertBefore
		statement2=SageInterface::getNextStatement(statement2);
		travel(statement2);
	}
	return 0;
}

int insertIntoSwitch(SgStatement* parent){
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

	SgFunctionDeclaration* mainFunc= findMain(project);

  	SgBasicBlock* body= mainFunc->get_definition()->get_body();
  	pushScopeStack(body);
	SgStatement* statement = getFirstStatement(topScopeStack());
	travel(statement);
	popScopeStack();
	AstTests::runAllTests(project);
     return backend(project);
   }


