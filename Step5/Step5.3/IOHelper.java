import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class IOHelper {
	private static BufferedReader br1;
	
	private static BufferedReader br2;
	
	private static BufferedWriter bw;
	
	public IOHelper(){
		try {
			br1=new BufferedReader(new FileReader("src/input_one.txt"));
			br2=new BufferedReader(new FileReader("src/input_two.txt"));
			bw=new BufferedWriter(new FileWriter("src/output.txt",true));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static File getNextFileOne() throws IOException{
		String line;
		String name;
		File myFile=null;
		if((name=br1.readLine())==null){
			return null;
		}else{
			myFile=new File(name);
		}
		while((line=br1.readLine())!=null&&!line.equals("******")){
			myFile.addStatement(line);
		}
		return myFile;
	}
	
	public static File getNextFileTwo() throws IOException{
		String line;
		String name;
		File myFile=null;
		if((name=br2.readLine())==null){
			return null;
		}else{
			myFile=new File(name);
		}
		while((line=br2.readLine())!=null&&!line.equals("******")){
			myFile.addStatement(line);
		}
		return myFile;
	}
	
	public static boolean write(String fileName,int index,String statement,boolean value1,boolean value2,int line){
		String arguement=fileName+":\nError occur at "+index+" branch and "+line+" line!\n";
		arguement+="First "+statement+" is "+value1+" while second "+statement+" is "+value2+"\n";
		try {
			bw.append(arguement);
			bw.append("******");
			bw.newLine();
			bw.flush();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public static boolean write(String fileName){
		try {
			bw.append(fileName+":\nThere are no errors in this file!\n");
			bw.append("******");
			bw.newLine();
			bw.flush();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public static void clear(){
		try {
			bw=new BufferedWriter(new FileWriter("src/output.txt",false));
			bw.write("");
			bw.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
