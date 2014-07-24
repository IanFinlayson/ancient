package android.CSLite;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.location.Location;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceHolder;
import android.view.SurfaceView;



/**
 * This class provides a few functions for computing GPS stuff
 */
class LocationStuff {

	public static final double		EARTH_RADIUS_KILOMETERS = 6378.1370;

	/**
	 * Compute a point which is along a vector a certain distance away
	 * @param loc Staring GPS position
	 * @param bearing the angle we are facing
	 * @param dis the distance (probably in km) we want to travel
	 * @param earth_radius the radius of the earth in kilometers
	 * @return the Location we end up at
	 */
	public static Location point(Location loc, double bearing, double dis, double earth_radius)
	{
		Location ret = new Location("na"); // check location constructor
		double dOverR = dis / earth_radius;
		bearing = Math.toRadians(bearing);
		double lat1 = Math.toRadians(loc.getLatitude( ));
		double lon1 = Math.toRadians(loc.getLongitude( ));
		double lat2 =
			(Math.asin(Math.sin(lat1) * Math.cos(dOverR) + Math.cos(lat1) * Math.sin(dOverR)
					* Math.cos(bearing)));
		double lon2 =
			(lon1 + Math.atan2((Math.sin(bearing) * Math.sin(dOverR) * Math.cos(lat1)),
					(Math.cos(dOverR) - Math.sin(lat1)
							* Math.sin(Math.toRadians(lat2)))));
		lon2 = (lon2 + Math.PI) % (2 * Math.PI) - Math.PI;
		ret.setLatitude(Math.toDegrees(lat2));
		ret.setLongitude(Math.toDegrees(lon2));
		return ret;
	}

	/**
	 * Calculates the distance between to GPS points
	 * @param position1 the first location
	 * @param position2 the second location
	 * @return the distance (probably in kilometers)
	 */
	public static double calculate_distance(Location position1, Location position2){
		double lat1 = position1.getLatitude();
		lat1 = Math.toRadians(lat1);
		double lat2 = position2.getLatitude();
		lat2 = Math.toRadians(lat2);
		double long1 = position1.getLongitude();
		long1 = Math.toRadians(long1);
		double long2 = position2.getLongitude();
		long2 = Math.toRadians(long2);

		double lat_diff = (lat2-lat1);
		double long_diff = (long2-long1);
		double temp1 = Math.sin(lat_diff/2) * Math.sin(lat_diff/2) +
		Math.cos(lat1) * Math.cos(lat2) * 
		Math.sin(long_diff/2) * Math.sin(long_diff/2); 
		double temp2 = 2 * Math.atan2(Math.sqrt(temp1), Math.sqrt(1-temp1)); 
		double distance = EARTH_RADIUS_KILOMETERS * temp2;

		return distance;
	}


	/**
	 * Scales kilometers to pixels for the display
	 * @param km distance in kilometers
	 * @return distance in pixels
	 */
	public static double scale_km_to_pixels(double km) {
		double meters = km * 1000;
		return meters * RadarThread.PIXELS_PER_METER;
	}

}


/**
 * A thread the continuously draws the radar display
 *
 */
class RadarThread extends Thread { 

	public static int SIZE = 320;

	/* used to trigger main thread if needed */
	Handler handler;

	public static Object locker = new Integer(42);

	/* handle to our surface */
	SurfaceHolder surface_holder;

	/* are we running */
	boolean running;

	/* Our location */
	public static Location player_location = new Location("na");

	/* pictures we use */
	Bitmap you, box, enemy, explosion;

	/* number of meters the diameter is in real life */
	public static int PIXELS_PER_METER;

	/**
	 * Creates a radarthread
	 * @param ctxt The apps base context
	 * @param sh a surface handler
	 * @param pix_per_meter THe amount of pixels per meter (controls scale of battle field)
	 * @param h A handler to send messages to main thread (currently unused)
	 */
	public RadarThread(Context ctxt, SurfaceHolder sh, int pix_per_meter, Handler h) {
		handler = h;
		surface_holder = sh;	
		Resources res = ctxt.getResources( );

		you = BitmapFactory.decodeResource(res, R.drawable.you);
		box = BitmapFactory.decodeResource(res, R.drawable.box);
		enemy = BitmapFactory.decodeResource(res, R.drawable.enemy);
		explosion = BitmapFactory.decodeResource(res, R.drawable.explosion);

		running = true;
		PIXELS_PER_METER = pix_per_meter;
		fire_time = 0;
	}

	/**
	 * quit the thread
	 */
	public void quit( ) {
		running = false;
	}


	static long fire_time;
	static Location fire_loc;
	/**
	 * Create a fire icon where we shot at
	 * @param l Location shot landed at
	 */
	public static void fire(Location l) {

		System.out.println("Distance is = " + LocationStuff.calculate_distance(l, player_location));
		fire_time = System.currentTimeMillis( );
		fire_loc = new Location(l);
	}




	/**
	 * Draw an entity (enemy, dropbox or explosion)
	 * @param canvas the drawing canvas to paint on
	 * @param bm the bitmap to use
	 * @param entity the entities location
	 * @param player the player location
	 */
	private void draw_entity_at(Canvas canvas, Bitmap bm, Location entity, Location player) {

		if(entity == null)
			return;
		/* dist between in km */
		double dist_km = LocationStuff.calculate_distance(player, entity);

		/* dist between in gps */
		double ydist = (player.getLatitude() - entity.getLatitude());
		double xdist = (player.getLongitude() - entity.getLongitude());

		/* angle between player and entity */
		double theta_radians;
		if(xdist == 0)
			theta_radians = (ydist >= 0) ? Math.PI : (3*Math.PI / 2);
		else
			theta_radians = Math.atan(ydist / xdist);

		/* km dist in X and Y */
		double xdist_km = dist_km * Math.cos(theta_radians);
		double ydist_km = dist_km * Math.sin(theta_radians);

		/* pixel dist in X and Y */
		float xdist_pix = (float) LocationStuff.scale_km_to_pixels(xdist_km);
		float ydist_pix = (float) LocationStuff.scale_km_to_pixels(ydist_km);
		float total_pix = (float) Math.sqrt(xdist_pix*xdist_pix + ydist_pix*ydist_pix);


		/* draw the entity at the location */
		if(total_pix < (RadarThread.SIZE / 2)) {
			canvas.drawBitmap(bm, (160 + xdist_pix) - 8, (160 + ydist_pix) - 8, null);
		}		
	}


	/**
	 * Draw the Radarview to canvas
	 * @param canvas the canvas to draw to
	 */
	private void draw(Canvas canvas) {
		/* background */
		canvas.drawColor(Color.WHITE);

		Paint ltgray = new Paint( );
		ltgray.setColor(Color.LTGRAY);
		ltgray.setStyle(Style.FILL_AND_STROKE);

		Paint drkgray = new Paint( );
		drkgray.setColor(Color.DKGRAY);
		drkgray.setStyle(Style.STROKE);

		/* Main circle */
		canvas.drawCircle(160, 160, 160, ltgray);

		/* rings */
		canvas.drawCircle(160, 160, 54, drkgray);
		canvas.drawCircle(160, 160, 105, drkgray);
		canvas.drawCircle(160, 160, 160, drkgray);

		/* you in the center */
		canvas.drawBitmap(you, 152, 152, null);

		/* rotate by current angle */
		canvas.save( );

		/* draw the explosion if neccessary */
		if((System.currentTimeMillis( ) - RadarThread.fire_time) < 3000) {
			draw_entity_at(canvas, this.explosion, RadarThread.fire_loc, player_location);
		}
		canvas.rotate(-Game.angle, 160, 160);

		/* draw the players */
		for(Player p : Game.players) {
			draw_entity_at(canvas, enemy, p.location, player_location);
		}

		synchronized(locker) {
			/* draw the dropboxes */
			for(DropBox d : Game.dropBoxes) {
				draw_entity_at(canvas, box, d, player_location);
			}
		}


		canvas.restore( );
	}


	/* (non-Javadoc)
	 * @see java.lang.Thread#run()
	 */
	public void run( ) {
		while (running) {
			Canvas canvas = null;
			try {
				canvas = surface_holder.lockCanvas(null);
				draw(canvas);
			} finally {
				if (canvas != null) {
					surface_holder.unlockCanvasAndPost(canvas);
				}
			}
		}
	}
}




/**
 * A View that displays a radar screen to a surface
 */
public class RadarView extends SurfaceView implements Callback {

	RadarThread thread;

	/**
	 * Constructs a Radar VIew
	 * @param diameter_meters The number of pixles in one meter
	 * @param ctxt THe apps base context
	 */
	public RadarView(int diameter_meters, Context ctxt) {
		super(ctxt);

		/* get surface info */
		SurfaceHolder holder = getHolder( );
		holder.addCallback(this);

		/* create thread */
		thread = new RadarThread(ctxt, holder, diameter_meters, new Handler( ) {
			public void handleMessage(Message m) {
				/* runs in main thread for UI stuff */
			}
		});		
	}

	/* (non-Javadoc)
	 * @see android.view.SurfaceHolder.Callback#surfaceChanged(android.view.SurfaceHolder, int, int, int)
	 */
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		/* we don't care */		
	}


	/* (non-Javadoc)
	 * @see android.view.SurfaceHolder.Callback#surfaceCreated(android.view.SurfaceHolder)
	 */
	public void surfaceCreated(SurfaceHolder holder) {
		/* start the thread */
		thread.start( );
	}


	/* (non-Javadoc)
	 * @see android.view.SurfaceHolder.Callback#surfaceDestroyed(android.view.SurfaceHolder)
	 */
	public void surfaceDestroyed(SurfaceHolder holder) {
		/* stop the thread */
		try {
			thread.quit( );
			thread.join( );
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
