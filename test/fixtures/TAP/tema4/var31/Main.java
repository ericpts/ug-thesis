import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Main {

    final static String fin = "date.in";
    final static String fout = "date.out";

    final static int SIGMA = 'z' - 'a' + 1;

    public static void main(String[] args) throws IOException {

        Scanner sc = new Scanner(new FileReader(fin));

        ArrayList<String> words = new ArrayList<String>();

        while(sc.hasNext()) {
            words.add(sc.next());
        }
        sc.close();

        final int n = words.size();


        /*
        DP[i][j][k] = cel mai lung subsir care sa contina cuvintele
        1..i
        si caracterele in care se termina sa fie j si k

        Dinamica o calculam in ordine i = 1, i = 2, ..., i = n

        Demonstratie prin inductie:

        pentru i = 1 dinamica va fi calculata corect
        (plina de 0, exceptand starea corespunzatoare cuvantului @1)

        Presupunem ca dinamica este corecta pentru i = 1, 2, ..., k

        Demonstram ca este corecta si pentru (k + 1) :

        pentru DP[k + 1][p][q], fie nu l-am inclus deloc pe cuvantul @k + 1
        caz in care ajungem in DP[k][p][q],

        fie l-am inclus ( deci @k + 1 se termina in p si q ), caz in care am venit din
        primele doua caractere ale lui @k + 1

         */

        int[][][] dp = new int[n + 1][SIGMA][SIGMA];

        boolean[][][] take = new boolean[n + 1][SIGMA][SIGMA];

        for(int j = 0; j < SIGMA; ++j) {
            for(int k = 0; k < SIGMA; ++k) {
                dp[0][j][k] = 0;
            }
        }


        for(int i = 1; i <= n; ++i) {
            for(int j = 0; j < SIGMA; ++j) {
                for(int k = 0; k < SIGMA; ++k) {
                    dp[i][j][k] = dp[i - 1][j][k];
                    take[i][j][k] = false;
                }
            }

            String w = words.get(i - 1);
            int k = w.length();

            int now = dp[i - 1][w.charAt(0) - 'a'][w.charAt(1) - 'a'] + 1;

            int p = w.charAt(k - 2) - 'a';
            int q = w.charAt(k - 1) - 'a';

            if (now > dp[i][p][q]) {
                take[i][p][q] = true;
                dp[i][p][q] = now;
            }
        }

        int sj = 0, sk = 0;

        for(int j = 0; j < SIGMA; ++j) {
            for(int k = 0; k < SIGMA; ++k) {
                if (dp[n][j][k] > dp[n][sj][sk]) {
                    sj = j;
                    sk = k;
                }
            }
        }

        ArrayList<String> sol = new ArrayList<String>();

        for(int at = n; at > 0; --at) {
            if (!take[at][sj][sk]) {
                continue;
            }

            String w = words.get(at - 1);
            int k = w.length();
            sol.add(w);

            assert(w.charAt(k - 2) - 'a' == sj);
            assert(w.charAt(k - 1) - 'a' == sk);

            sj = w.charAt(0) - 'a';
            sk = w.charAt(1) - 'a';
        }

        FileWriter fw = new FileWriter(new File(fout));
        for(int i = sol.size(); i >= 1; --i) {
            fw.write(sol.get(i - 1) + "\n");
        }
        fw.close();
    }
}
