import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class Main {

    private static void IntArbore(String file) throws FileNotFoundException {
        Scanner sc = new Scanner(new FileReader(file));

        Arbore<Integer> arb = new Arbore<>();

        while (sc.hasNextInt()) {
            arb.add(sc.nextInt());
        }

        System.out.println(arb.toString());
    }

    private static void BaseArbore(List<Integer> ints) {

        Arbore<Base> arb = new Arbore<>();

        for(Integer i: ints)
            arb.add(new Base(i));

        System.out.println(arb.toString());
    }

    private static void DerivedArbore(List<Integer> ints) {

        Arbore<Base> arb = new Arbore<>();

        for(Integer i: ints)
            arb.add(new Derived(i));

        System.out.println(arb.toString());
    }

    public static void main(String[] args) throws FileNotFoundException {
        String file = "in";
        IntArbore(file);

        LinkedList<Integer> list = new LinkedList<Integer>();
        list.add(1);
        list.add(3);
        list.add(5);
        list.add(6);
        list.add(10);

        BaseArbore(list);
        DerivedArbore(list);

        CheckEquals(list);

        LinkedList<String> strList = new LinkedList<String>();
        strList.add("Hello");
        strList.add("world");
        strList.add("this is");
        strList.add("eric");

        CheckEquals(strList);
    }

    private static<E extends Comparable> Arbore<E> makeArbore(LinkedList<E> list) {
        Arbore<E> ret = new Arbore<E>();
        for (E e: list)
            ret.add(e);

        return ret;
    }

    private static<E extends Comparable> void CheckEquals(LinkedList<E> list) {
        Arbore<E> init = makeArbore(list);

        Collections.shuffle(list);
        Arbore<E> shuffled = makeArbore(list);

        assert(init.equals(shuffled));
    }
}
