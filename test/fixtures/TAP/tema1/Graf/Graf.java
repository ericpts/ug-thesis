import java.util.*;

/**
 * Created by ericpts on 10/23/16.
 */
public class Graf {

    private Map<Integer, List<Integer>> _edges;


    public Graf() {
        _edges = new HashMap<>();
    }

    private List<Integer> get(int x) {
        if (!_edges.containsKey(x))
            _edges.put(x, new LinkedList<>());

        return _edges.get(x);
    }

    public void addEdge(int a, int b) {
        get(a).add(b);
        get(b).add(a);
    }

    public Map<Integer, Integer> breadthFirstSearch(int root) {
        Queue<Integer> queue = new LinkedList<>();

        Map<Integer, Integer> dist = new HashMap<>();

        queue.add(root);
        dist.put(root, 0);

        while (!queue.isEmpty()) {
            int now = queue.remove();
            int nowDist = dist.get(now);

            for (int next: get(now)) {
                if (!dist.containsKey(next)) {
                    dist.put(next, nowDist + 1);
                    queue.add(next);
                }
            }
        }

        return dist;
    }
}
