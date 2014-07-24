/* MainActivity.java
 * this class implements the main menu screen
 */

package fsu.cs.alpha;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageButton;

/**
 * activity for the main menu
 *
 */
public class MainActivity extends Activity {
	
	/* used to trigger name dialog, db store */
	public static boolean won = false;
	public static boolean lost = false;
	

    /* (non-Javadoc)
     * @see android.app.Activity#onCreate(android.os.Bundle)
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        
        setContentView(R.layout.main);
        
        /* reference to 'this' (for handlers) */
        final MainActivity ms = this;
        
        /* single player game handler */
        final ImageButton button = (ImageButton) findViewById(R.id.single);
        button.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v) {
	            Intent i = new Intent(ms, GameActivity.class);
	            i.setType("single");
	            ms.startActivity(i);
            }
        });
        
        /* multiplayer game handler */
        final ImageButton button2 = (ImageButton) findViewById(R.id.multi);
        button2.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v) {
	            Intent i = new Intent(ms, GameActivity.class);
	            i.setType("multi");
	            ms.startActivity(i);
            }
        });
                
        /* quit handler */
        final ImageButton button3 = (ImageButton) findViewById(R.id.quit);
        button3.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v) {
              ms.finish( );
            }
        });
    }
    
    
    public void onResume( ) {
        super.onResume( );
    	
        /* do win/loss stuff if needed */
    	if(won) {
    		NameDialog n = new NameDialog(this, "Congratulations!!");
    		n.show( );
    		
    		StatsDatabase db = new StatsDatabase(this);
    		db.open( );
    		db.recordOneWin(Game.player_name);
    		db.close( );
    		won = false;
    	}
    	
    	if(lost) {
    		NameDialog n = new NameDialog(this, "You have Lost!!");
    		n.show( );
    		
    		StatsDatabase db = new StatsDatabase(this);
    		db.open( );
    		db.recordOneLoss(Game.player_name);
    		db.close( );     
    		lost = false;
    	}
    }
    


    /* Creates the menu items */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		boolean result = super.onCreateOptionsMenu(menu);
		menu.add(0, Menu.FIRST, 1, "High Scores").setIcon(android.R.drawable.star_big_on);
		menu.add(0, Menu.FIRST+1, 1, "Options").setIcon(android.R.drawable.ic_menu_preferences);
		menu.add(0, Menu.FIRST+2, 1, "About").setIcon(android.R.drawable.ic_menu_info_details);
		return result;
	}
	/* Action methods for the menu items */
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
	    case Menu.FIRST:
            startActivity(new Intent(this, StatsActivity.class));
	    	return true;
	    case Menu.FIRST+1:
            startActivity(new Intent(this, OptionsActivity.class));
	    	return true;
	    case Menu.FIRST+2:
            startActivity(new Intent(this, AboutActivity.class));
	    	return true;
		}
		
		return true;
	}
	
}

