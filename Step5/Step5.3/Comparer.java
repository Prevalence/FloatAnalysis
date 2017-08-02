import java.util.Iterator;

public class Comparer {
	
	private String statement;
	
	private int index;
	
	private int line;
	
	private boolean value1;
	
	private boolean value2;
	
	private boolean result;
	
	public String getStatement(){
		return statement;
	}
	
	public boolean getResult(){
		return result;
	}
	
	public int getIndex(){
		return index;
	}
	
	public boolean getValue1(){
		return value1;
	}
	
	public boolean getValue2(){
		return value2;
	}
	
	public int getLine(){
		return line;
	}
	
	public boolean compare(File file1,File file2){
		if(!file1.getFile_name().equals(file2.getFile_name())){
			return false;
		}else{
			Iterator<String> itr1=file1.getStatements();
			Iterator<String> itr2=file2.getStatements();
			String statement1,statement2;
			index=1;
			while(itr1.hasNext()&&itr2.hasNext()){
				statement1=itr1.next();
				statement2=itr2.next();
				if(statement1.compareTo(statement2)!=0){
					int number1=0,number2=0;
					String list1[]=statement1.split(":");
					String list2[]=statement2.split(":");
					number1=Integer.parseInt(statement1.substring(statement1.indexOf(":")+1,statement1.indexOf(",")));
					number2=Integer.parseInt(statement2.substring(statement2.indexOf(":")+1,statement2.indexOf(",")));
					if(number1>number2){
						//line:2,statement:3,bool:1
						value2=Integer.parseInt(list2[3])==1;
						value1=!value2;
						statement=list2[2].substring(0, list2[2].indexOf(","));
						line=number2;
					}else{
						value1=Integer.parseInt(list1[3])==1;
						value2=!value1;
						statement=list1[2].substring(0, list1[2].indexOf(","));
						line=number1;
					}
					result=true;
					break;
				}
				index++;
			}
			result=false;
		}
		return true;
	}
}
