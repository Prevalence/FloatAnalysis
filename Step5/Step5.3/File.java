import java.util.ArrayList;
import java.util.Iterator;

public class File {
	private String file_name;
	
	private ArrayList<String> statements;
	
	public File(String name){
		this.file_name=name;
		statements=new ArrayList<String>();
	}
	
	public void addStatement(String statement){
		statements.add(statement);
	}
	
	public String getFile_name(){
		return file_name;
	}
	
	public Iterator<String> getStatements(){
		return statements.iterator();
	}
}