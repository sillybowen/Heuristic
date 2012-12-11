package wordwrestlegame;

import java.util.*;
public class Node {
    private boolean isWord; // 0- no child not a word                                                                                       
    //1- has child                                                                                                                        
    //2- is a word                                                                                                                        
    private HashMap children;
    public Node () {
        isWord = false;
        children = new HashMap<Character,Node>();
    }
    public String getRandomString(String s) {
      Random rand = new Random();
      int rint;
      if (isWord) {
        rint = rand.nextInt()%7;
        if (rint == 0|| children.size()==0)
          return s;
      }
      rint = rand.nextInt();
      if(rint<0)rint = -rint;
      rint = rint%children.size();
      System.out.print(rint);
      Character key='a';
      Set<Character> keys=children.keySet(); 
      for (Character k : keys){
        if (rint == 0)
          key = k;
        rint --;
      }
      System.out.print(key);
      return  ((Node)children.get(key)).getRandomString(s+key);
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
