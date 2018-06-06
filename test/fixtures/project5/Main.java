public class Main {
    static private class C implements I{
        public void foo() {
            System.out.println("foo() of C");
        }
    }

    public static void bar(I i) {
        i.foo();
    }

    public static void main(String[] args) {
        I i = new C();
        bar(i);
    }
}
