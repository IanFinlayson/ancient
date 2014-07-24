/* Asteroid.java
 * This class extends Entity.  It represents an asteroid object
 */

package fsu.cs.alpha;

import android.graphics.Canvas;

/**
 * totally unused unfortunately :(
 */
public class Asteroid extends Entity {

	/**
	 * make a new asteroi
	 * @param em entity manager we belong to
	 */
	Asteroid(EntityManager em) {
		super(ASTEROID_TAG, em);
		
		/* TODO
		 * setup asteroid initial parameters etc
		 */
		
	}

	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#draw(android.graphics.Canvas)
	 */
	public void draw(Canvas canvas) {
		/* TODO
		 * draw the asteroid to the canvas
		 */

	}

	/* (non-Javadoc)
	 * @see fsu.cs.alpha.Entity#onCollide(fsu.cs.alpha.Entity)
	 */
	public void onCollide(Entity other) {
		/* TODO
		 * handle asteroid collisions
		 * break into smaller chunks etc.
		 */

	}
}
