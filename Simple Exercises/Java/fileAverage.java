package Java;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class fileAverage {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: java file_average.java file_name");
        } else {

            File fileObj = new File(args[0]);
            ArrayList<Double> numList = new ArrayList<Double>();
            boolean add;
            String line = "";
            Double num = 0d;

            try {
                Scanner reader = new Scanner(fileObj);
                while (reader.hasNextLine()) {
                    add = true;
                    line = reader.nextLine();
                    try {
                        if (!line.equals(new String("NaN"))) {
                            num = Double.parseDouble(line);
                        } else {
                            System.out.println("Found NaN! Skipping.");
                            add = false;
                        }
                    } catch (NumberFormatException e) {
                        System.out.println("Skipping non-numeric line!");
                        add = false;
                    } catch (NullPointerException e) {
                        System.out.println("No numbers! Skipping this line.");
                        add = false;
                    }

                    if (add) {
                        System.out.println("Adding " + Double.toString(num));
                        numList.add(num);
                    }
                }
                reader.close();
            } catch (FileNotFoundException e) {
                System.out.println(e.getMessage());
                e.printStackTrace();
            }
            int index = 0;
            int numListSize = numList.size();
            Double sum = 0d;
            while (index < numListSize) {
                sum += numList.get(index);
                index++;
            }
            System.out.println("Sum: " + Double.toString(sum));
            System.out.println("NumListSize: " + Integer.toString(numListSize));
            if (numListSize > 0)
                System.out.println("Average of file: " + Double.toString(sum/numList.size()));
        }
    }   
}
