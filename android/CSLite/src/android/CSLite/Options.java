package android.CSLite;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;

/**
 * The options activity where a user can set game settings
 *
 */
public class Options extends Activity {
	CheckBox grenades, launchers, mortars, mines, armor, cloke, radar;
	Spinner drop_rate;
	Button ok, reset, cancel;

	ArrayAdapter<CharSequence> dropRateOpts;
	int drop_rate_val;
	OptData inOptData;

	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.options2);

		final Options me = this;

		grenades = (CheckBox) findViewById(R.id.opt_grenades);
		launchers = (CheckBox) findViewById(R.id.opt_launchers);
		mortars = (CheckBox) findViewById(R.id.opt_mortars);
		mines = (CheckBox) findViewById(R.id.opt_mines);
		armor = (CheckBox) findViewById(R.id.opt_armor);
		cloke = (CheckBox) findViewById(R.id.opt_cloking);
		radar = (CheckBox) findViewById(R.id.opt_radar);
		drop_rate = (Spinner) findViewById(R.id.opt_drop_rate);
		ok = (Button) findViewById(R.id.opt_set);
		reset = (Button) findViewById(R.id.opt_reset);
		cancel = (Button) findViewById(R.id.opt_cancel);
		pupulateDropRate();

		// Get current OptData
		Intent i = this.getIntent();
		if (i.hasExtra("OptData")) {
			inOptData = new OptData(i.getExtras().getString("OptData"));
		} else
			inOptData = new OptData();
		init();

		drop_rate.setOnItemSelectedListener(new OnItemSelectedListener() {
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {

				int index = drop_rate.getSelectedItemPosition();
				switch(index) {
				case 0: drop_rate_val = 1 * 60; break;
				case 1: drop_rate_val = 2 * 60; break;
				case 2: drop_rate_val = 5 * 60; break;
				case 3: drop_rate_val = 10 * 60; break;
				case 4: drop_rate_val = -1;				
				}
			}


			public void onNothingSelected(AdapterView<?> arg0) {
			}
		});
		ok.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				final Intent i = new Intent(me, Game.class);
				i.putExtra("OptData", new OptData(grenades.isChecked(), launchers.isChecked(), mortars.isChecked(), mines.isChecked(), armor.isChecked(), cloke.isChecked(), radar.isChecked(), drop_rate_val).toString());

				new AlertDialog.Builder(Options.this)
				.setTitle("Choose your side!")
				.setPositiveButton("Counter Terrorist", new DialogInterface.OnClickListener() {

					public void onClick(DialogInterface dialog, int which) {

						i.setType("CT");

						me.startActivity(i);
						finish();
					}
				})
				.setNegativeButton("Terrorist", new DialogInterface.OnClickListener() {

					public void onClick(DialogInterface dialog, int which) {
						i.setType("T");
						//me.startActivityForResult(i, GAME_ACTIVITY);	
						me.startActivity(i);
						finish();
					}
				})
				.show();

			}
		});
		cancel.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				me.finish();
			}
		});
		reset.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				init();
			}
		});
	}

	/**
	 * Initialize the Options screen to default values
	 */
	private void init() {
		grenades.setChecked(inOptData.grenades);
		launchers.setChecked(inOptData.launchers);
		mortars.setChecked(inOptData.mortars);
		mines.setChecked(inOptData.mines);
		armor.setChecked(inOptData.armor);
		cloke.setChecked(inOptData.cloke);
		radar.setChecked(inOptData.radar);
		drop_rate.setSelection((inOptData.drop_rate-10)/5);
		drop_rate_val = inOptData.drop_rate;    	
	}

	/**
	 * Populate the drop box rates (times between drops) in the UI
	 */
	private void pupulateDropRate() {
		dropRateOpts = new ArrayAdapter<CharSequence>(this, android.R.layout.simple_spinner_item);
		dropRateOpts.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		dropRateOpts.add("1 Min.");
		dropRateOpts.add("2 Min.");
		dropRateOpts.add("5 Min.");
		dropRateOpts.add("10 Min.");
		dropRateOpts.add("Never.");
		drop_rate.setAdapter(dropRateOpts);
	}
}