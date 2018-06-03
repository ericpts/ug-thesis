package com.stavindustries;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Scanner;

import static java.lang.Integer.max;

public class Main {

    static final String fin = "date.in";
    static final String fout = "date.out";


    private static ArrayList<Integer> getBestCol(int[][] dp, int i, int j, int m) {

        ArrayList<Integer> candid = new ArrayList<>();
        candid.add(j);
        if (j > 0)
            candid.add(j - 1);
        if (j + 1 < m)
            candid.add(j + 1);

        int best = dp[i - 1][j];

        for(Integer l: candid) {
            if (dp[i - 1][l] > best)
                best = dp[i - 1][l];
        }

        ArrayList<Integer> ret = new ArrayList<Integer>();
        for(Integer l: candid) {
            if (dp[i - 1][l] == best) {
                ret.add(l);
            }
        }
        return ret;
    }


    /* Tinem DP[i][j] =>
    cel mai mare cost obtinut astfel incat sa fi ajuns pe casuta (i, j)

    Il calculam dinspre coloana 1 inspre coloana m.

    Demonstratie prin inductie dupa coloana:

    DP pentru prima coloana este calculat corect (singurul mod de a ajunge
    pe o anumita casuta este sa fi inceput acolo).

    Presupunem DP corect pentru coloanele 1, 2, ..., k

    Atunci pentru casuta (k + 1, i) sunt 3 moduri in care putem ajunge:
    fie din stanga, stanga-sus sau stanga-jos
    fiecarei optiune ii corespunde o stare de pe coloana anterioara
    (calculata corect).
    Asa ca si dinamica pentru (k + 1, i) va fi calculata corect,
    deci toata coloana k + 1 va fi corecta.

     */

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new File(fin));
        final int n = sc.nextInt();
        final int m = sc.nextInt();

        int[][] mat = new int[n][m];
        boolean[][] mul = new boolean[m][n];
        int[][] dp = new int[m][n];

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                mat[i][j] = sc.nextInt();
            }
        }
        sc.close();

        for(int i = 0; i < n; ++i) {
            dp[0][i] = mat[i][0];
            mul[0][i] = false;
        }

        for(int i = 1; i < m; ++i) {
            for(int j = 0; j < n; ++j) {
                ArrayList<Integer> p = getBestCol(dp, i, j, n);

                mul[i][j] = mul[i - 1][p.get(0)];
                dp[i][j] = dp[i - 1][p.get(0)] + mat[j][i];

                if (p.size() > 1)
                    mul[i][j] = true;
            }
        }

        int best = 0;
        for(int j = 0; j < n; ++j) {
            if (dp[m - 1][j] > dp[m - 1][best]) {
                best = j;
            }
        }

        FileWriter wrt = new FileWriter(new File(fout));

        wrt.write(Integer.toString(dp[m - 1][best]) + "\n");

        class Entry {
            public int x, y;

            Entry(int x, int y) {
                this.x = x;
                this.y = y;
            }

            public String tostring() {
                return Integer.toString(x) + " " + Integer.toString(y);
            }
        };

        ArrayList<Entry> entries = new ArrayList<Entry>();

        int j = best;
        for(int at = m - 1; at >= 0; --at) {

            entries.add(new Entry(j + 1, at + 1));

            if (at > 0)
                j = getBestCol(dp, at, j, n).get(0);
        }

        for(int i = entries.size(); i >= 1; --i) {
            Entry e = entries.get(i - 1);
            wrt.write(e.tostring() + "\n");
        }

        if (!mul[m - 1][best]) {
            wrt.write("Traseu unic\n");
        }

        wrt.close();
    }
}
