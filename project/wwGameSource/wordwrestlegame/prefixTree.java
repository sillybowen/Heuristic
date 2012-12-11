import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class prefixTree {
	Node root;
	public prefixTree(){
    	root = new Node();
	}
	public void load() throws IOException{
		InputStream ist = new FileInputStream("/Users/silly/Desktop/dictionary.txt"); 
		BufferedReader istream = new BufferedReader(new InputStreamReader(ist));
		String text = istream.readLine(); //just read the first line in the text file		
		while (text!=null) {
			insert(text);
			text = istream.readLine();
		}
		istream.close();
	}
	public void insert(String s){
		root.insert(s, 0);
	}
	public boolean isValid(String s){
		return root.isValid(s, 0);
	}

}
