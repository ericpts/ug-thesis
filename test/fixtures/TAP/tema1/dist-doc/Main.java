import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.file.Files;
import java.util.*;
import java.util.regex.Pattern;

import static java.lang.Math.sqrt;

public class Main {

    static List<String> readFile(String fname) throws java.io.IOException {
        List<String> words = new ArrayList<String>();

        Scanner scanner = new Scanner(new FileReader(fname));

        Pattern pattern = Pattern.compile("[\\s\\.\\,]");
        scanner.useDelimiter(pattern);

        while (scanner.hasNext()) {
            String next = scanner.next();
            if (!next.isEmpty())
                words.add(next);
        }

        return words;
    }

    static Map<String, Integer> wordDict(List<String> words) {
        Map<String, Integer> ret = new HashMap<String, Integer>();

        for (String word: words) {

            Integer cnt = 1;

            if (ret.containsKey(word))
                cnt += ret.get(word);

            ret.put(word, cnt);
        }

        return ret;
    }

    static double dcos(String F1, String F2) throws java.io.IOException{
        Map<String, Integer> dict1 = wordDict(readFile(F1)),
                dict2 = wordDict(readFile(F2));

        System.out.println(dict1.toString());
        System.out.println(dict2.toString());

        Set<String> allWords = new HashSet<>();
        allWords.addAll(dict1.keySet());
        allWords.addAll(dict2.keySet());

        int Sup = 0, Sdown1 = 0, Sdown2 = 0;

        for(String word: allWords) {
            int ap1 = dict1.getOrDefault(word, 0),
                    ap2 = dict2.getOrDefault(word, 0);

            Sup += ap1 * ap2;

            Sdown1 += ap1 * ap1;
            Sdown2 += ap2 * ap2;
        }

        double total = Sup / (sqrt(Sdown1) * sqrt(Sdown2));
        return total;
    }

    public static void main(String[] args) throws java.io.IOException{

        String F1 = args[0],
                F2 = args[1];

        System.out.println(dcos(F1, F2));
    }
}
