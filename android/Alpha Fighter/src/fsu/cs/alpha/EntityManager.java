/* EntityManager.java
 * This is in abstract class for handling entities.  It is in charge
 * of storing, updating, drawing them and processing collisions
 */

package fsu.cs.alpha;

import java.util.ArrayList;

import android.content.res.Resources;
import android.graphics.Canvas;


/**
 * Manages a list of entites and updates them etc.
 *
 */
public abstract class EntityManager {

	/* handle to the game - in order to pause/end on conditions */
	Game game;
	
	/* handle to the resources for loading images and such */
	Resources resources;
	
	/* actual list of entities */
	ArrayList<Entity> entities;
	
	/**
	 * make an entity manager
	 * @param g the game we manage
	 * @param res the resource thingy so we can get pictures
	 */
	EntityManager(Game g, Resources res) {
		/* create our list of entities */
		entities = new ArrayList<Entity>( );
		
		/* store game */
		game = g;
		
		/* store resources */
		resources = res;
	}
	
	
	/**
	 * add an entity to the list
	 * @param entity the entity to add
	 */
	public void addEntity(Entity entity) {
		entities.add(entity);
	}
	
	/**
	 * stop the game
	 * @param msg win/loss message to give to user
	 */
	public void end(String msg) {
		game.end(msg);
	}
	

	/**
	 * update each entity in the list
	 * @param elapsed_seconds the number of elapsed seconds since last frame
	 */
	public void updateAll(float elapsed_seconds) {
		/* check for collisions */
		handleCollisions( );
		
		/* update each entity */
		for(int i = 0; i < entities.size( ); i++) {
			Entity curr = entities.get(i);
			
			/* if dead remove, else update */
			if(curr.isDead( )) {
				entities.remove(i);
			} else
				curr.update(elapsed_seconds);
		}
	}
	
	 
	/**
	 * return whether two entities have collided - we use bounding
	 * box collision detection.  error refers to the amount bounding
	 * boxes are allowed to overlap without triggerring a collision
	 * @param a first entity to test
	 * @param b second entity to test
	 * @param error the amount of error to allow for
	 * @return whether there was a collision
	 */
	public static boolean collides(Entity a, Entity b, float error) {
		float left_a = a.getX( ) + error;
		float right_a = a.getX( ) + a.getW( ) - error;
		float top_a = a.getY( ) + error;
		float bottom_a = a.getY( ) + a.getH( ) - error;
		
		float left_b = b.getX( ) + error;
		float right_b = b.getX( ) + b.getW( ) - error;
		float top_b = b.getY( ) + error;
		float bottom_b = b.getY( ) + b.getH( ) - error;
		
		/* if a is wholly above b, no collision */
		if(bottom_a <= top_b)
			return false;
		
		/* if a is wholly below b, no collision */
		if(top_a >= bottom_b)
			return false;
		
		/* if a is wholly to the left of b, no collision*/
		if(right_a <= left_b)
			return false;
		
		/* if a is wholly to the right of b, no collision */
		if(left_a >= right_b)
			return false;

		/* otherwise we must have a collision! */	
		return true;
	}
	
	 
	/**
	 * check all entity collisions and dispatch collision messages
	 */
	public void handleCollisions( ) {
		/* use n^2 scheme of checking each pair.  We could base checks on locality,
		 * but we shouldn't have many entities on the screen at once anyways
		 */
		
		/* for each entity */
		for(int i = 0; i < entities.size( ); i++) {
			Entity a_entity = entities.get(i);
		
			/* for each entity */
			for(int j = 0; j < entities.size( ); j++) {
				/* don't collide with ourselves */
				if(i == j) continue;
				
				Entity b_entity = entities.get(j);
				
				/* check for collision between a and b and send the
				 * message to a (b will get message on *his* turn
				 */
				if(collides(a_entity, b_entity, 3.0f))
					a_entity.onCollide(b_entity);
			}
		}
	}
	

	/**
	 * draw all entities to the given canvas
	 * @param canvas canvas to draw them to
	 */
	public void drawAll(Canvas canvas) {
		/* draw each entity */
		for(int i = 0; i < entities.size( ); i++) {
			entities.get(i).draw(canvas);
		}
	}
}




