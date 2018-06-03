import java.io.*;
import java.util.Scanner;

public class Main {

    final static String fin = "date.in";
    final static String fout = "date.out";

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new File(fin));

        final int n = sc.nextInt();
        int[] v = new int[n];

        for(int i = 0; i < n; ++i)
            v[i] = sc.nextInt();

        sc.close();

        int lo = 0, hi = n - 1;
        int last = -1;
        /* La fiecare pas se injumatateste intervalul de cautare
        => O(logN)
         */

        while(lo <= hi) {
            int mid = (lo + hi) / 2;
            if (mid != hi && v[mid] < v[mid + 1]) {
                last = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        Writer wrt = new FileWriter(fout);
        wrt.write(((Integer)(v[last + 1])).toString() + "\n");
        wrt.close();
    }
}
