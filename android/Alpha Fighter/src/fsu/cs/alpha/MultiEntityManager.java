/* MultiEntityManager.java
 * This class extends the EntityManager
 * to do updates and logic needed for multiplayer
 * mode including network connection
 */

package fsu.cs.alpha;

import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;

/* This entity allows us to black out the screen with a message
 * until the opponent is connected to us over the network
 */
class BlackOut extends Entity {

	String message;
	
	BlackOut(EntityManager em, String msg) {
		super(Entity.OTHER_TAG, em);
		message = msg;
	}
	
	public void draw(Canvas canvas) {
		Paint paint = new Paint( );
		paint.setTextSize(20.0f);
		paint.setColor(Color.WHITE);
		
		canvas.drawRGB(0, 0, 0);
		canvas.drawText(message, 5, 225, paint);
	}

	public void onCollide(Entity other) { }
	
}

/**
 * An entity manager for the networked game
 *
 */
public class MultiEntityManager extends EntityManager {

	/* the network connection */
	Network net;
	
	/* entity used for blacking out the screen */
	BlackOut black_out;
	
	boolean playing;
	
	/* refresh rate of network sending of coords */
	static final long REFRESH_DELAY = 125; /* ms */
	long last_send;
	
	Ship player;
	Ship enemy;
	
	/**
	 * create the networked entity manager
	 * @param g the game we are managing for
	 * @param res the resources so we can load pretty pictures
	 */
	MultiEntityManager(Game g, Resources res) {
		super(g, res);
		
		net = new Network( );
		playing = false;
		black_out = new BlackOut(this, "Waiting for opponenet to connect...");
		addEntity(black_out);
		last_send = 0;
	}
	
	
	/**
	 * begin playing the game now that we have an opponent
	 */
	public void begin( ) {
		playing = true;
		
		/* add the ships to the list */
		player = new Ship(true, this);
		addEntity(player);
		
		enemy = new Ship(false, this);
		addEntity(enemy);
	}
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.EntityManager#end(java.lang.String)
	 */
	public void end(String msg) {
		/* send quit to remote server and close it out */		
		if(net.isConnected( )) {
			net.send("quit");
			net.stop( );
		}
		
		/* super classes */
		super.end(msg);
	}
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.EntityManager#updateAll(float)
	 */
	public void updateAll(float elapsed_seconds) {
	
		/* try to get a message from the network */
		String received;
		if((received = net.recv( )) != null) {
			if(received.equalsIgnoreCase("ready")) {
				/* kill the blackout and begin playing! */
				black_out.kill( );
				black_out = null;
				begin( );
			} else if(received.equalsIgnoreCase("quit")) {
				/* end the game */
				game.end("Remote player quit");
			} else if(received.equalsIgnoreCase("fire")) {
				enemy.fire( );
			} else {
				/* must be a number */
				enemy.setX(Float.parseFloat(received));
			}
		}

		if(playing) {
			/* update player */
			if(player.updateControls( ))
				net.send("fire");
	
			long now = System.currentTimeMillis( );
			if((now - last_send) > REFRESH_DELAY) {
				/* send info across */
				net.send(Float.toString(player.getX( )));
				last_send = now;
			}
		}
		
		/* parent */
		super.updateAll(elapsed_seconds);
	}

}









