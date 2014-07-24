package android.CSLite;

import java.util.ArrayList;
import java.util.Random;

/**
 * Class for stroing the game option 
 * which is mostly the generation of dropboxes
 */
public class OptData {
	public boolean grenades, launchers, mortars, mines, armor, cloke, radar;
	public Integer drop_rate;
	public ArrayList<Integer> itemMap;
	public ArrayList<Integer> itemProb;

	/**
	 * Constructs a default OptData
	 */
	public OptData() {
		itemMap = new ArrayList<Integer>();
		itemProb = new ArrayList<Integer>();
		setOptData(true, true, true, true, true, true, true, 10);
	}

	/**
	 * Construct an OptData with the given characteristics for dropbox generation
	 * @param _grenades include grenades?
	 * @param _launchers include grenade launchers?
	 * @param _mortars include mortars?
	 * @param _mines include mines?
	 * @param _armor include armors?
	 * @param _cloke include cloaks?
	 * @param _radar include radars?
	 * @param _drop_rate rate at which dropboxes will occur in seconds
	 */
	public OptData(boolean _grenades, boolean _launchers, boolean _mortars, boolean _mines, boolean _armor, boolean _cloke, boolean _radar, int _drop_rate) {
		itemMap = new ArrayList<Integer>();
		itemProb = new ArrayList<Integer>();
		setOptData(_grenades, _launchers, _mortars, _mines, _armor, _cloke, _radar, _drop_rate);
	}

	/**
	 * Construct a dropbox from the given string argument
	 * @param str the description to take the dropbox from (same as what toString will give you)
	 */
	public OptData(String str) {
		itemMap = new ArrayList<Integer>();
		itemProb = new ArrayList<Integer>();
		fromString(str);
	}

	/**
	 * Generate a random dropbox given the characteristics
	 * @return the dropbox we made
	 */
	public DropBox genDropBox() {
		Random rnd = new Random(android.os.SystemClock.elapsedRealtime());
		int _id = getRndItem(rnd);
		int _amount;
		switch (_id) {
		case Home.GRENADE_ID:
		case Home.LAUNCHER_ID:
		case Home.MORTAR_ID:
		case Home.MINE_ID:
			_amount = Home.AMMO_MIN + rnd.nextInt(Home.AMMO_MAX+1-Home.AMMO_MIN);
			break;
		default:
			_amount = 1;
		}
		DropBox box = new DropBox();
		box.item_id = _id;
		box.amount = _amount;
		return box;
	}
	/**
	 * Set the dropbox generation characterisitics
	 * The parameters are the same as the constructor
	 * @param _grenades
	 * @param _launchers
	 * @param _mortars
	 * @param _mines
	 * @param _armor
	 * @param _cloke
	 * @param _radar
	 * @param _drop_rate
	 */
	public void setOptData(boolean _grenades, boolean _launchers, boolean _mortars, boolean _mines, boolean _armor, boolean _cloke, boolean _radar, int _drop_rate) {
		grenades = _grenades;
		launchers = _launchers;
		mortars = _mortars;
		mines = _mines;
		armor = _armor;
		cloke = _cloke;
		radar = _radar;
		drop_rate = _drop_rate;
		parse();
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String str = new String("");
		str = str + (grenades?"1":"0");		// byte 0
		str = str + (launchers?"1":"0");
		str = str + (mortars?"1":"0");
		str = str + (mines?"1":"0");
		str = str + (armor?"1":"0");
		str = str + (cloke?"1":"0");
		str = str + (radar?"1":"0");		// byte 6
		str = str + drop_rate.toString();
		return str;
	}

	/**
	 * Get characterisitcs out of a string
	 * @param str the string to take characteristics from
	 */
	public void fromString(String str) {
		char buffer[] = new char[7];
		str.getChars(0, 7, buffer, 0);
		grenades  = (buffer[0] == '1') ? true : false;
		launchers = (buffer[1] == '1') ? true : false;
		mortars   = (buffer[2] == '1') ? true : false;
		mines     = (buffer[3] == '1') ? true : false;
		armor     = (buffer[4] == '1') ? true : false;
		cloke     = (buffer[5] == '1') ? true : false;
		radar     = (buffer[6] == '1') ? true : false;
		buffer = new char[str.length()-7];
		str.getChars(7, str.length(), buffer, 0);
		drop_rate = new Integer(new String(buffer));
		parse();
	}
	/**
	 * Put the data from params into maps that we can use
	 */
	private void parse() {
		itemMap.clear();
		itemProb.clear();
		if (grenades) {
			itemMap.add(new Integer(Home.GRENADE_ID));
			itemProb.add(new Integer(Home.GRENADE_PROB));
		}
		if (launchers) {
			itemMap.add(new Integer(Home.LAUNCHER_ID));
			itemProb.add(new Integer(Home.LAUNCHER_PROB));
		}
		if (mortars) {
			itemMap.add(new Integer(Home.MORTAR_ID));
			itemProb.add(new Integer(Home.MORTAR_PROB));
		}
		if (mines) {
			itemMap.add(new Integer(Home.MINE_ID));
			itemProb.add(new Integer(Home.MINE_PROB));
		}
		if (armor) {
			itemMap.add(new Integer(Home.ARMOR_ID));
			itemProb.add(new Integer(Home.ARMOR_PROB));
		}
		if (cloke) {
			itemMap.add(new Integer(Home.CLOKE_ID));
			itemProb.add(new Integer(Home.CLOKE_PROB));
		}
		if (radar) {
			itemMap.add(new Integer(Home.RADAR_ID));
			itemProb.add(new Integer(Home.RADAR_PROB));
		}
	}

	/**
	 * Get a random item
	 * @param rnd Ransom number generator
	 * @return the id of the random item type
	 */
	private int getRndItem(Random rnd) {
		int prob = rnd.nextInt(100);
		int _id = 0;
		System.out.println("rndVal = " + prob);
		for (int i = itemMap.size()-1; i >= 0; --i) {
			if (prob > itemProb.get(i))
				_id = itemMap.get(i).intValue();
		}
		System.out.println("_id = " + _id);
		return _id;
	}
}