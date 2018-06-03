import java.util.Map;

public class Main {

    public static void main(String[] args) {
        Graf G = new Graf();

        G.addEdge(1, 2);
        G.addEdge(1, 3);
        G.addEdge(1, 4);
        G.addEdge(4, 10);

        Map<Integer, Integer> distance = G.breadthFirstSearch(1);

        for(Map.Entry<Integer, Integer> entry: distance.entrySet()) {
            System.out.println("dist to " + entry.getKey().toString() + " is " + entry.getValue().toString());
        }
    }
}
