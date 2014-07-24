/* GameActivity.java
 * This class implements the Game Activity screen
 */

package fsu.cs.alpha;

import android.app.Activity;
import android.hardware.SensorListener;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.widget.LinearLayout;


/**
 * Activity for the main game
 *
 */
public class GameActivity extends Activity implements SensorListener {
	
	/* The following bool's are set in the Options of the Main Activity
	 * 		and effect where input is accepted
	 */
	static public boolean tilt_input = true;
	static public boolean trk_ball_input = false;
	static public boolean play_sound = true;
	static public boolean play_music = true;
	
	GameView game_view;
	SensorManager mSensorManager;
	public static MediaPlayer mp = null;
	
    /* (non-Javadoc)
     * @see android.app.Activity#onCreate(android.os.Bundle)
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().requestFeature(Window.FEATURE_NO_TITLE); 
        setContentView(R.layout.game);
        if(play_music){
        	mp = MediaPlayer.create(this, R.raw.bg);
        	mp.start();
        }
        
        /* create our game view widget */
        game_view = new GameView(getBaseContext( ), getIntent( ).getType( ), this);
 
        /* add the custom surface view to the layout */
        ((LinearLayout) findViewById(R.id.Layout)).addView(game_view);
        
        /* Register Sensor Listener*/
        if(tilt_input) {
        	mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        	mSensorManager.registerListener(this,
				  SensorManager.SENSOR_ORIENTATION,
			      SensorManager.SENSOR_DELAY_GAME);
        }
    }
    
    /* Creates the menu items */

	/* (non-Javadoc)
	 * @see android.app.Activity#onCreateOptionsMenu(android.view.Menu)
	 */
	public boolean onCreateOptionsMenu(Menu menu) {
		boolean result = super.onCreateOptionsMenu(menu);
		menu.add(0, Menu.FIRST, 1, "Cancel").setIcon(android.R.drawable.ic_menu_close_clear_cancel);
		return result;
	}
	/* Action methods for the menu items */
	/* (non-Javadoc)
	 * @see android.app.Activity#onOptionsItemSelected(android.view.MenuItem)
	 */
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
	    case Menu.FIRST:
	    	/* end the thread */
	    	game_view.end( );
	    	
	    	game_view.game.entity_manager.end("");
	    	if(tilt_input) {
	    		mSensorManager.unregisterListener(this);
	    	}
	    	this.finish();
	    	return true;    	
		}
		return true;
	}
    

    /* (non-Javadoc)
     * @see android.app.Activity#onResume()
     */
    public void onResume( ) {
    	super.onResume();
    	if(tilt_input) {
    		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
    		mSensorManager.registerListener(this,
				  SensorManager.SENSOR_ORIENTATION,
			      SensorManager.SENSOR_DELAY_GAME);
    	}
    	if(play_music){
        	mp.start();
        }
        game_view.game.resume( );
    }
    

    /* (non-Javadoc)
     * @see android.app.Activity#onPause()
     */
    public void onPause( ) {
    	super.onPause();
    	if(tilt_input) {
    		mSensorManager.unregisterListener(this);
    	}
    	if(play_music){
        	mp.pause();
        }
    	game_view.game.pause( );
    }
    
    /* (non-Javadoc)
     * @see android.app.Activity#onDestroy()
     */
    public void onDestroy( ) {
    	/* end the thread */
    	game_view.end( );
    	
    	game_view.game.entity_manager.end("");
    	if(tilt_input) {
    		mSensorManager.unregisterListener(this);
    	}
    	if(play_music){
        	mp.stop();
        	mp.release();
        }
    	super.onDestroy( );
    }
    
    /* (non-Javadoc)
     * @see android.app.Activity#onKeyDown(int, android.view.KeyEvent)
     */
    public boolean onKeyDown(int key_code, KeyEvent msg) {
    	/* add it to the buffer only if we care about it */
    	switch(key_code) {
		case KeyEvent.KEYCODE_DPAD_CENTER:
	    	KeyBuffer.bufferKey(key_code);
	    	return true;
		case KeyEvent.KEYCODE_DPAD_LEFT:
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			if(trk_ball_input) {
				KeyBuffer.bufferKey(key_code);
			}
    	}
    	
    	/* do not want */
    	return false;
    }


	/* (non-Javadoc)
	 * @see android.hardware.SensorListener#onAccuracyChanged(int, int)
	 */
	public void onAccuracyChanged(int sensor, int accuracy) {
		
	}

	/* (non-Javadoc)
	 * @see android.hardware.SensorListener#onSensorChanged(int, float[])
	 */
	public void onSensorChanged(int sensor, float[] values) {
		assert tilt_input;
		if(sensor == SensorManager.SENSOR_ORIENTATION) {
			KeyBuffer.set_ship_speed(values[2]);
		}
	}
}
