package fsu.cs.alpha;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;

/**
 * Bullet entity that is shot from the ships
 */
public class Bullet extends Entity {

	public static int WIDTH = 8;
	public static int HEIGHT = 16;
	public static int SPEED = 175; /* pixels per second */
	
	static Bitmap img_north = null;
	static Bitmap img_south = null;
	
	boolean north_bound;
	
	/**
	 * make a bullet
	 * @param startx starting position X coord
	 * @param starty starting position Y coord
	 * @param north are we headed north (south if not)
	 * @param em entity manager that is holding us
	 */
	Bullet(float startx, float starty, boolean north, EntityManager em) {
		super(north ? PLAYER_BULLET_TAG : ENEMY_BULLET_TAG, em);
		
		if(img_north == null)
			img_north = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.bullet_north);
		
		if(img_south == null)
			img_south = BitmapFactory.decodeResource(entity_manager.resources, R.drawable.bullet_south);
		
		north_bound = north;
		x = startx;
		y = starty;
		w = WIDTH;
		h = HEIGHT;
		dx = 0;
		
		if(north_bound)
			dy = -SPEED;
		else
			dy = SPEED;
		
	}

	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#draw(android.graphics.Canvas)
	 */
	public void draw(Canvas canvas) {
		if(north_bound)
			canvas.drawBitmap(img_north, x, y, null);
		else
			canvas.drawBitmap(img_south, x, y, null);
	}
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#update(float)
	 */
	public void update(float elapsed_seconds) {
		/* if we are off the screen (miss) go away */
		if(((y + h) < 0) || (y > GameView.HEIGHT)) {
			kill( );
		}
		
		super.update(elapsed_seconds);
	}
	

	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#onCollide(fsu.cs.alpha.Entity)
	 */
	public void onCollide(Entity other) {
		/* if we collided with something hard (as opposed to another bullet), go away */
		switch(other.type_tag) {
		case PLAYER_SHIP_TAG:
			if(!north_bound) kill( );
			break;
		case ENEMY_SHIP_TAG:
			if(north_bound) kill( );
			break;
		case PLAYER_BULLET_TAG:
		case ENEMY_BULLET_TAG:
		case ASTEROID_TAG:
			kill( );
			break;
		}
	}
}
