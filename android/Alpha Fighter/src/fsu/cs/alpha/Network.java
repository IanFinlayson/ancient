/* Network.java
 * This class is used to encapsulate a network connection
 * for multiplayer game
 */

package fsu.cs.alpha;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.LinkedList;
import java.util.Queue;

import android.widget.Toast;

/**
 * Network object that has a socket and lets us talk to
 * the opponent
 */
public class Network implements Runnable {
	
	/* where the server is running (Pete's computer :P) */
	private static String HOST_NAME = "michaelangelo.cs.fsu.edu";
	private static int PORT = 5656;
	
	/* connection to the remote host */
	Socket socket;
	
	/* input and output streams */
	PrintWriter out;
	BufferedReader in;
	
	/* listen in on the socket */
	boolean listen, connected;
	
	/* buffer of messages coming from the socket */
	Queue<String> in_buffer;
	
	/* report errors */
	void error(String msg, Exception e) {
		System.out.println(msg);
		e.printStackTrace(System.out);
		connected = false;
		Toast.makeText(null, "Error: Could not end game!", Toast.LENGTH_LONG).show( );
	}
	
	/**
	 * Initialize the network
	 */
	Network( ) {
		try {
			/* connect */
			socket = new Socket(HOST_NAME, PORT);
			
			/* setup I/O channels across socket */
			out = new PrintWriter(socket.getOutputStream( ), true);
			in = new BufferedReader(new InputStreamReader(socket.getInputStream( )));
		
			in_buffer = new LinkedList<String>( );
			
			listen = true;
			connected = true;
			
			/* spawn a thread to listen on it */
			(new Thread(this)).start( );
		
		} catch (UnknownHostException e) {
			error("Can't connect! Is server up?", e);
		} catch (IOException e) {
			error("Couldn't get I/O for the connection!", e);
		}
	}
	
	
	/**
	 * stop the connection and close all the stuff out
	 */
	public void stop( ) {
		listen = false;
		try {
			in.close( );
			out.close( );
			socket.close( );
		} catch (IOException e) {
			error("Couldn't close I/O connections!", e);
		}
	}

	
	/* (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	public void run( ) {
		String input = "";
		while(listen) {
			try {
				input = in.readLine( );
			} catch(IOException e) {
				error("Couldn't read from socket!", e);
			}
		
			/* enqueue */
			in_buffer.offer(input);
		}
	}
	
	
	/**
	 * receive a string or null if there is none
	 * @return the string we read or null for no input now
	 */
	String recv( ) {
		if(!connected)
			return "quit";
		
		return in_buffer.poll( );
	}


	/**
	 * sends a string across the network
	 * @param msg
	 */
	void send(String msg) {
		out.println(msg);
	}

	/**
	 * @return whether the connection is up and going
	 */
	public boolean isConnected( ) {
		return connected;
	}
}







