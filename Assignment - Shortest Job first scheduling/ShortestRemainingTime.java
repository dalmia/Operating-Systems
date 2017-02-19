import java.util.List;


public class ShortestRemainingTime extends AllocationStrategy {

    public ShortestRemainingTime(List<Job> jobs) {
        super(jobs);
    }

    public void run() {

    }

    public void run(List<Job> jobList) {

        float avgTurnArroundTime = 0;
        float avgWaitigTime = 0;
        int n;
        n = jobList.size();
        int p[] = new int[n];
        int at[] = new int[n];
        int bt[] = new int[n];
        int bt2[] = new int[n];
        int wt[] = new int[n];
        int tat[] = new int[n];
        int count = 0;
        double tempWT=0;
        for (Job job:jobList) {
            p[count] = count;
            at[count] = job.getArrivalTime();
            bt[count] = job.getCpuTime();
            bt2[count] = bt[count];
            count++;
        }
        int tbt = 0;
        for (int i = 0; i < n; i++) {
            tbt = tbt + bt[i];
        }
        int time[] = new int[tbt];
        int k = 0;
        int q2 = 0;

        System.out.println("============================================ ");
        System.out.println("Process ID | Turnaround time | Waiting time ");
        System.out.println("============================================ ");
        int counter=1;
        for (int i = 0; i < tbt; i++) {

            int q = Min(bt, at, tbt, i, n);

            if (q != q2) {
                time[k++] = i;

                wt[q] = i;
                tat[q] = i + bt[q];
                counter++;
                tempWT+=wt[q];
            }
            avgWaitigTime+= wt[q];
            avgTurnArroundTime+=tat[q];
            bt[q] = bt[q] - 1;
            q2 = q;

            System.out.println( (p[q]+1) +"\t|\t"+tat[q]+"\t|\t"+wt[q]);
            System.out.println("----------------------------------------");

        }
        time[k] = tbt;
        System.out.println();
        System.out.print("0\t");

        for (int i = 0; i <= k; i++) {
            System.out.print(time[i] + "\t");
        }
        System.out.println("\n============================================ ");
        System.out.println("Avg WT||TAT::"+tempWT/counter+"|"+avgTurnArroundTime/counter);
        System.out.println("============================================ ");

    }

    public int Min(int b[], int a[], int tbt, int r, int n) {
        int j = 0;
        int min = tbt;
        for (int i = n - 1; i != 0; i--) {
            if (b[i] < min && b[i] > 0 && r != a[i]) {
                min = b[i];
                j = i;
            }
        }
        return j;
    }

}
