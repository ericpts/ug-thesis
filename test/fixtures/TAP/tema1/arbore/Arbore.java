import java.util.Comparator;
import java.util.LinkedList;
import java.util.Stack;

/**
 * Created by ericpts on 10/23/16.
 */
public class Arbore<ValueType extends Comparable> {
    private static class Node<ValueType extends Comparable> {
        public ValueType val;
        public Node L, R;

        public Node(ValueType v) {
            val = v;
            L = R = null;
        }
    }

    Node root = null;

    private boolean add(Node root, Node add) {

        int cmp = add.val.compareTo(root.val);

        if (cmp == 0)
            return false;

        if (cmp < 0) {
            if (root.L == null) {
                root.L = add;
                return true;
            }
            return add(root.L, add);
        } else {
            if (root.R == null) {
                root.R = add;
                return true;
            }
            return add(root.R, add);
        }
    }

    boolean add(ValueType e) {
        Node add = new Node(e);
        if (root == null) {
            root = add;
            return true;
        }

        return add(root, add);
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        Stack<Node> stack = new Stack<>();

        Node now = root;
        stack.add(now);

        while(now.L != null) {
            now = now.L;
            stack.add(now);
        }

        while(!stack.isEmpty()) {
            now = stack.pop();
            sb.append(now.val.toString() + " ");

            Node next = now.R;
            while(next != null) {
                stack.add(next);
                next = next.L;
            }
        }

        return sb.toString();
    }

    private String uniqueString() {
      String in = toString();
      StringBuilder sb = new StringBuilder();

      Stack<Node> stack = new Stack<>();

      Node now = root;
      stack.add(now);

      while(!stack.isEmpty()) {
        now = stack.pop();
        sb.append(now.val.toString() + " ");

        if (now.R != null)
          stack.add(now.R);

        if (now.L != null)
          stack.add(now.L);
      }

      String pre = sb.toString();

      return in + "|||" + pre;

    }

    public boolean equals(Arbore arb) {
        return uniqueString().equals(arb.uniqueString());
    }

    public int hashCode() {
        return uniqueString().hashCode();
    }
}
