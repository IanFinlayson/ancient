/* OptionsActivity.java
 * The Options screen.  For now this is just place holder.
 */

package fsu.cs.alpha;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.RadioButton;


/**
 * an activity screen for choosing game options
 *
 */
public class OptionsActivity extends Activity {
	
	
    /* (non-Javadoc)
     * @see android.app.Activity#onCreate(android.os.Bundle)
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().requestFeature(Window.FEATURE_NO_TITLE); 
        setContentView(R.layout.options);
        
        /* get buttons */
        RadioButton tilt_button = (RadioButton) findViewById(R.id.tilit_input);
        RadioButton track_button = (RadioButton) findViewById(R.id.trk_input);
        CheckBox sound_box = (CheckBox) findViewById(R.id.snd_output);
        CheckBox music_box = (CheckBox) findViewById(R.id.music_output);
        
        /* setup initial values */
        tilt_button.setChecked(GameActivity.tilt_input);
        track_button.setChecked(GameActivity.trk_ball_input);
        sound_box.setChecked(GameActivity.play_sound);
        music_box.setChecked(GameActivity.play_music);
        
        /* make sure we don't do both kinds of input */
        assert(GameActivity.tilt_input ^ GameActivity.trk_ball_input);
        
        tilt_button.setOnCheckedChangeListener(new RadioButton.OnCheckedChangeListener( ) {
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				GameActivity.tilt_input = isChecked;
				GameActivity.trk_ball_input = !isChecked;
			}
        });
        
        track_button.setOnCheckedChangeListener(new RadioButton.OnCheckedChangeListener( ) {
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				GameActivity.trk_ball_input = isChecked;
				GameActivity.tilt_input = !isChecked;
			}
        });

        sound_box.setOnCheckedChangeListener(new CheckBox.OnCheckedChangeListener( ) {
			public void onCheckedChanged(CompoundButton buttonView,	boolean isChecked) {
				GameActivity.play_sound = isChecked;
			}
        });
        
        music_box.setOnCheckedChangeListener(new CheckBox.OnCheckedChangeListener( ) {
			public void onCheckedChanged(CompoundButton buttonView,	boolean isChecked) {
				GameActivity.play_music = isChecked;
			}
        });        
    }
}
