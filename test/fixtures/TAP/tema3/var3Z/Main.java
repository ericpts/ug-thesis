import java.io.*;
import java.util.Scanner;

public class Main {

    static final String fin = "z.in";
    static final String fout = "z.out";

    /* La fiecare apel recursiv N este redus cu 1
    => O(N)
     */
    static Integer solve(final int n, final int x, final int y, final int acc) {

        if (n == 0) {
            assert x == 1;
            assert y == 1;
            return 1 + acc;
        }

        final int side = 1 << (n - 1);
        final int area = side * side;

        int add = 0;

        int nx = x;
        if (nx > side) {
            nx -= side;
            add += 2 * area;
        }

        int ny = y;
        if (ny > side) {
            ny -= side;
            add += area;
        }

        return solve(n - 1, nx, ny, acc + add);
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new FileInputStream(fin));
        final int n = sc.nextInt();
        final int k = sc.nextInt();


        Writer writer = new FileWriter(fout);
        for(int i = 0; i < k; ++i) {
            final int x = sc.nextInt();
            final int y = sc.nextInt();

            writer.write(solve(n, x, y, 0).toString() + "\n");
        }

        sc.close();
        writer.close();
    }
}
