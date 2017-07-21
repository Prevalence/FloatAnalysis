#include "rose.h"
#include "DefUseAnalysis.h"
#include <string>
#include <iostream>


using namespace std ;
void excute(int argc,char* argv[]);
void forOut(vector<SgVarRefExp*> a);


int main(int argc,char* argv[])
{
    excute(argc,argv);
}

void excute(int argc,char* argv[]){
    ROSE_INITIALIZE;
    vector<string> argvList(argv , argv + argc );
    SgProject* project = frontend(argvList );
    
    // Call the Def−Use Analysis
    DFAnalysis* defuse = new DefUseAnalysis(project );
    bool debug = false;
    defuse -> run (debug) ;
    // Output def use analysis results into a dot file defuse−>dfaToDOT ( ) ;
    defuse->dfaToDOT();
    
    // Find all variable references
    NodeQuerySynthesizedAttributeType vars = NodeQuery::querySubTree(project , V_SgVarRefExp);
    NodeQuerySynthesizedAttributeType::const_iterator i = vars.begin();
    
    cout<<sizeof(i);
    
    vector<SgVarRefExp*> tem;
    for (; i!=vars.end();i++){
        SgVarRefExp* varRef = isSgVarRefExp (*i ) ;
        if(varRef->get_file_info()->get_line()!=tem.end()->get_file_info()->get_line()){
            forOut(tem);
            tem.clean();
        }
        tem.push_back(varRef);
    }
    forOut(tem);
    tem.clean();

    return 0;
}

void forOut(vector<SgVarRefExp*> a){
    if(a.begin()->get_type()->class_name()!="SgTypeInt"){
        return 0;
    }
    vector<SgVarRefExp*>::iterator it;
    int mark = 0;
    for(it=a.begin();it!=vec.end();it++){
        if(it->get_type()->class_name()=="SgTypeFloat"||it->get_type()->class_name()=="SgTypeDouble"){
            makr=1;
        }
    }
    if(mark){
        for(it=a.begin();it!=vec.end();it++){
            SgInitializedName* initName = isSgInitializedName (it->get_symbol()->get_declaration());
            std::string name = initName->get_qualified_name().str();
            // Find reaching definition of initName at the control flow node varRef
            vector<SgNode* > vec = defuse->getDefFor(varRef , initName );
            ROSE_ASSERT(vec.size() >0 ); // each variable reference must have a definition somewhere
            // Output each reaching definition node and the corresponding statement.
            std::cout <<"−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−"<<std::endl;
            
            cout<<varRef->get_type()->class_name()<<endl;
            
            //cout<<tem->unparseToString()<<" = "<<varRef->unparseToString()<<endl;
            
            std::cout << vec.size() << " definition entry/entries for " << varRef->unparseToString() << " @ line " << varRef->get_file_info()->get_line()<<":"<<varRef->get_file_info()->get_col()<< std::endl;
            for (size_t j =0; j<vec.size(); j++) {
                cout<<vec[j]->class_name()<<" "<<vec[j]<<endl ;
                SgStatement* def_stmt = SageInterface::getEnclosingStatement(vec[j]);
                ROSE_ASSERT(def_stmt);
                cout<<def_stmt->unparseToString()<<" @ line "<<def_stmt->get_file_info()->get_line()<<":"<<def_stmt->get_file_info()->get_col()<<endl;
            }

        }
    }else{
        return 0;
    }
    
}
