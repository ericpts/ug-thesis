public class Main {

    public static void main(String[] args) {
        queue<Character> qchar = new queue<>();

        qchar.add('a');
        qchar.add('b');
        qchar.add('c');

        System.out.println(qchar.remove());
        System.out.println(qchar.remove());


        queue<Integer> qint = new queue<>();
        qint.add(123);
        qint.add(150);
        qint.add(190);

        System.out.println(qint.remove());
        System.out.println(qint.remove());

        queue<queue<Integer>> qqint = new queue<>();

        qqint.add(qint);

        queue<Integer> qint2 = new queue<>();
        qint2.add(130);

        System.out.println(qqint.remove().remove());
        System.out.println(qqint.remove().remove());
    }
}
