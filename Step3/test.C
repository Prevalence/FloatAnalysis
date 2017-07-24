#include "rose.h"
#include "DefUseAnalysis.h"
#include <string>
#include <iostream>
#include"SgNodeHelper.h"

using namespace std ;
void excute(SgProject* project);
void forOut(vector<SgVarRefExp*> a,SgProject* project,DFAnalysis* defuse);


int main(int argc,char* argv[])
{
    ROSE_INITIALIZE;
    vector<string> argvList(argv , argv + argc );
    SgProject* project = frontend(argvList );
    excute(project);
    return 0;
}
   
void excute(SgProject* project){
// Call the Def−Use Analysis
    DFAnalysis* defuse = new DefUseAnalysis(project );
    bool debug = false;
    defuse -> run (debug) ;
    // Output def use analysis results into a dot file defuse−>dfaToDOT ( ) ;
    defuse->dfaToDOT();
    
    // Find all variable references
    NodeQuerySynthesizedAttributeType vars = NodeQuery::querySubTree(project , V_SgVarRefExp);
    NodeQuerySynthesizedAttributeType::const_iterator i = vars.begin();
    
    cout<<vars.size();
    vector<SgVarRefExp*> tem;
    SgVarRefExp* varRef = isSgVarRefExp (*i ) ;
    tem.push_back(varRef);
    i++;
    for (; i!=vars.end();i++){
        SgVarRefExp* varRef = isSgVarRefExp (*i ) ;
        if(varRef->get_file_info()->get_line()!=(*(tem.end()-1))->get_file_info()->get_line()){
           forOut(tem, project,defuse);
           vector<SgVarRefExp*>().swap(tem);
        }
        tem.push_back(varRef);
    }
    forOut(tem, project,defuse);
    vector<SgVarRefExp*>().swap(tem);
    
    return;
}

void forOut(vector<SgVarRefExp*> a, SgProject* project,DFAnalysis* defuse){
    if((*a.begin())->get_type()->class_name()!="SgTypeInt"){
        return;
    }
    vector<SgVarRefExp*>::iterator it;
    int mark = 0;
    for(it=a.begin();it!=a.end();it++){
        if((*it)->get_type()->class_name()=="SgTypeFloat"||(*it)->get_type()->class_name()=="SgTypeDouble"){
            mark=1;
        }
    }
    if(mark){
        for(it=a.begin();it!=a.end();it++){
            SgInitializedName* initName = isSgInitializedName ((*it)->get_symbol()->get_declaration());
            std::string name = initName->get_qualified_name().str();
            
            vector<SgNode* > vec = defuse->getDefFor(*it , initName );
            ROSE_ASSERT(vec.size() >0 ); 
            std::cout <<"−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−"<<std::endl;
            if(it==a.begin()){
		cout<<SgNodeHelper::nodeToString((*it)->get_parent())<<endl;
		continue;
	    }
            //cout<<isSgStatement(*it)->unparseToString()<<endl;
            //cout<<isSgStatement((*it)->get_originalExpressionTree)->unparseToString()<<endl;

            std::cout << vec.size() << " definition entry/entries for " << (*it)->unparseToString() << " @ line " << (*it)->get_file_info()->get_line()<<":"<<(*it)->get_file_info()->get_col()<< std::endl;
            for (size_t j =0; j<vec.size(); j++) {
                cout<<vec[j]->class_name()<<" "<<vec[j]<<endl ;
                SgStatement* def_stmt = SageInterface::getEnclosingStatement(vec[j]);
                ROSE_ASSERT(def_stmt);
                cout<<def_stmt->unparseToString()<<" @ line "<<def_stmt->get_file_info()->get_line()<<":"<<def_stmt->get_file_info()->get_col()<<endl;
            }

        }
    }else{
        return;
    }
    
}
