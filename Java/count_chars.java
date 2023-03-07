package Java;

import java.util.Scanner;
import java.util.Set;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class count_chars {
    
    public static void main(String[] args) {
        Scanner usrScan = new Scanner(System.in);

        String input = usrScan.nextLine();
        char[] strChars = input.toCharArray();
        Arrays.sort(strChars);
        strChars = new String(strChars).strip().toCharArray();

        // System.out.println(new String(strChars));

        Map<Character, Integer> charCounts = new HashMap<Character, Integer>();
        int count = 1;
        char last_char = strChars[0];

        for (int i = 1; i < strChars.length; i++) {
            if (last_char == strChars[i]) {
                count += 1;
            } else {
                charCounts.put(last_char, count);
                count = 1;
                last_char = strChars[i];
            } 
        }
    

        Set<Character> objs = charCounts.keySet();
        Character[] chars = objs.toArray(new Character[objs.size()]);

        Arrays.sort(chars);

        for (int i = 0; i < chars.length; i++) {
            System.out.println(chars[i] + " : " + charCounts.get(chars[i]));
        }

        usrScan.close();
    }

}
