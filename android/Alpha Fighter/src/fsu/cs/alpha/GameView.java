/* GameView.java
 * This class is a custom View that holds our main drawing surface
 */

package fsu.cs.alpha;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.SurfaceHolder.Callback;
import android.widget.Toast;


/**
 * a new widget that plays our game
 *
 */
public class GameView extends SurfaceView implements Callback {
	
	public static int WIDTH = 320;
	public static int HEIGHT = 455;

    /* thread that does the work */
    Game game;
    
    /* activity that called us */
    GameActivity act;
    
    /**
     * stop playing the game
     */
    public void end( ) {
    	game.stop_running( );
    	/* game is now over - finish this activity */
    	try {
			game.join( );
		} catch (InterruptedException e) {
			Toast.makeText(game.context, "Error: Could not end game!", Toast.LENGTH_LONG).show( );
		}
		act.finish( );
    }
	
    /**
     * make a new game view
     * @param ctxt the context we are using
     * @param type "single" or "multi" for what you want
     * @param activity the GameActivity object called from
     */
    public GameView(final Context ctxt, String type, GameActivity activity) {
        super(ctxt);
        
        /* make sure we get callbacks on surface updates */
        SurfaceHolder holder = getHolder( );
        holder.addCallback(this);

        act = activity;
        
        /* create main game thread */
        game = new Game(type, holder, ctxt.getResources( ), ctxt, new Handler( ) {
            public void handleMessage(Message m) {
            	/* should run in main ui thread */
                end( );
				/* let them know status via toast */
                if(!Game.end_message.equals("")) {
                	/* get name and record result if needed */
                	if(Game.end_message.contains("Won")) {
                		MainActivity.won = true;
                	} else if(Game.end_message.contains("Lost")) {
                		MainActivity.lost = true;
                	} else {
                		Toast.makeText(ctxt, Game.end_message, Toast.LENGTH_SHORT).show( );
                	}
                }
            }
        });

        /* make sure we get key events */    
        setFocusable(true);
    }

    /* (non-Javadoc)
     * @see android.view.View#onWindowFocusChanged(boolean)
     */
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        if(!hasWindowFocus)
        	game.pause( );
    }


    /* (non-Javadoc)
     * @see android.view.SurfaceHolder.Callback#surfaceChanged(android.view.SurfaceHolder, int, int, int)
     */
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        /* ignore for now... */
    }

    
    /* (non-Javadoc)
     * @see android.view.SurfaceHolder.Callback#surfaceCreated(android.view.SurfaceHolder)
     */
    public void surfaceCreated(SurfaceHolder holder) {
        /* start 'er up */
        game.begin( );
        game.start();
    }

    
    /* (non-Javadoc)
     * @see android.view.SurfaceHolder.Callback#surfaceDestroyed(android.view.SurfaceHolder)
     */
    public void surfaceDestroyed(SurfaceHolder holder) {
        /* end the thread! */
        boolean retry = true;
        while (retry) {
            try {
                game.join();
                retry = false;
            } catch (InterruptedException e) {
            }
        }
    }
}


