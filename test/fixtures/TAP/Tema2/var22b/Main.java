import java.io.*;
import java.util.*;

public class Main {

    static final String fin = "date.in";
    static final String fout = "date.out";

    static class Entry {
        Integer index;
        Integer length;
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(new File(fin));

        final int n = sc.nextInt();
        final int p = sc.nextInt();

        Entry[] entries = new Entry[n];

        for(int i = 0; i < n; ++i) {
            entries[i] = new Entry();
            entries[i].index = i + 1;
            entries[i].length = sc.nextInt();
        }
        sc.close();

        Arrays.sort(entries, new Comparator<Entry>() {
            @Override
            public int compare(Entry entry, Entry t1) {
                return entry.length.compareTo(t1.length);
            }
        });

        ArrayList<List<Entry>> pipelines = new ArrayList<>(p);

        for(int i = 0; i < p; ++i) {
            pipelines.add(new LinkedList<>());
        }

        /* Demonstratie:

        Lema 1:
        pentru o singura banda pe care au fost puse lungimile
        L[1], L[2], ..., L[k]
        este optim sa avem L[1] <= L[2] <= ... <= L[k]

        Dem. lema 1:

        fie S o solutie astfel incat exista o pozitie i cu
        L[i] > L[i + 1]

        notam L[i] = a, L[i + 1] = b
        a > b => a - b > 0

        Costul total este k * L[1] + (k - 1) * L[2] + ... + (k - i + 1) * a + (k - i) * b + ...

         vom construi o solutie S' obtinuta din S, in care interschimbam
         L[i] cu L[i + 1]

            Costul lui S' este k * L[1] + (k - 1) * L[2] + ... + (k - i + 1) * b + (k - i) * a + ...


         S - S' = (k - i + 1) * a + (k - i) * b - (k - i + 1) * b - (k - i) * a
         S - S' = (k - i) * a + a - (k - i) * a + (k - i) * b - (k - i) * b - b
         S - S' = a - b > 0
         Rezulta ca S' are un cost mai bun decat S
         deci S nu e solutie


         In continuare, vom considera cazul cu P benzi

         v[1][1]     v[1][2]     v[1][3] ...     v[1][k]      v[1][k + 1]
         v[2][1]     v[2][2]     v[2][3] ...     v[2][k]      v[2][k + 1]
         ..........................................
         v[j][1]     v[j][2]     v[j][3] ...     v[j][k]      v[j][k + 1]
         v[j + 1][1] v[j + 1][2] v[j + 1][3] ... v[j + 1][k]
         ..................................
         v[p][1] v[p][2] v[p][3] ... v[p][k]

         pe coloana 1 vom pune primele cele mai mici P lungimi, pe coloana 2 urmatoarele, samd
         (ultima coloana nu va fi umpluta pana la capat).

         demonstratia corectitudinii:

         fie S[i] = suma elementelor de pe coloana i.

         Atunci luam o solutie T care nu are proprietatea de mai sus.
         Inseamna ca exista doua coloane i, j cu i < j
         Astfel incat pe coloana i este un element a, si pe coloana j un element b
         astfel incat a > b.

         Atunci, aplicand lema 1, obtinem o solutie T' in care b e pe coloana i si a pe coloana j
         si costul solutiei T' < costul solutiei T.

         */

        int at = 0;
        for(int i = 0; i < n; ++i) {
            pipelines.get(at).add(entries[i]);
            at++;
            if (at == p)
                at = 0;
        }

        Writer wrt = new FileWriter(new File(fout));

        for(int i = 0; i < p; ++i) {
            for(Entry e: pipelines.get(i)) {
                wrt.write(e.index.toString() + " ");
            }
            wrt.write("\n");
        }

        wrt.close();
    }
}
