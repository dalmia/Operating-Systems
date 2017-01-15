import java.net.*;
import java.io.*;
import java.util.*;

public class EchoClient{
    public static void main(String[] args){
        try {
            /* make connection to server socket */
            Socket socket = new Socket("127.0.0.1", 6010);

            /* setup reading and writing to the socket */
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);

            InputStream in = socket.getInputStream();
            BufferedInputStream is = new BufferedInputStream(in);

            /* byte array to store the input from BufferedInputStream */
            byte[] contents = new byte[1024];
            int len;
            String input;

            /* keep running until null encountered */
            while( (input = reader.readLine()) != null){
                /* write string to socket */
                writer.write(input);
                writer.flush();

                /* read string from socket */
                String line;
                len = is.read(contents);
                if(len == -1) break;
                line = new String(contents, 0, len);
                System.out.println(line);

                /* exit on 'bye' */
                if(line.toLowerCase().contains("bye")) break;
            }

            /* close the socket connection */
            socket.close();
        }catch(IOException ioe){
            System.err.println(ioe);
        }
    }
}
