import java.util.function.Function;

public class Main {
    String foo(String arg) {
        System.out.println(arg);
        return arg;
    }

    public static void main(String[] args) {
        Main m = new Main();
        Function<String, String> c = (e) -> m.foo(e);
        c.apply("Bla");
    }
}
