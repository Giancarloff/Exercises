package Java;
import java.util.Scanner;

public class primes_upto {

    public static boolean is_prime(int num) {

        if (num == 0 || num == 1) {
            return false;
        }

        int i = 2;
        int root_num = (int) Math.sqrt(num);
        for (i = 2; i <= root_num; i++) {
            if (num % i == 0) {
                return false;
            }
        }

        return true;
    }

    public static void main(String args[]) {
        Scanner usrScan = new Scanner(System.in);

        System.out.println("Give me a number: ");
        int num = usrScan.nextInt();
        int i = 2;

        System.out.println("Primes up to " + num + ": ");
        while (i <= num) {
            if (is_prime(i)) {
                System.out.println(i);
            }
            i++;
        }

        usrScan.close();
    }

}
