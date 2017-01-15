import java.net.*;
import java.io.*;

public class PoemServer{
    public static void main(String[] args){
        try{
            ServerSocket sock = new ServerSocket(6017);

            /* listen for connections */
            while(true){
                Socket client = sock.accept();
                PrintWriter writer = new PrintWriter(client.getOutputStream(), true);

                /* write poem to client */
                String s = "a a a a a\n"
                         + "b b b b b b b\n"
                         + "c c c c c";
                writer.println(s);

                /* close the socket and resume listening for connections */
                client.close();
            }
        }catch(IOException ioe){
            System.err.println(ioe);
        }
    }
}
