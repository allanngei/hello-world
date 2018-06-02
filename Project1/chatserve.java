package CS372_Project1;

import java.io.*;
import java.net.*;
import java.util.*;

public class chatserve{
	
	static ServerSocket startup(int portno) throws IOException{
		ServerSocket result = new ServerSocket(portno);
		return result;
	}
	
	static void recvMessage(String[] message, InputStream in) throws Exception{
		BufferedReader in2 = new BufferedReader(new InputStreamReader(in));
		
		int available = 0;
		
		while(available == 0){
			available = in.available();
			continue;
		}
		
		message[0] = in2.readLine();
		
		System.out.println(message[0]);
	}
	
	static void sendMessage(PrintStream out, String handle, Scanner c){
		System.out.print(handle);
		String response = handle + c.nextLine();
		out.println(response);
	}
	
	public static void main(String[] args) throws Exception{
		
		String handle = "ServerAdmin> ";
		
		ServerSocket server = startup(50001);
		
		Scanner c = new Scanner(System.in);
				
		while (true) {
			Socket chat = server.accept();
//			Scanner c = new Scanner(System.in);
			OutputStream out = chat.getOutputStream();
			InputStream in = chat.getInputStream();
			
			PrintStream out2 = new PrintStream(out);
		
			String confirm = "Connection established!";
			out2.println(confirm);

			
			String quit = "/quit";
			
			while(!chat.isClosed()){
				String[] message = new String[1];
								

				recvMessage(message, in);
				
				if(message[0].endsWith(quit)){
					out2.println(quit);
					System.out.println("Chat session has terminated!");
					chat.close();
					break;
				}
				
				sendMessage(out2, handle, c);
			}
//			c.close();
			out.close();
			in.close();
		}

	}
}
