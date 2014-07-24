package android.CSLite;

import java.util.StringTokenizer;

import android.location.Location;

/**
 * THis class stores info on a player in the game
 *
 */
public class Player {
	public int numGrenades, numLaunchers, numMortars, numMines, numClokes, numRadars, kills, killed;
	public boolean hasArmor, active;
	public String team, name, phoneNumber;

	public Location location;

	/**
	 * Make a player
	 * @param _team which team they're on: either 'T' or 'CT'
	 */
	public Player(String _team) {
		numGrenades = 30;
		numLaunchers = numMortars = numMines = numClokes = numRadars = 30;
		hasArmor = false;
		team = _team;
		phoneNumber = "";
	}
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String str = phoneNumber + " " + name + " " + team;
		str += " " + new Integer(kills).toString();
		str += " " + new Integer(killed).toString();
		str += " " + new Integer(numGrenades).toString();
		str += " " + new Integer(numLaunchers).toString();
		str += " " + new Integer(numMortars).toString();
		str += " " + new Integer(numMines).toString();
		str += " " + new Integer(numClokes).toString();
		str += " " + new Integer(numRadars).toString();
		str += " " + (hasArmor?"1":"0");
		return str;
	}
	/**
	 * Decodes a player from a string
	 * @param str the string (same format as toString returns)
	 */
	public void fromString(String str) {
		StringTokenizer stok = new StringTokenizer(str, " ");
		phoneNumber = stok.nextToken();
		name = stok.nextToken();
		team = stok.nextToken();
		kills = new Integer(stok.nextToken()).intValue();
		killed = new Integer(stok.nextToken()).intValue();
		numGrenades = new Integer(stok.nextToken()).intValue();
		numLaunchers = new Integer(stok.nextToken()).intValue();
		numMortars = new Integer(stok.nextToken()).intValue();
		numMines = new Integer(stok.nextToken()).intValue();
		numClokes = new Integer(stok.nextToken()).intValue();
		numRadars = new Integer(stok.nextToken()).intValue();
		hasArmor = (new Integer(stok.nextToken()).intValue()==1)?true:false;
	}
	/**
	 * Decodes a player from a list of tokes
	 * @param stok the tokesn
	 */
	public void fromTokens(StringTokenizer stok) {
		phoneNumber = stok.nextToken();
		name = stok.nextToken();
		team = stok.nextToken();
		kills = new Integer(stok.nextToken()).intValue();
		killed = new Integer(stok.nextToken()).intValue();
		numGrenades = new Integer(stok.nextToken()).intValue();
		numLaunchers = new Integer(stok.nextToken()).intValue();
		numMortars = new Integer(stok.nextToken()).intValue();
		numMines = new Integer(stok.nextToken()).intValue();
		numClokes = new Integer(stok.nextToken()).intValue();
		numRadars = new Integer(stok.nextToken()).intValue();
		hasArmor = (new Integer(stok.nextToken()).intValue()==1)?true:false;
	}
}
