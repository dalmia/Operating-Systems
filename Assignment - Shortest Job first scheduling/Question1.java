
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Question1 {

    public static void main(String[] args) {

        int quantum=20;
        String filename = "testing.txt";

        BufferedReader br = null;

        try {

            String sCurrentLine;

            br = new BufferedReader(new FileReader(filename));
            System.out.println("processId  arrivalTime  cpuTime");

            List<Job> jobList = new ArrayList<Job>();
            while ((sCurrentLine = br.readLine()) != null) {

                String a[] = sCurrentLine.split(",");
                int processId = new Integer(a[0]);
                int arrivalTime = new Integer(a[1]);
                int cpuTime = new Integer(a[2]);

                Job job = new Job(processId, arrivalTime, cpuTime);
                jobList.add(job);

                System.out.println(processId + "\t\t" + arrivalTime + "\t\t" + cpuTime);
            }

            ShortestRemainingTime srtf = new ShortestRemainingTime(jobList);
            srtf.run(jobList);

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) br.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

}
