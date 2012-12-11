import java.io.IOException;


public class main {
    public static void main(String[] args) throws IOException {
    	prefixTree pt=new prefixTree();
    	pt.load();	
    	System.out.println(pt.isValid("thisiswrong"));
    	System.out.println(pt.isValid("zymoses"));
    }
}
