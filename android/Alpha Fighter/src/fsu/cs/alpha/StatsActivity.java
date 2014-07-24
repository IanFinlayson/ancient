package fsu.cs.alpha;

import android.app.Activity;


import java.text.NumberFormat;

import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Window;
import android.widget.LinearLayout;
import android.widget.TextView;


/**
 * activity for viewing stats
 *
 */
public class StatsActivity extends Activity {

	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.stats);
       
        /* open the database */
        StatsDatabase db = new StatsDatabase(this);

        /* container for stats */
        LinearLayout container = (LinearLayout) findViewById(R.id.StatList);
        container.setBackgroundColor(Color.parseColor("#34ffffff"));
        
        /* make the headings */
		LinearLayout headings = new LinearLayout(this);
		
		TextView heading_name = new TextView(this);
		heading_name.setTextColor(Color.WHITE);
		heading_name.setTextSize(20.0f);
		heading_name.setText("Name");
		heading_name.setWidth(140);
		headings.addView(heading_name);
		
		TextView heading_wins = new TextView(this);
		heading_wins.setTextColor(Color.WHITE);
		heading_wins.setTextSize(20.0f);
		heading_wins.setText("W");
		heading_wins.setWidth(35);
		headings.addView(heading_wins);
		
		TextView heading_losses = new TextView(this);
		heading_losses.setTextColor(Color.WHITE);
		heading_losses.setTextSize(20.0f);
		heading_losses.setText("L");
		heading_losses.setWidth(35);
		headings.addView(heading_losses);
		
		TextView heading_pct = new TextView(this);
		heading_pct.setTextColor(Color.WHITE);
		heading_pct.setTextSize(20.0f);
		heading_pct.setText("%");
		heading_pct.setWidth(35);
		headings.addView(heading_pct);
	
		
		container.addView(headings);
        
        /* loop through the stats */
        db.open( );
        Cursor c = db.getAllStats( );
        if (c.moveToFirst( ))
        {
            do {            	
            		/* make a linear layout for this row */
            		LinearLayout row = new LinearLayout(this);
            		
            		/* make a textview for each column */
            		TextView name = new TextView(this);
            		name.setText(c.getString(1));
            		name.setTextColor(Color.LTGRAY);
            		name.setWidth(140);
            		row.addView(name);
            		
            		TextView wins = new TextView(this);
            		wins.setText(c.getString(2));
            		wins.setTextColor(Color.LTGRAY);
            		wins.setWidth(35);
            		row.addView(wins);
            		
            		TextView losses = new TextView(this);
            		losses.setText(c.getString(3));
            		losses.setTextColor(Color.LTGRAY);
            		losses.setWidth(35);
            		row.addView(losses);
            		
            		
            		TextView pct = new TextView(this);
            		double wins_num = (double) Integer.parseInt(c.getString(2));
            		double losses_num = (double) Integer.parseInt(c.getString(3));
            		double percent = wins_num / (wins_num + losses_num);
            		NumberFormat nf = NumberFormat.getInstance( );
            		nf.setMaximumFractionDigits(3);
            		pct.setText(nf.format(percent));
            		pct.setTextColor(Color.LTGRAY);
            		pct.setWidth(45);
            		row.addView(pct);
            	
            		/* put the row in the layout */
            		container.addView(row);
            } while (c.moveToNext( ));
        }
        db.close( );
	}
}


