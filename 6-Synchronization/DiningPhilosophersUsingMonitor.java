/** Solution to Dining Philosophers problem
 *  Does not cause deadlock
 *  May lead to deadlock
 *
 *  @author Barbara Lerner
 *  https://www.mtholyoke.edu/~blerner/cs322/Examples/Lecture09/DiningPhilosophersUsingMonitor.java
 */
import java.util.Random;

public class DiningPhilosophersUsingMonitor(){
    private static final int NUM_PHILOSOPHERS = 5;

    public static void main(String args[]){
        Philosopher[] philosophers = new Philosopher[NUM_PHILOSOPHERS];

        // Monitors ensure that the philosophers pick up chopsticks at the
        // same time
        PhilosopherMonitor monitor = new PhilosopherMonitor(NUM_PHILOSOPHERS);
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            philosophers[i] = new Philosopher(i, monitor);
            new Thread(philosophers[i]).start();
        }
    }
}

/** A philosopher either thinks or eats. For eating, it needs to pick up
 *  chopsticks from its both sides sequentially. Hence, when one philosopher is
 *  eating, none of its neighbors can.
 */
class Philosopher extends implements Runnable{
    // Varies how long a philosopher eats and thinks
    private Random numGenerator = new Random();

    // philosopher's unique id
    private int id;

    // Controls when a philosopher can pick up chopsticks
    private PhilosopherMonitor monitor;

    /**
     * Contruct a new philosopher
     */
    public Philosopher(int id, PhilosopherMonitor monitor){
        this.id = id;
        this.monitor = monitor;
    }

    /**
     * Repeatedly think, pick up chopsticks, eat and put down chopsticks
     */
    public void run(){
        try{
            while(true){
                think();
                monitor.pickUp(i);
                eat();
                monitor.putDown(i);
            }
        } catch(InterruptedException e){
            System.out.println("Philosopher " + id + " was interrupted.\n");
        }
    }

    /**
     * Sleep for a random amount to time to model thinking
     */
    private void think(int id) throws InterruptedException(){
        System.out.println("Philosopher " + id + " is thinking.")
        System.out.flush();
        Thread.sleep(numGenerator.nextInt(10));
    }

    /**
     * Sleep for a random amount of time to model eating
     */
    public void eat(int id) throws InterruptedException{
        Thread.sleep(numGenerator.nextInt(10));
    }
}

/** Make sure philosopher picks up chopsticks only when no other neighbor is
 * eating.
 */
class PhilosopherMonitor{
    // The states of the philosoper can be in any of these
    private enum State {THINKING, HUNGRY, EATING};

    // The state of each philosopher
    private State[] philosopherState;

    /**
     * Creates a monitor for the number of philosophers passed
     */
    public PhilosopherMonitor(int numPhilosophers){
        philosopherState = new State[numPhilosophers];
        for (int i = 0; i < numPhilosophers; i++){
            philosopherState[i] = State.THINKING;
        }
    }

    /**
     * A philosopher picks up both chopsticks, waits if either of its neighbors
     * is eating
     */
    public synchronized void pickUp(int philosopherId) throws InterruptedException{
        philosopherState[philosopherId] = State.HUNGRY;
        System.out.println("Philosopher " + philosopherId + " is hungry.")
        System.out.flush();

        while(neighborEating(philosopherId)){
            wait();
        }

        philosopherState[philosopherId] = State.EATING;
        System.out.println("Philosopher " + philosopherId + " is eating");
        System.out.flush();
    }

    /**
     * Return true if some neighbor is currently eating
     */
    private boolean neighborEating(int philosopherId){
        int numPhilosophers = philosopherState.length;
        if(philosopherState[(philosopherId + 1) % numPhilosophers] == State.EATING){
            return true;
        }

        if(philosopherState[(philosopherId + numPhilosophers - 1) % numPhilosophers] == State.EATING){
            return true;
        }

        return false;
    }

    /**
     * Put down both chopsticks. Notify all waiting philosophers
     */
    public synchronized void putDown(int philosopherId) throws InterruptedException{
        philosopherState[philosopherId] = State.THINKING;
        notifyAll();
    }
}
