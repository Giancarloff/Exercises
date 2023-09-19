package Java;

import java.util.Scanner;
import java.util.Set;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class countChars {
    
    public static void main(String[] args) {
        Scanner usrScan = new Scanner(System.in);

        String input = usrScan.nextLine();
        char[] strChars = input.toCharArray();
        Arrays.sort(strChars);
        strChars = new String(strChars).toCharArray();

        Map<Character, Integer> charCounts = new HashMap<Character, Integer>();
        int count = 1;
        char last_char = strChars[0];

        for (int i = 1; i < strChars.length; i++) {
            System.out.println("I: " + i + " strChars[i]: " + strChars[i] + " último caractere: " + last_char);
            if (last_char == strChars[i]) {
                count += 1;
            } else {
                charCounts.put(last_char, count);
                count = 1;
                last_char = strChars[i];
            }
        }
    
        charCounts.put(last_char, count);

        Set<Character> objs = charCounts.keySet();
        System.out.println("3rd : " + objs.toString());
        Character[] chars = objs.toArray(new Character[objs.size()]);

        Arrays.sort(chars);

        for (int i = 0; i < chars.length; i++) {
            System.out.println(chars[i] + " : " + charCounts.get(chars[i]));
        }

        usrScan.close();
    }

}
