package fsu.cs.alpha;

import android.app.Activity;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.Window;

/**
 * Activity for the about screen
 */
public class AboutActivity extends Activity {
	public static MediaPlayer mp;
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().requestFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.about);
		if(GameActivity.play_music){
			mp = MediaPlayer.create(this, R.raw.theme);
			mp.start();
		}
	}
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onResume()
	 */
	public void onResume( ) {
		super.onResume();
		if(GameActivity.play_music){
			mp.start();
        }
	}
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onPause()
	 */
	public void onPause( ) {
		super.onPause();
		if(GameActivity.play_music){
        	mp.pause();
        }
	}
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onDestroy()
	 */
	public void onDestroy( ) {
		if(GameActivity.play_music){
			mp.stop();
			mp.release();
		}
    	super.onDestroy( );
	}
}
