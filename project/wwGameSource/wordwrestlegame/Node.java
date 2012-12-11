import java.util.*;
public class Node {
    private boolean isWord; // 0- no child not a word                                                                                       
    //1- has child                                                                                                                        
    //2- is a word                                                                                                                        
    private HashMap children;
    public Node () {
        isWord = false;
        children = new HashMap<Character,Integer>();
    }
    public void insert(String s, int index) {
        if (index == s.length()) {
            isWord = true;
            return ;
        }
        Character c = new Character(s.charAt(index));
        Node child = (Node) children.get(c);
        if (child == null) {
        	child = new Node();
        	children.put(c, child);
        	child = (Node) children.get(c);
        }
        child.insert(s, index+1);
    }
    public boolean isValid(String s, int index) {
        if (index == s.length()) {
            if (isWord)
                return true;
            else
                return false;
        }
        Character c = new Character(s.charAt(index));
        Node child = (Node) children.get(c);
        if (child == null) 
        	return false;
        return child.isValid(s,index+1);
    }
}
