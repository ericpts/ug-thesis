import java.io.*;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

public class Main {

    final static String fin = "cmap.in";
    final static String fout = "cmap.out";

    static class Point implements Comparable {
        int x, y;

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public int compareTo(Object o) {
            Point p = (Point)o;
            if (x == p.x) {
                if (y < p.y)
                    return -1;
                if (y == p.y)
                    return 0;
                return 1;
            } else {
                if (x < p.x)
                    return -1;
                return 1;
            }
        }
    }

    static long distance(Point a, Point b) {
        long dx = a.x - b.x;
        long dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    /* La fiecare pas facem doua apeluri recursive
    si O(N) operatii
    T(N) = 2 * T(N / 2) + O(N)
    T(N) = O(NlogN)
     */

    static Long divide(Point[] p, int lo, int hi) {
        if (lo == hi)
            return Long.MAX_VALUE;

        int mid = (lo + hi) / 2;

        Long d1 = divide(p, lo, mid);
        Long d2 = divide(p, mid + 1, hi);

        Long dmin = d1;
        if (d2 < dmin)
            dmin = d2;

        Double d = Math.sqrt(Double.valueOf(dmin));

        ArrayList<Point> pRight = new ArrayList<>();
        ArrayList<Point> pLeft = new ArrayList<>();

        int xMid = p[mid].x;
        for(int i = mid + 1; i <= hi; ++i)
            if (Math.abs(p[i].x - xMid) <= d)
                pRight.add(p[i]);

        int at = 0;
        for(int i = lo; i <= mid; ++i) {
            Point now = p[i];
            while (at < pRight.size() && pRight.get(at).y + d < now.y)
                at++;

            for(int j = 0; j <= 7 && at + j < pRight.size(); ++j) {
                Point cmp = pRight.get(at + j);
                Long dnow = distance(now, cmp);

                if (dnow < dmin)
                    dmin = dnow;
            }
        }

        pRight.clear();


        int atLeft = 0;
        for(int i = lo; i <= mid; ++i)
            pLeft.add(p[i]);

        int atRight = 0;
        for(int i = mid + 1; i <= hi; ++i)
            pRight.add(p[i]);


        at = lo;
        while (at <= hi) {

            if (atLeft == pLeft.size() ||
                    (atRight != pRight.size() && pRight.get(atRight).y < pLeft.get(atLeft).y)) {
                p[at] = pRight.get(atRight);
                atRight++;
            } else {
                p[at] = pLeft.get(atLeft);
                atLeft++;
            }

            at++;
        }

        return dmin;
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new FileInputStream(fin));

        final int n = sc.nextInt();


        Point[] p = new Point[n];

        for(int i = 0; i < n; ++i)
            p[i] = new Point(sc.nextInt(), sc.nextInt());
        sc.close();

        Arrays.sort(p);

        Writer wrt = new FileWriter(fout);
        Double rez = Math.sqrt(Double.valueOf(divide(p, 0, n - 1)));
        wrt.write(rez.toString() + "\n");
        wrt.close();
    }
}
