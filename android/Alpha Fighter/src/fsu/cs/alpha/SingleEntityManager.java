/* SingleEntityManager.java
 * this class is a subclass of the entity manager
 * it's job is to do the AI and update logic needed
 * for the single player game
 */

package fsu.cs.alpha;

import android.content.res.Resources;

/**
 * Entity manager for the single player game
 *
 */
public class SingleEntityManager extends EntityManager {

	Ship player;
	Ship enemy;
	
	/**
	 * create an entity manager
	 * @param g the game object we are using
	 * @param res the contexts' resource thingy so we can load images
	 */
	SingleEntityManager(Game g, Resources res) {
		super(g, res);
		
		/* add the ships to the list */
		player = new Ship(true, this);
		enemy = new Ship(false, this);
		addEntity(player);
		addEntity(enemy);
	}
	
	/* (non-Javadoc)
	 * @see fsu.cs.alpha.EntityManager#updateAll(float)
	 */
	public void updateAll(float elapsed_seconds) {
		/* update ships */
		if(!enemy.isDead( ))
			enemy.updateAI(player);
		
		if(!player.isDead( ))
			player.updateControls( );
		
		
		/* parent */
		super.updateAll(elapsed_seconds);
	}

}
