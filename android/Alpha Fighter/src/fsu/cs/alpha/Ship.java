/* Ship.java
 * This class is a subclass of Entity.  It represents a ship
 * at the bottom or top of the screen.
 */

package fsu.cs.alpha;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.media.MediaPlayer;
import android.view.KeyEvent;

/**
 * a player or enemy ship entity
 *
 */
public class Ship extends Entity {
	
	public static int WIDTH = 48;
	public static int HEIGHT = 32;
	public static int SPEED = 100;				/* pixels per second */
	public static float SPEED_SCALE = 3;		/* multiplied by orientation */
	public static int FIRE_DELAY = 1000;		/* ms */
	
	Bitmap ship_image = null;
	boolean human_controlled;
	
	/* used for the explosion animation */
	boolean blown_up;
	static final int BLOW_DELAY = 125;		/* ms between each explosion frame */
	long blown_up_time;
	int blown_up_stage;
	static Bitmap blown1 = null;
	static Bitmap blown2 = null;
	static Bitmap blown3 = null;
	static Bitmap blown4 = null;
	
	public static MediaPlayer mp = null;
	
	long last_fire;
	String message = null;
	
	/**
	 * create a new ship
	 * @param players_ship whether this will be controlled by player
	 * @param em the entity manager who will be managing us
	 */
	public Ship(boolean players_ship, EntityManager em) {
		super(players_ship ? PLAYER_SHIP_TAG : ENEMY_SHIP_TAG, em);
		
		if((mp == null)&&GameActivity.play_sound) {
			mp = MediaPlayer.create(this.entity_manager.game.context, R.raw.shot);
		}
		
		/* load image */
		if(players_ship)
			ship_image = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.playership);
		else
			ship_image = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.enemyship);
		
		if(blown1 == null) blown1 = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.blown1);
		if(blown2 == null) blown2 = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.blown2);
		if(blown3 == null) blown3 = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.blown3);
		if(blown4 == null) blown4 = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.blown4);
		
		/* initial coordinates */
		w = WIDTH;
		h = HEIGHT;
		dx = 0;
		dy = 0;
		
		x = (GameView.WIDTH / 2) - (WIDTH / 2);
		
		if(players_ship)
			y = GameView.HEIGHT - HEIGHT;
		else
			y = 0;
		
		last_fire = 0;
		blown_up = false;
	}

	
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#draw(android.graphics.Canvas)
	 */
	public void draw(Canvas canvas) {
		if(blown_up) {
			switch(blown_up_stage) {
			case 1:canvas.drawBitmap(blown1, x, y, null); break;
			case 2:canvas.drawBitmap(blown2, x, y, null); break;
			case 3:canvas.drawBitmap(blown3, x, y, null); break;
			case 4:canvas.drawBitmap(blown4, x, y, null); break;
			}

		} else {
			canvas.drawBitmap(ship_image, x, y, null);
		}
	}
	
	
	/**
	 * update enemy ship based on player
	 * @param player the human player so we know where to shoot
	 */
	public void updateAI(Ship player) {
		if(blown_up) return;
		int fudge = 50;
		if((x - player.x) > fudge)
			dx = -SPEED;
		else if((x - player.x) < -fudge)
			dx = SPEED;
		
		/* always :) */
		fire( );
	}
	
	/**
	 * update human ship from controls
	 * @return whether we successfully fire a shot or not
	 */
	public boolean updateControls( ) {
		if(blown_up) return false;
		boolean fired = false;
		switch(KeyBuffer.pollKey( )) {
		case KeyEvent.KEYCODE_DPAD_LEFT:
			dx = -SPEED;
			break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			dx = SPEED;
			break;
		case KeyEvent.KEYCODE_DPAD_CENTER:
			fired = fire( );
			break;
		}
		
		/* get speed */
		if(GameActivity.tilt_input) {
			float speed = KeyBuffer.get_ship_speed( );
			if(speed != 123456.0f)
				setDx(SPEED_SCALE * speed);
		}
		
		return fired;
	}
	
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#update(float)
	 */
	public void update(float elapsed_seconds) {
		
		/* if off the border, snap to border and set speed to 0 */
		if(x < 0) {
			x = 0;
			dx = 0;
		} else if((x + w) > GameView.WIDTH) {
			x = GameView.WIDTH - w;
			dx = 0;
		}
		
		/* update blown up stage */
		if(blown_up) {
			if(blown_up_stage == 5) {
				kill( );
				/* end the game */
				entity_manager.end(message);
			}
			long now = System.currentTimeMillis( );
			
			if((now - blown_up_time) > BLOW_DELAY) {
				blown_up_time = now;
				blown_up_stage++;
			}
		}
		
		/* super class */
		super.update(elapsed_seconds);
	}
	
	/**
	 * attempt to fire (could fail based on fire delay etc)
	 * @return whether we were successful
	 */
	public boolean fire( ) {
		/* if we can fire */
		long now = System.currentTimeMillis( ); 
		if((now - last_fire) > FIRE_DELAY) {
			entity_manager.addEntity(new Bullet(x + (w/2), y, (type_tag == PLAYER_SHIP_TAG), entity_manager));
			last_fire = now;
			if(GameActivity.play_sound){
				mp.release();
				mp = MediaPlayer.create(this.entity_manager.game.context, R.raw.shot);
				mp.start();
			}
			
			return true;
		}
		
		return false;
	}
	
	/**
	 * blow up the ship
	 * @param msg message to pass up to the player eg "You have lost"
	 */
	void blowup(String msg) {
		blown_up = true;
		message = msg;
		dx = 0;
		blown_up_time = System.currentTimeMillis( );
		blown_up_stage = 1;
	}
	

	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#onCollide(fsu.cs.alpha.Entity)
	 */
	public void onCollide(Entity other) {
		/* don't die *again* */
		if(blown_up) return;
		
		/* check if we are hit by enemy bullet */
		if(((type_tag == PLAYER_SHIP_TAG) && (other.type_tag == ENEMY_BULLET_TAG))) {
			if(GameActivity.play_sound) {
				mp.release();
				mp = MediaPlayer.create(this.entity_manager.game.context, R.raw.explode2);
				mp.start();
			}
			
			blowup("You Have Lost!!");
		}
		else if((type_tag == ENEMY_SHIP_TAG) && (other.type_tag == PLAYER_BULLET_TAG)) {
			if(GameActivity.play_sound) {
				mp.release();
				mp = MediaPlayer.create(this.entity_manager.game.context, R.raw.explode3);
				mp.start();
			}
			
			blowup("You Have Won!!");
		}
	}
}





