/* KeyBuffer.java
 * This class allows the main event thread (which can catch events but not act)
 * to communicate with the game thread (which can act on events but not catch them)
 * there may be some kind of synchronization issues here, but well worry about that later
 */

package fsu.cs.alpha;

import java.util.LinkedList;
import java.util.Queue;

/**
 * a global buffer to hold the keys as we read them in this is
 * so we can commune between the UI thread which may read keys
 * but can't mess with game stuff and the game thread which can
 * not read from the keys but must know what they are
 * we also put the ship speed from the tilt in here too
 */
public class KeyBuffer {
	static Queue<Integer> key_buffer = new LinkedList<Integer>( );
	
	static float new_ship_speed = 123456.0f;
	/**
	 * shet ship speed when using tilt controls
	 * @param val the value (will be scaled later)
	 */
	public static void set_ship_speed(float val) {
		new_ship_speed = val;
	}
	/**
	 * get the ship speed as set previously
	 * @return the ship speed of course
	 */
	public static float get_ship_speed( ) {
		return new_ship_speed;
	}
	
	/**
	 * buffer a key press
	 * @param key_code the code as returned from androoid
	 */
	public static void bufferKey(int key_code) {
		key_buffer.offer(new Integer(key_code));
	}
	
	/**
	 * @return key code or -1 if no new key is here
	 */
	public static int pollKey( ) {
		Integer i = key_buffer.poll( );
		
		if(i == null)
			return -1;
		else
			return i.intValue( );
	}
}



