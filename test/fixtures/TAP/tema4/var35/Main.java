package com.stavindustries;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class Main {

    final static String fin = "date.in";
    final static String fout = "date.out";

    /*
    DP[i][j] = am restrictionat jocul la subsecventa [i..j]

    Care este diferenta optima dintre jucatorul 1 si jucatorul 2
    (in functie de cine este la mutare)

    facem inductie dupa lungimea secventei:

    pentru L = 1, este un singur element de luat
    (si el va face scorul pt. cine este la mutare)

    presupunem DP calculata corect pentru secvente de lungime 1, 2, ..., L

    vrem sa calculam DP pentru o subsecventa i..j de lungime (L + 1).

    In functie de paritatea lungimii, va fi fie randul jucatorului 1 sau 2.

    In caz ca este jucatorul 1:
        fie ia elementul din stanga si reduce la DP[i + 1][j]
        fie din dreapta si reduce la DP[i][j - 1]
        deoarece incearca sa maximizeze diferenta, va lua maximul pentru cele doua optiuni

    In caz ca este jucatorul 2:
        fie ia din stanga si trece in DP[i + 1][j]
        fie din dreapta si trece in DP[i][j - 1]
        fie valoarea luata @val, atunci scorul sau se va apropria de scorul jucatorului 1
        cu @val, sau cu alte cuvinte diferenta dintre ei va scadea cu @val, asa ca ar ajunge in starea
        DP[a][b] - val
        Deoarece incearca sa minimizeze diferenta, va lua minimul dintre cele doua optiuni

     */

    public static void main(String[] args) throws FileNotFoundException {

        Scanner sc = new Scanner(new File(fin));

        final int n = sc.nextInt();

        int[] arr = new int[n];
        for(int i = 0; i < n; ++i) {
            arr[i] = sc.nextInt();
        }
        sc.close();

        int[][] DP = new int[n][n];
        {
            int sign;
            if (n % 2 == 1)
                sign = 1;
            else
                sign = -1;
            for (int i = 0; i < n; ++i) {
                DP[i][i] = sign * arr[i];
            }
        }

        for(int len = 2; len <= n; ++len) {
            boolean isPlayer1;

            if (len % 2 == n % 2)
                isPlayer1 = true;
            else
                isPlayer1 = false;

            for(int i = 0, j = len - 1; j < n; ++i, ++j) {

                if (isPlayer1) {
                    int opt1 = arr[i] + DP[i + 1][j];
                    int opt2 = arr[j] + DP[i][j - 1];
                    DP[i][j] = max(opt1, opt2);
                } else {
                    int opt1 = - arr[i] + DP[i + 1][j];
                    int opt2 = - arr[j] + DP[i][j - 1];
                    DP[i][j] = min(opt1, opt2);
                }
            }
        }

        int L = 0, R = n - 1;

        int compScore = 0;
        int playerScore = 0;


        Scanner in = new Scanner(System.in);

        while (L <= R) {

            if (DP[L][R] == arr[L] + DP[L + 1][R]) {
                System.out.println("Voi lua din stanga " + Integer.toString(arr[L]));
                compScore += arr[L];
                L++;
            } else {
                System.out.println("Voi lua din dreapta " + Integer.toString(arr[R]));
                compScore += arr[R];
                R--;
            }

            System.out.println("Starea jocului: ");
            System.out.println("Scor PC: " + Integer.toString(compScore) + " Scor om: " + Integer.toString(playerScore));

            for(int i = L; i <= R; ++i) {
                System.out.print(Integer.toString(arr[i]) + " ");
            }
            System.out.print("\n");

            System.out.println("Calculatorul va castiga cu un avantaj de cel putin " + Integer.toString(compScore - playerScore + DP[L][R]));


            if (!(L <= R))
                break;

            System.out.print("Mutati L/R: ");

            String move = in.next();
            if (move.contains("L")) {
                playerScore += arr[L];
                L++;
            } else {
                playerScore += arr[R];
                R--;
            }
        }

        System.out.println("Scor PC: " + Integer.toString(compScore) + " Scor om: " + Integer.toString(playerScore));
    }
}
