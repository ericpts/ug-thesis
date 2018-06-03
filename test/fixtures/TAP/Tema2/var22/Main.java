import java.io.*;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;

public class Main {

    static final String fin = "date.in";
    static final String fout = "date.out";

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new File(fin));

        final int n = sc.nextInt();

        class Entry {
            Integer index;
            Integer length;
            Integer weight;

            Float getValue() {
                return ((float)weight) / length;
            }
        }

        Entry[] entries = new Entry[n];

        for(int i = 0; i < n; ++i) {
            entries[i] = new Entry();

            entries[i].index = i + 1;
            entries[i].length = sc.nextInt();
            entries[i].weight = sc.nextInt();
        }

				/* Demonstratie:
				 * o solutie ca avea entry-urile cu > in functie de val
				 *
				 * presupunem ca exista o solutie S1 astfel incat exista
				 * i, cu getVal(i) < getVal(i + 1)
				 * deci weight[i] / length[i] < weight[i + 1] / length[i + 1]
				 * deci weight[i] * length[i + 1] < weight[i + 1] * length[i]
				 * w[i] * l[i + 1] < w[i + 1] * l[i]
				 * w[i] * l[i + 1] - w[i + 1] * l[i] < 0
				 *
				 * Formam solutia S2 din S1, cu (i, i + 1) swap-uite
				 * iar diferenta dintre ele va fi:
				 * S2 - S1 = weight[i] * length[i + 1] - weight[i + 1] * length[i] < 0
				 * deci S2 este mai buna decat S1
				 */

        Arrays.sort(entries, new Comparator<Entry>() {
            @Override
            public int compare(Entry entry, Entry t1) {
                return t1.getValue().compareTo(entry.getValue());
            }
        });

        Writer wrt = new FileWriter(new File(fout));

        for(int i = 0; i < n; ++i) {
            wrt.write(entries[i].index.toString() + " ");
        }
        wrt.write("\n");
        wrt.close();
    }
}
