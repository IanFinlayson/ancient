package android.CSLite;

import java.util.ArrayList;
import java.util.Random;
import java.util.StringTokenizer;
import java.util.Timer;
import java.util.TimerTask;

import org.openintents.hardware.SensorManagerSimulator;
import org.openintents.provider.Hardware;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.SensorListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.telephony.gsm.SmsManager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.AbsoluteLayout;
import android.widget.Button;
import android.widget.EditText;




/**
 * Main game activity
 *
 */
public class Game extends Activity implements SensorListener, LocationListener {


	/* (non-Javadoc)
	 * @see android.location.LocationListener#onLocationChanged(android.location.Location)
	 */
	public void onLocationChanged(Location location) {
		if(Game.leave)
			finish( );

		if(location != null){
			location = new Location("na");
			location.setLongitude(-84.30000);
			location.setLatitude(30.441254);
			RadarThread.player_location.set(location);
			// Check for drop boxes
			for (int i = 0; i < Game.dropBoxes.size(); ++i) {
				if (LocationStuff.calculate_distance(location, Game.dropBoxes.get(i))*1000 < 10) {
					// Pickup drop box
					int amt = Game.dropBoxes.get(i).amount;
					Game.sendToAll("DROP_PU "+location.getLongitude()+" "+location.getLatitude());
					switch(Game.dropBoxes.get(i).item_id) {
					case Home.GRENADE_ID:
						player.numGrenades += amt;
						grenade.setText(""+player.numGrenades);
						break;
					case Home.LAUNCHER_ID:
						player.numLaunchers += amt;
						grenade.setText(""+player.numLaunchers);
						break;
					case Home.MORTAR_ID:
						player.numMortars += amt;
						grenade.setText(""+player.numMortars);
						break;
					case Home.MINE_ID:
						player.numMines += amt;
						grenade.setText(""+player.numMines);
						break;
					case Home.CLOKE_ID:
						player.numClokes += amt;
						grenade.setText(""+player.numClokes);
						break;
					case Home.RADAR_ID:
						player.numRadars += amt;
						grenade.setText(""+player.numRadars);
						break;
					case Home.ARMOR_ID:
						player.hasArmor = true;
						break;
					}
				}
			}
			// Check for mines
			for (Mine mine : Game.mines) {
				if (LocationStuff.calculate_distance(location, mine)*1000 < 5) {
					// POWNED
					Game.sendToAll("MINE_DES "+mine.getLongitude()+" "+mine.getLatitude());
					if (player.hasArmor) {
						player.hasArmor = false;
					} else {
						player.killed += Home.MINE_DAMAGE;
						Game.sendToNumber("HIT "+Home.MINE_DAMAGE, mine.owner);
					}
				}					
			}
		}
	}

	/* (non-Javadoc)
	 * @see android.location.LocationListener#onProviderDisabled(java.lang.String)
	 */
	public void onProviderDisabled(String provider) {
	}

	/* (non-Javadoc)
	 * @see android.location.LocationListener#onProviderEnabled(java.lang.String)
	 */
	public void onProviderEnabled(String provider) {
	}

	/* (non-Javadoc)
	 * @see android.location.LocationListener#onStatusChanged(java.lang.String, int, android.os.Bundle)
	 */
	public void onStatusChanged(String provider, int status, Bundle extras) {
	}

	public static final short PORT = 1234;
	public static final int LEAVE_MENU_ID = 0;
	public static final int END_MENU_ID = 1;
	public static final int CHNAME_MENU_ID = 2;
	public static final int RADIO_MENU_ID = 3;

	public static String host = null;
	public static boolean hostInPlayerList;
	public static boolean gameOn;
	public static boolean leave;
	public static ArrayList<Player> players;
	public static ArrayList<Mine> mines;
	public static ArrayList<DropBox> dropBoxes;

	public static Player player;
	public OptData gameOptions = null;
	public Timer clock = null;
	public Location startGameLocation = null;
	public Random rnd = null;

	public static int angle;
	public static int pitch;
	public long last_mine;

	MessageReceiver mr = null;
	SensorManager sensor_manager = null;

	private LocationManager myLocationManager = null;


	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow( ).requestFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.gameview);

		IntentFilter in = new IntentFilter( );
		in.addAction("android.provider.Telephony.SMS_RECEIVED");
		mr = new MessageReceiver( );
		this.registerReceiver(mr, in);



		myLocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		myLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, this);

		// get last known location to initialize values
		Location now = myLocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
		RadarThread.player_location = new Location(now);

		last_mine = 0;
		hostInPlayerList = false;

		Intent i = this.getIntent();

		players = new ArrayList<Player>();
		mines = new ArrayList<Mine>();
		dropBoxes = new ArrayList<DropBox>();
		player = new Player(this.getIntent().getType());
		player.active = true;
		//player.phoneNumber = ((TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE)).getLine1Number();
		//Toast.makeText(this, "Player phone Num: " + player.phoneNumber, Toast.LENGTH_LONG).show();
		gameOn = true;
		leave = false;
		System.out.println("Team: " + player.team);
		selectName();
		if (this.getIntent().hasExtra("host")) {
			host = this.getIntent().getExtras().getString("host");
			joinGame();
		} else {
			System.out.println("I am the host");
			rnd = new Random(android.os.SystemClock.elapsedRealtime());
			startGameLocation = new Location(RadarThread.player_location);
		}
		initView();

		Player enemy1 = new Player("T");
		enemy1.location = new Location("na");
		enemy1.location.setLongitude(-84.30000);
		if(host == null) {
			enemy1.location.setLatitude(30.441354);	
		} else {
			enemy1.location.setLatitude(30.441454);
		}
		Game.players.add(enemy1);



		if (i.hasExtra("OptData") && host == null) {
			System.out.println("I got the OPTIONS!");
			gameOptions = new OptData();
			gameOptions.fromString(i.getExtras().getString("OptData"));
			if(gameOptions.drop_rate != -1) { 
				clock = new Timer( );
				clock.scheduleAtFixedRate(new TimerTask() {
					@Override
					public void run() {
						genBox();
					}				
				}, 0, gameOptions.drop_rate*1000);
			}
		}
	}
	Button cloak;
	Button radar;
	Button grenade;
	Button launcher;
	Button mortar;
	Button mine;


	/**
	 * Fire a weapon
	 * @param weapon_id ID of the weapon as given in Home.java
	 */
	public void fire(int weapon_id) {
		Location here, impact;
		double dist;

		here = new Location("na");
		here.set(RadarThread.player_location);

		if(weapon_id == Home.GRENADE_ID){
			dist = (Math.pow(Home.GRENADE_VELOCITY,2)/Home.ACCELERATION_GRAVITY)*Math.sin(2*pitch);
		}
		else if(weapon_id == Home.LAUNCHER_ID){
			dist = (Math.pow(Home.GRENADE_LAUNCHER_VELOCITY,2)/Home.ACCELERATION_GRAVITY)*Math.sin(2*pitch);
		}
		else if(weapon_id == Home.MORTAR_ID){
			dist = (Math.pow(Home.MORTAR_VELOCITY,2)/Home.ACCELERATION_GRAVITY)*Math.sin(2*pitch);
		}
		else{
			// invalid weapon
			dist = 0;
		}

		impact = LocationStuff.point(here, angle, dist/2000, Home.EARTH_RADIUS_KILOMETERS);

		RadarThread.fire(impact);
		sendToAll("FIRE " + weapon_id + " " + impact.getLongitude() + " " + impact.getLatitude( ));
	}


	/**
	 * Initialize UI components
	 */
	private void initView() {
		AbsoluteLayout ll = (AbsoluteLayout) findViewById(R.id.linlay);

		/* make a radar view */
		RadarView radar_view = new RadarView(4, getBaseContext( ));

		/* add it to the top of the layout with a fixed size */
		ll.addView(radar_view, 0, new LayoutParams(320, 320));

		/* get buttons */
		cloak = (Button) findViewById(R.id.Button05);
		radar = (Button) findViewById(R.id.Button06);
		grenade = (Button) findViewById(R.id.Button01);
		launcher = (Button) findViewById(R.id.Button02);
		mortar = (Button) findViewById(R.id.Button03);
		mine = (Button) findViewById(R.id.Button04);

		cloak.setText("" + player.numClokes);
		radar.setText("" + player.numRadars);
		grenade.setText("" + player.numGrenades);
		launcher.setText("" + player.numLaunchers);
		mortar.setText("" + player.numMortars);
		mine.setText("" + player.numMines);

		/* listeners */
		cloak.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numClokes > 0) {
					/* TODO do cloak */
					player.numClokes--;
					cloak.setText("" + player.numClokes);
				}
			}});

		radar.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numRadars > 0) {
					/* TODO do radar */
					player.numRadars--;
					radar.setText("" + player.numRadars);
				}
			}});

		grenade.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numGrenades > 0) {
					fire(Home.GRENADE_ID);
					player.numGrenades--;
					grenade.setText("" + player.numGrenades);
				}
			}});

		launcher.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numLaunchers > 0) {
					fire(Home.LAUNCHER_ID);
					player.numLaunchers--;
					launcher.setText("" + player.numLaunchers);
				}
			}});

		mortar.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numMortars > 0) {
					fire(Home.MORTAR_ID);
					player.numMortars--;
					mortar.setText("" + player.numMortars);
				}
			}});

		mine.setOnClickListener(new OnClickListener ( ) {
			public void onClick(View v) {
				if(player.numMines > 0) {
					Radio.play(getBaseContext(), Radio.BOMB_PLANTED);
					last_mine = System.currentTimeMillis( );
					player.numMines--;
					mine.setText("" + player.numMines);
					sendToAll("MINE " + RadarThread.player_location.getLongitude() + " " + RadarThread.player_location.getLatitude());
				}
			}});







		/* get openintents manager */
		Hardware.mContentResolver = getContentResolver( );
		sensor_manager = (SensorManager) new SensorManagerSimulator((SensorManager) 
				getSystemService(SENSOR_SERVICE)); 

		//	for Real phone only sensor_manager = (SensorManager) getSystemService(SENSOR_SERVICE);

		/* now connect to simulator */ 
		SensorManagerSimulator.connectSimulator(); 

		/* now enable the new sensors */
		sensor_manager.registerListener(this, 
				SensorManager.SENSOR_ORIENTATION, 
				SensorManager.SENSOR_DELAY_GAME);


	}

	/**
	 * Join a given game
	 */
	public static void joinGame() {
		System.out.println("host #:'" + host + "'");
		sendToServer("JOIN "+player.team+" "+player.name);
	}
	/**
	 * Send a message to the Host phone
	 * @param msg the message to send
	 */
	public static void sendToServer(String msg) {
		if (gameOn == false)
			return;
		if (host != null) {
			SmsManager smsManager = SmsManager.getDefault();
			smsManager.sendTextMessage(host, null, msg, null, null);
			//			smsManager.sendDataMessage(host, null, PORT, msg.getBytes(), null, null);
		}
	}
	/**
	 * Send a message to all phones
	 * @param msg the message to send
	 */
	public static void sendToAll(String msg) {
		if (gameOn == false)
			return;

		System.out.println("Sedning to all of " + players.size( ) + " players!");

		SmsManager smsManager = SmsManager.getDefault();
		for (int i = 0; i < players.size(); ++i) {
			if(players.get(i).active || msg.substring(0, 3).equalsIgnoreCase("END")) {
				System.out.println("Sending " + msg + " to " + players.get(i).phoneNumber);
				smsManager.sendTextMessage(players.get(i).phoneNumber, null, msg, null, null);
			}
		}
		/*		if(host!=null){
			smsManager.sendTextMessage(host, null, msg, null, null);			
		}*/
	}

	/**
	 * Send SMS to given number
	 * @param msg the message to send
	 * @param number the phone number to send to
	 */
	public static void sendToNumber(String msg, String number) {
		if (gameOn == false)
			return;
		SmsManager smsManager = SmsManager.getDefault();
		smsManager.sendTextMessage(number, null, msg, null, null);		
	}
	/**
	 * Dialog to choose your player handle
	 */
	private void selectName() {
		final EditText name = new EditText(this);
		name.setText(player.name);

		new AlertDialog.Builder(Game.this)
		.setTitle("Choose your name!")
		.setView(name)
		.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				player.name = name.getText().toString();
				sendToAll("NEWNAME "+player.name+" "+player.phoneNumber);
			}
		})
		.show();
	}
	/* (non-Javadoc)
	 * @see android.app.Activity#onCreateOptionsMenu(android.view.Menu)
	 */
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		if (host == null)
			menu.add(0, END_MENU_ID, 0, "End Game");
		else
			menu.add(0, LEAVE_MENU_ID, 0, "Leave Game");

		menu.add(0, CHNAME_MENU_ID, 0, "Change Name");
		menu.add(0, RADIO_MENU_ID, 0, "Radio");
		return true;
	}
	/* (non-Javadoc)
	 * @see android.app.Activity#onOptionsItemSelected(android.view.MenuItem)
	 */
	public boolean onOptionsItemSelected(MenuItem item) {
		switch(item.getItemId()) {
		case CHNAME_MENU_ID:
			selectName();
			break;
		case END_MENU_ID:
			Game.player.active = false;
			sendToAll("STAT");
			System.out.println("Requested stats...");
			for (int i = 0; i < players.size(); ++i) {

				System.out.println("players[" + i + "].active = " + (players.get(i).active?"true":"false"));
			}
			break;
		case LEAVE_MENU_ID:
			sendToServer("LEAVE "+player.toString());
			finish();
			break;
		case RADIO_MENU_ID:
			String [] sounds = {"Enemy Spotted", "Fire in the Hole", "Fallback", "Follow Me",
					"Move Out", "Hold Position", "Regroup", "Roger"};


			new AlertDialog.Builder(Game.this)
			.setTitle("Radio to Team")
			.setSingleChoiceItems(sounds, -1, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
				}
			}
			)
			.setPositiveButton("Send", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					/* TODO */
				}
			})
			.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {				
				}
			})
			.show();



			break;
		default:
			return false;
		}
		return true;
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
		assert(sensor == SensorManager.SENSOR_ORIENTATION);

		/* get the angle we are heading in and our pitch (angle phone is pointing) */
		int theta = (int) values[0];
		pitch = (int) values[1];
		angle = theta;


	}

	/* (non-Javadoc)
	 * @see android.app.Activity#onResume()
	 */
	protected void onResume( ) {
		super.onResume();
		sensor_manager.registerListener(this, 
				SensorManager.SENSOR_ORIENTATION,
				SensorManager.SENSOR_DELAY_GAME);
	}







	/* (non-Javadoc)
	 * @see android.app.Activity#onStop()
	 */
	protected void onStop( ) {
		this.unregisterReceiver(mr);
		Game.gameOn = false;
		sensor_manager.unregisterListener(this);
		super.onStop();
	}
	/**
	 * Generate a dropbox at some random location
	 */
	private void genBox() {
		if(gameOn) {
			DropBox box = gameOptions.genDropBox( );
			double rnd1 = rnd.nextDouble( ), rnd2 = rnd.nextDouble( );
			double amt_to_add1 = (rnd1 / 1000.0) - .0005;
			double amt_to_add2 = (rnd2 / 1000.0) - .0005;

			box.setLongitude(startGameLocation.getLongitude() + amt_to_add1);
			box.setLatitude(startGameLocation.getLatitude() + amt_to_add2);
			Game.sendToAll("DROP " + box.toString());
			synchronized(RadarThread.locker) {
				Game.dropBoxes.add(box);
			}
		}
	}
}

/**
 * A mine
 */
class Mine extends Location{
	/**
	 * Make a new mine
	 * @param owner the player who planted the name - probably their phone number
	 */
	public Mine(String owner) {
		super("na");
		this.owner = owner;
	}
	public String owner;
}

/**
 * A Dropbox
 *
 */
class DropBox extends Location {
	/**
	 * Make a new dropbox
	 */
	public DropBox() {
		super("na");
	}
	public int item_id;
	public int amount;
	/* (non-Javadoc)
	 * @see android.location.Location#toString()
	 */
	public String toString() {
		return (item_id + " " + amount + " " + this.getLongitude() + " " + this.getLatitude());
	}
	/**
	 * Setup a dropbox from a string token list
	 * @param stok the tokesn to use (format same as what toString gives)
	 */
	public void fromStok(StringTokenizer stok) {
		item_id = new Integer(stok.nextToken()).intValue();
		amount = new Integer(stok.nextToken()).intValue();
		this.setLongitude(new Double(stok.nextToken()).doubleValue());
		this.setLatitude(new Double(stok.nextToken()).doubleValue());
	}
}
