    vector<SgVarRefExp*>::iterator it;
    int mark = 0;
    vector<string> definition;
    for(it=a.begin();it!=a.end();it++){
        if((*it)->get_type()->class_name()=="SgTypeFloat"||(*it)->get_type()->class_name()=="SgTypeDouble"){
            mark=1;
        }
    }
    ofstream fout;
    if(mark){
        for(it=a.begin();it!=a.end();it++){
            if(markForIO){
                fout.open("testT.txt",ios::trunc);
                markForIO = 0;
            }
            else{
                fout.open("testT.txt",ios::app);
            }
            SgInitializedName* initName = isSgInitializedName ((*it)->get_symbol()->get_declaration());
            std::string name = initName->get_qualified_name().str();
            
            vector<SgNode* > vec = defuse->getDefFor(*it , initName );
            ROSE_ASSERT(vec.size() >0 );
            if(it==a.begin()){
                fout<<"−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−"<<std::endl;
                fout<<"# "<<SgNodeHelper::nodeToString((*it)->get_parent())<<" #";
                fout.close();
                continue;
            }
            if((*it)->get_type()->class_name()=="SgTypeFloat"){
                fout<<" float:"<<(*it)->unparseToString();
            }else if((*it)->get_type()->class_name()=="SgTypeDouble"){
                fout<<" double:"<<(*it)->unparseToString();
            }else if((*it)->get_type()->class_name()=="SgTypeInt"){
                fout<<" int:"<<(*it)->unparseToString();
            }
            definition.push_back("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            definition.push_back("\n");
            definition.push_back(boost::to_string(vec.size()));
            definition.push_back(" definition entry/entries for ");
            definition.push_back((*it)->unparseToString());
            definition.push_back(" @ line ");
            definition.push_back(boost::to_string((*it)->get_file_info()->get_line()));
            definition.push_back(":");
            definition.push_back(boost::to_string((*it)->get_file_info()->get_col()));
            definition.push_back("");
        
            //std::cout << vec.size() << " definition entry/entries for " << (*it)->unparseToString() << " @ line " << (*it)->get_file_info()->get_line()<<":"<<(*it)->get_file_info()->get_col()<< std::endl;
            for (size_t j =0; j<vec.size(); j++) {
//                cout<<vec[j]->class_name()<<" "<<vec[j]<<endl ;
                SgStatement* def_stmt = SageInterface::getEnclosingStatement(vec[j]);
                ROSE_ASSERT(def_stmt);
//                cout<<def_stmt->unparseToString()<<" @ line "<<def_stmt->get_file_info()->get_line()<<":"<<def_stmt->get_file_info()->get_col()<<endl;
                definition.push_back(def_stmt->unparseToString());
                definition.push_back(" @ line ");
                definition.push_back(boost::to_string(def_stmt->get_file_info()->get_line()));
                definition.push_back(":");
                definition.push_back(boost::to_string(def_stmt->get_file_info()->get_col()));
                definition.push_back("\n");
            }
        fout.close();
        }
    fout.open("testT.txt",ios::app);
    fout<<"\n";
    vector<string>::iterator i = definition.begin();
    for(;i!=definition.end();i++){
        fout<<*i;
    }
    fout.close();
	return 0;
    }else{
        return 1;
    }
    
}