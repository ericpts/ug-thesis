public class Main {
    static private class One extends Other {
        public void foo() {
            System.out.println("method foo() of One");
        }
    }

    public static void bar(Other o) {
        o.foo();
    }

    public static void main(String[] args) {
        Other o = new One();
        bar(o);
    }
}
