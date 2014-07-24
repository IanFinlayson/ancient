/* Entity.java
 * This is an abstract class representing any on screen element
 * such as the player and enemy ships, asteroids, bullets, powerups etc.
 * it has location, speed and tag
 */

package fsu.cs.alpha;

import android.graphics.Canvas;

/**
 * This abstract class is the base for anything seen on screen
 */
public abstract class Entity {
	
	/* tags used to tell which entity we are dealing with*/
	public static final int PLAYER_SHIP_TAG = 1;
	public static final int ENEMY_SHIP_TAG = 2;
	public static final int ASTEROID_TAG = 3;
	public static final int PLAYER_BULLET_TAG = 4;
	public static final int ENEMY_BULLET_TAG = 5;
	/* ... more to come ... */
	public static int OTHER_TAG = -1; 
	protected int type_tag;
	
	/* location and velocity */
	protected float x, y, dx, dy;
	
	/* set speed and such */
	public void setX(float x) {this.x = x;}
	public void setY(float y) {this.y = y;}
	public void setDx(float dx) {this.dx = dx;}
	public void setDy(float dy) {this.dy = dy;}
	/* get dimensions */ 
	public float getX( ) {return x;}
	public float getY( ) {return y;}
	public float getW( ) {return w;}
	public float getH( ) {return h;}
	
	/* width, height used for collisions */
	protected float w, h;
	
	/* used to remove entities that have gone
	 * past their prime
	 */
	boolean dead;
	
	/* handle to the entity manager so an entity can add entities to the manager
	 * so an asteroid can add smaller chunks on breakage or a ship can add bullets
	 * missles and so on
	 */
	EntityManager entity_manager;
	
	
	/**
	 * make a new entity
	 * @param tag the tag we are - valid ones appear as constants in this class
	 * @param em entity manager we are managed by
	 */
	Entity(int tag, EntityManager em) {
		type_tag = tag;
		entity_manager = em;
		dead = false;
	}
	
	
	/**
	 * update entity position - should be called by children when overriden!
	 * @param elapsed_seconds the seconds since last frame - for time-based movement
	 */
	public void update(float elapsed_seconds) {
		x += dx * elapsed_seconds;
		y += dy * elapsed_seconds;
	}
	
	
	/**
	 * used for clearing up old entities i.e. dead asteroids, bullets etc.
	 * @return whether we are dead
	 */
	public boolean isDead( ) {
		return dead;
	}
	
	
	/**
	 * remove an entity from existance
	 */
	public void kill( ) {
		dead = true;
	}
	
	
	/**
	 * called when this entity has collided with another - it will be called
	 * for the other entity with us as the parameter as well
	 * @param other the entity we collided with
	 */
	public abstract void onCollide(Entity other);
	
	/**
	 * draw the entity to the canvas
	 * @param canvas where to draw us
	 */
	public abstract void draw(Canvas canvas);
	
}





