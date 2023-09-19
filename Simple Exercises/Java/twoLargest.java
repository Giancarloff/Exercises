package Java;

import java.util.Scanner;

public class twoLargest {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.print("Enter the number of integers: ");
        int n = input.nextInt();

        int[] v = new int[n];
        int largest1, largest2, i;

        largest1 = largest2 = Integer.MIN_VALUE;

        do {
            v[v.length - n] = input.nextInt();
            n -= 1;
        } while (n > 0);

        input.close();

        for (i = 0; i < v.length; i++) {
            if (largest1 < v[i]) {
                largest1 = v[i];
            }
        }
        boolean found = false;
        for (i = 0; i < v.length; i++) {
            if (largest2 < v[i] && v[i] < largest1) {
                largest2 = v[i];
                found = true;
            }
        }

        if (!found) {
            largest2 = largest1;
        }

        System.out.println("The two largest numbers are "+largest1+" and "+largest2+".");

    }
}