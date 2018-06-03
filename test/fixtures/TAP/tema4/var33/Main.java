import java.io.*;
import java.util.InputMismatchException;
import java.util.Scanner;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class Main {

    final static String fin = "date.in";
    final static String fout = "date.out";

    /*
    Tinem DP[i][j] = latura cel mai mare patrat plin de 0
    care sa aiba coltul (jos,dreapta) pe (i, j)

    Dinamica o calculam intai pe linii, si in cadrul fiecarei linii
    crescator dupa coloane.


    -> DP pentru liniile de pe conturul matricii va fi corect
    (poate sa fie doar 0 sau 1, in functie de ce se afla pe casuta respectiva)

    Presupunem ca suntem la (i, j) si avem DP[i - 1][j], DP[i][j - 1], DP[i-1][j-1] calculate corect.
    (in caz ca aceastea nu exista suntem in cazul degenerat de mai sus)

    Dinamica din stanga ne restrictioneaza cat de lunga poate fi latura in partea aceea,
    idem pentru dinamica de sus,
    iar dinamica din (sus, stanga) ne restrictioneaza cat de lunga poate fi diagonala.

    Luam minim dintre toate aceastea si extindem cu 1.

    Pentru punctul b) tinem cum[i] = cate patrate exista cu latura >= i
    si il calculam de la i = latura maxima pana la i = 2
    tinand cont ca un patrat de latura K + 1 cu coltul (i, j)
    implica existenta si a unui patrat de latura K.
     */

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new FileReader(fin));
        final int n = sc.nextInt();
        final int m = sc.nextInt();

        boolean[][] mat = new boolean[n][m];
        int[][] dp = new int[n][m];

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                mat[i][j] = sc.nextInt() > 0;
            }
        }

        final int k = sc.nextInt();

        sc.close();
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                if (mat[i][j]) {
                    dp[i][j] = 0;
                    continue;
                }

                int prev = 0;

                if (i > 0 && j > 0) {
                    prev = min(dp[i - 1][j], dp[i][j - 1]);
                    prev = min(prev, dp[i - 1][j - 1]);
                }

                dp[i][j] = 1 + prev;
            }
        }

        FileWriter wrt = new FileWriter(new File(fout));

        {
            int mi = 0, mj = 0;

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (dp[i][j] > dp[mi][mj]) {
                        mi = i;
                        mj = j;
                    }
                }
            }
            int len = dp[mi][mj];

            wrt.write(Integer.toString(len) + "\n");
            wrt.write(Integer.toString(mi - len + 2) + " " + Integer.toString(mj - len + 2) + "\n");
        }

        int bound = max(n, m);

        int[] cum = new int[bound + 1];

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                cum[dp[i][j]]++;
            }
        }

        int count = 0;

        for(int i = bound; i > 1; --i) {
            count += cum[i];
            cum[i - 1] += cum[i];
        }

        wrt.write(Integer.toString(count) + "\n");
        wrt.close();
    }
}
