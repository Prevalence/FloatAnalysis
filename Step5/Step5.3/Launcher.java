import java.io.IOException;

public class Launcher {
	public static void main(String[]args){
		Launcher launcher=new Launcher();
		new IOHelper();
		launcher.run();
	}
	
	public void run(){
		Comparer comparer=new Comparer();
		File file1,file2 = null;
		IOHelper.clear();
		try {
			while((file1=IOHelper.getNextFileOne())!=null&&(file2=IOHelper.getNextFileTwo())!=null){
				boolean result=comparer.compare(file1, file2);
				if(result){
					if(comparer.getResult()){
						IOHelper.write(file1.getFile_name(), comparer.getIndex(), comparer.getStatement(), comparer.getValue1(), comparer.getValue2(),comparer.getLine());
					}else{
						IOHelper.write(file1.getFile_name());
					}
				}else{
					System.out.println("wrong");
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
