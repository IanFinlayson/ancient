package android.CSLite;

import java.util.StringTokenizer;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.Contacts.People;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.ImageView;

/**
 * Home screen where user can start/join game from
 */
public class Home extends Activity {
	// Request Codes
	public static final int OPTIONS_ACTIVITY = 0;
	public static final int GAME_ACTIVITY = 1;
	// Constants
	public static final int GRENADE_ID  = 0;
	public static final int LAUNCHER_ID = 1;
	public static final int MORTAR_ID   = 2;
	public static final int MINE_ID     = 3;
	public static final int ARMOR_ID    = 4;
	public static final int CLOKE_ID    = 5;
	public static final int RADAR_ID    = 6;

	public static final int GRENADE_PROB  = 0;  //30%
	public static final int LAUNCHER_PROB = 30; //25%
	public static final int MORTAR_PROB   = 55; //10%
	public static final int MINE_PROB     = 65; // 5%
	public static final int ARMOR_PROB    = 70; //10%
	public static final int CLOKE_PROB    = 80; //10%
	public static final int RADAR_PROB    = 90; //10%


	public static final double		EARTH_RADIUS_KILOMETERS = 6378.1370;
	public static final double		ACCELERATION_GRAVITY = 9.80665;
	// velocities in meters/second
	public static final double		GRENADE_VELOCITY = 19.8;
	public static final double		GRENADE_LAUNCHER_VELOCITY = 49.5;
	public static final double		MORTAR_VELOCITY = 140.0;
	// blast radii in meters
	public static final double		GRENADE_BLAST_RADIUS = 5;
	public static final double		GRENADE_LAUNCHER_BLAST_RADIUS = 10;
	public static final double		MORTAR_BLAST_RADIUS = 35;
	// max damages per weapon
	public static final double		GRENADE_MAX_DAMAGE = 20;
	public static final double		GRENADE_LAUNCHER_MAX_DAMAGE = 40;
	public static final double		MORTAR_MAX_DAMAGE = 60;
	public static final int 		MINE_DAMAGE = 30;

	public static final int AMMO_MIN  = 5;
	public static final int AMMO_MAX  = 10;

	private Home me;
	private OptData optData;

	private ImageView options2, join2;


	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.main2);
		me = this;
		optData = new OptData();
		options2 = (ImageView) findViewById(R.id.hostImageView);

		join2 = (ImageView) findViewById(R.id.joinImageView);

		options2.setOnClickListener(new OnClickListener(){
			public void onClick(View v) {
				Intent i = new Intent(me, Options.class);
				me.startActivity(i);
			}
		});
		join2.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// Start playing
				final Intent i = new Intent(me, Game.class);
				new AlertDialog.Builder(Home.this)
				.setTitle("Choose your side!")
				.setSingleChoiceItems(managedQuery(People.CONTENT_URI, null, null, null, "name"), -1, "name", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						Cursor c = managedQuery(People.CONTENT_URI, null, null, null, "name");
						if (c.moveToPosition(which)) {
							if (i.hasExtra("host"))
								i.removeExtra("host");
							StringTokenizer stok = new StringTokenizer(c.getString(c.getColumnIndex(People.NUMBER)), "-");
							String formattedPhoneNumber = "";
							while (stok.hasMoreTokens())
								formattedPhoneNumber += stok.nextToken();
							i.putExtra("host", formattedPhoneNumber);
							System.out.println("host: '"+c.getString(c.getColumnIndex(People.NUMBER))+"' => '"+
									formattedPhoneNumber+"'");
						}
					}
				})
				.setPositiveButton("Counter Terrorist", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						i.setType("CT");
						System.out.println("Start CT");
						me.startActivityForResult(i, GAME_ACTIVITY);				
					}
				})
				.setNegativeButton("Terrorist", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						i.setType("T");
						System.out.println("Start T");
						me.startActivityForResult(i, GAME_ACTIVITY);				
					}
				})
				.show();
			}
		});
	}
	/* (non-Javadoc)
	 * @see android.app.Activity#onActivityResult(int, int, android.content.Intent)
	 */
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == OPTIONS_ACTIVITY) {
			if (data.getType().equals("OPT_RET") && resultCode == Activity.RESULT_OK) {
				// Options returned
				if (data.hasExtra("OptData")) {
					System.out.println("Options returned" + data.getExtras().getString("OptData"));
					optData.fromString(data.getExtras().getString("OptData"));
				} else
					System.out.println("No extra");
			}
		} else if (requestCode == GAME_ACTIVITY) {
			// Returned from game
		}
	}
}