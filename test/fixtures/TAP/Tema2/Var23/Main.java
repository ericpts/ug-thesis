import java.io.*;
import java.util.*;

public class Main {

    final static String fin = "date.in";
    final static String fout = "date.out";

    static private class UnionFind {

        int[] next;
        boolean[] viz;

        UnionFind(int n) {
            next = new int[n];
            for(int i = 0; i < n; ++i)
                next[i] = i - 1;

            viz = new boolean[n];
            for(int i = 0; i < n; ++i)
                viz[i] = false;
        }

        int find(int x) {
            if (x == -1 || !viz[x]) {
                return x;
            }

            return next[x] = find(next[x]);
        }
    }

    /* Solutie:
    sortam activitatile dupa profit (descrescator)
    si o activitate incercam sa o asignam unui moment de timp
    cat mai apropiat de deadline
     */

    public static void main(String[] args) throws IOException {
        class Activitate {
            Integer profit;
            Integer limit;
            Integer index;

            Activitate(Integer p, Integer l, Integer i) {
                profit = p;
                limit = l;
                index = i;
            }

            Integer compareTo(Activitate a) {
                return profit.compareTo(a.profit);
            }
        }

        Scanner sc = new Scanner(new File(fin));
        final int n = sc.nextInt();

        /* Solutie:

        Sortam  descrescator dupa profit.
        Fiecare item va fi pus cat de aproape se poate de timpul de deadline.


        Demonstratie:

        Lema 1:

        Putem obtine o solutie daca mereu punem cat de aproape de deadline putem.

        Dem. lema 1:

        Fie S o solutie. O putem scrie ca o serie de operatii de genul
        insert X (luand itemele in ordinea desc. a  profitului).

        Presupunem ca exista un pas in crearea lui S in care un obiect O cu profitul P si deadline D
        nu a fost pus pe pozitia cea mai aproapiata de d
        (cu alte cuvinte, la momentul in care O a fost inserat pe pozitia poz[O]
        exista o pozitie p' libera, cu poz[O] < p' <= D)

        Demonstram ca se poate o obtine o solutie S' punand pe O pe pozitia p':

        la sfarsit, fie pozitia p' e goala, caz in care il putem muta pe O direct.
                    fie pe pozitia p' se afla un obiect O'.
                     Cum poz[O] < p', il putem muta pe obiectul O' pe poz[O] si pe O pe p'
                     si S' ramane in continuare valid.

        Am demonstrat ca mereu, daca e sa inseram un obiect, sa il punem pe cea mai mare pozitie posibila
        este o alegere care va duce la o solutie buna.



        Demonstratie ca este optim sa incercam sa le inseram descrescator dupa profit:

        fie S un candidat de solutie obtinut prin aplicarea algoritmului nostru, si fie S' o solutie buna(diferita de S).
        luand itemele in ordine descrescatoare, va exista un obiect O astfel incat acesta sa fie ales in S, dar nu si in S'.
        Fie P pozitia lui O in solutia S.
        Atunci fie in S' pozitia P e goala, caz in care il putem plasa pe O si obtinem o solutie mai buna.  (deci S' nu e solutie)
        Fie in S' pe pozitia P este un alt item O' != O, cu un profit mai mic.
        Atunci inlocuind pe O' cu O, vom obtine o solutie cu profit mai mare. (deci S' din nou, nu e solutie)

        => nu exista niciun astfel de obiect => S' = S
        qed

         */

        List<Activitate> activitati = new LinkedList<Activitate>();

        for(int i = 0; i < n; ++i) {
            int p = sc.nextInt();
            int t = sc.nextInt();

            activitati.add(new Activitate(p, t - 1, i));
        }
        sc.close();

        activitati.sort(Activitate::compareTo);

        UnionFind uf = new UnionFind(n);

        List<Activitate> taken = new LinkedList<Activitate>();

        for(int i = n - 1; i >= 0; --i) {
            Activitate now = activitati.get(i);
            int pos = uf.find(now.limit);

            if (pos != -1) {
                taken.add(now);
                uf.viz[pos] = true;
            }
        }

        Integer profit = 0;
        for(Activitate a: taken) {
            profit += a.profit;
        }

        taken.sort((activitate, t1) -> activitate.limit.compareTo(t1.limit));

        Writer writer = new FileWriter(fout);
        writer.write(profit.toString() + "\n");

        for(Activitate a: taken) {
            Integer i = a.index + 1;
            writer.write(i.toString() + " ");
        }
        writer.write("\n");

        writer.close();
    }
}
