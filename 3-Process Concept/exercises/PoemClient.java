import java.net.*;
import java.io.*;

public class PoemClient{
    public static void main(String[] args){
        try {
            /* make connection to server socket */
            Socket socket = new Socket("127.0.0.1", 6017);

            InputStream in = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(in));

            /* read string from socket */
            String line;
            while( (line = reader.readLine()) != null)
                System.out.println(line);

            /* close the socket connection */
            socket.close();
        }catch(IOException ioe){
            System.err.println(ioe);
        }
    }
}
