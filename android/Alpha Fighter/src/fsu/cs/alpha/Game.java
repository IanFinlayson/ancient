/* Game.java
 * This class implements the main Game logic and game loop
 * it runs as its own thread so that we can have a responsive
 * GUI and still run as fast as we can
 */

package fsu.cs.alpha;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceHolder;

/**
 * main game object - is also a separate thread
 *
 */
class Game extends Thread {
	
	SurfaceHolder surface_holder;
	
	/* used to trigger the main thread */
	Handler handler;
	
	/* used for correct, time-based movement */
	long last_time;
	
	/* current state of the game */
	boolean running, paused;
	
    /* manager for all of the game objects */
    EntityManager entity_manager;
    
    /* background image */
    Bitmap bg;
    
    /* used to store win/loss message and name */
    public static String end_message;
    public static String player_name;
    
    /* context */
    public Context context;
	
    
	/**
	 * setup the game
	 * @param type "single" or "multi"
	 * @param sh the surface holder
	 * @param res the resources
	 * @param ctxt the context
	 * @param h a handler we can call to make crap run in the main UI thread
	 */
	Game(String type, SurfaceHolder sh, Resources res, Context ctxt, Handler h) {
		surface_holder = sh;
		handler = h;
		context = ctxt;
		bg = BitmapFactory.decodeResource(res, R.drawable.bg);
		
		/* setup entity manager */
        if(type.equals("single"))
        	entity_manager = new SingleEntityManager(this, res);
        else if(type.equals("multi"))
        	entity_manager = new MultiEntityManager(this, res);
        else
        	entity_manager = null;
	}
	
	
	/**
	 * start the game
	 */
	void begin( ) {
		last_time = System.currentTimeMillis( );
		running = true;
		paused = false;
	}
	
	/**
	 * stop the game thread from running
	 */
	void stop_running( ) {
		running = false;
	}
	
	/**
	 * stop the game!
	 * @param message message to show user like "You Won!"
	 */
	void end(String message) {
		running = false;
		
		end_message = message;
		
		/* why can't we add string to this thingy? */
		Message msg = new Message( );
		handler.sendMessage(msg);
	}
	
	/**
	 * pause the game
	 */
	void pause( ) {
		paused = true;
	}
	
	/**
	 * unpause the game
	 */
	void unpause( ) {
		paused = false;
		last_time = System.currentTimeMillis( );
	}
	
	
	/**
	 * update the game
	 */
	void doUpdate( ) {
		long now = System.currentTimeMillis( );
		
		/* time travel not allowed */
		if(last_time > now) return;
		
		/* calculate change in time (in seconds) */
		float delta = (float) ((now - last_time) / 1000.0);
		
		/* update last time */
		last_time = now;
		
		/* update all components with delta */
		entity_manager.updateAll(delta);
	}
	
	
	/**
	 * draw the game
	 * @param canvas canvas to draw to
	 */
	void doDraw(Canvas canvas) {
		/* draw the background */
		canvas.drawBitmap(bg, 0.0f, 0.0f, null);
		
		/* draw the entities */
		entity_manager.drawAll(canvas);
	}
	
	/* main game loop */
    /* (non-Javadoc)
     * @see java.lang.Thread#run()
     */
    public void run( ) {
        while (running) {
        	if(paused) continue;
            Canvas canvas = null;
            try {
            	canvas = surface_holder.lockCanvas(null);
            	doUpdate( );
                doDraw(canvas);
            } finally {
                if (canvas != null) {
                	surface_holder.unlockCanvasAndPost(canvas);
                }
            }
        }
    }	
}










