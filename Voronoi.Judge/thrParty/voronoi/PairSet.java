package voronoi;

import java.util.Set;
import java.util.HashSet;
import java.lang.Integer;

public class PairSet {
  private Set<Pair<Integer, Integer>> myset;
  
  public PairSet() {
    myset = new HashSet<PairSet.Pair<Integer,Integer>>();
  }
  
  public boolean insert(int x, int y) {
    Pair<Integer, Integer> newpair = new Pair<Integer, Integer>(x, y);
    return myset.add(newpair);
  }
  
  public boolean find(int x, int y) {
    Pair<Integer, Integer> toFindPair = new Pair<Integer, Integer>(x, y);
    return myset.contains(toFindPair);
  }
  
  public static void main(String[] args) {
    PairSet myPairSet = new PairSet();
    myPairSet.insert(10, 20);
    System.out.println("My set contains(10, 21)? (no): " + myPairSet.find(10, 21));
  }
  
  
  // A Pair<L, R> class for recording (x, y) axise for a placed stone
  public class Pair<L,R> {
    private final L left;
    private final R right;

    public Pair(L left, R right) {
      this.left = left;
      this.right = right;
    }

    public L getLeft() { return left; }
    public R getRight() { return right; }

    @Override
    public int hashCode() { return left.hashCode() ^ right.hashCode(); }

    @Override
    public boolean equals(Object o) {
      if (o == null) return false;
      if (!(o instanceof Pair)) return false;
      Pair pairo = (Pair) o;
      return this.left.equals(pairo.getLeft()) &&
             this.right.equals(pairo.getRight());
    }

  }

}
