import java.net.*;
import java.io.*;

public class EchoServer{
    public static void main(String[] args){
        try{
            ServerSocket sock = new ServerSocket(6010);

            /* listen for connections */
            while(true){
                Socket client = sock.accept();
                InputStream in = client.getInputStream();
                BufferedInputStream is = new BufferedInputStream(in);
                PrintWriter writer = new PrintWriter(client.getOutputStream(), true);

                byte[] contents = new byte[1024];
                int len;

                while(true){
                    /* read input from socket */
                    len = is.read(contents);
                    
                    /* echo back to client */
                    String line = new String(contents, 0, len);
                    writer.write("Server: " + line);
                    writer.flush();

                    if(line.toLowerCase().compareTo("bye")==0) break;
                }

                /* close the socket and resume listening for connections */
                client.close();
            }
        }catch(IOException ioe){
            System.err.println(ioe);
        }
    }
}
