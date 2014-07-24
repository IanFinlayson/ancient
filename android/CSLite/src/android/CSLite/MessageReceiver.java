package android.CSLite;

import java.util.StringTokenizer;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.os.Bundle;
import android.telephony.gsm.SmsMessage;
import android.util.Log;
import android.widget.Toast;

/**
 * The Broadcast recevier that interprets text messages
 * from the game.  This should be changed to use the data
 * text messages instead of normal SMS
 */
public class MessageReceiver extends BroadcastReceiver{

	private static StringTokenizer stok;
	private static String token;
	private static String from;

	/* (non-Javadoc)
	 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
	 */
	public void onReceive(Context context, Intent intent) {

		Bundle bundle = intent.getExtras();


		Object messages[] = (Object[]) bundle.get("pdus");


		SmsMessage smsMessage[] = new SmsMessage[messages.length];
		for (int n = 0; n < messages.length; n++) {
			smsMessage[n] = SmsMessage.createFromPdu((byte[]) messages[n]);
		}


		from= smsMessage[0].getOriginatingAddress(); // 0123456789
		String message=smsMessage[0].getMessageBody();
		Log.w("MYdebug", "Message = " + message + "FROM: " + from + " ");

		stok = new StringTokenizer(message, " ");

		// Parse the message
		token = stok.nextToken();
		System.out.println("Token is: '" + token +"'");
		if(token.equalsIgnoreCase("JOIN")){
			// if host parse Join message
			if(Game.host == null)
				parseJoin(stok);

		} else if (token.equalsIgnoreCase("WHOAMI")) { 
			if (Game.host != null)
				parseWhoAmI(stok);
		} else if (token.equalsIgnoreCase("YOUARE")) { 
			parseYouAre(stok);
		} else if(token.equalsIgnoreCase("DJOIN")){
			// if client join request has been denied
			// if host send message to client denied
			Toast.makeText(context, "Server denied your request, you must still be active...", Toast.LENGTH_SHORT).show();

		} else if(token.equalsIgnoreCase("RJOIN")){
			// if client parse re-join message
			if(Game.host != null)
				parseRejoin(stok);

		} else if(token.equalsIgnoreCase("ACCEPT")){
			// if client parse accept message
			if(Game.host != null)
				parseAccept(stok);

		} else if(token.equalsIgnoreCase("ADD_PL")){
			// if client parse add player message
			if(Game.host != null)
				parseAddPl(stok);

		} else if (token.equalsIgnoreCase("NEWNAME")) {
			// Player changed his name
			parseNewName(stok);
		}else if(token.equalsIgnoreCase("START")){
			// game starts here

		} else if(token.equalsIgnoreCase("FIRE")){
			// parse fire message
			parseFire(stok);

		} else if(token.equalsIgnoreCase("HIT")){
			// parse hit message
			parseHit(stok);

		} else if(token.equalsIgnoreCase("MINE")){
			// parse mine set message
			parseMine(stok);

		} else if(token.equalsIgnoreCase("MINE_DES")){
			// parse mine destroyed message
			parseMineDes(stok);

		} else if(token.equalsIgnoreCase("POS")){
			// if client send back lat and long to host
			if(Game.host != null)
			{
				// send lat and lon
			}

		} else if(token.equalsIgnoreCase("MYPOS")){
			// if host parse my position message
			if(Game.host == null)
				parseMyPos(stok);

		} else if(token.equalsIgnoreCase("DROP")){
			// parse dropbox created message
			parseDrop(stok);

		} else if(token.equalsIgnoreCase("DROP_PU")){
			// parse dropbox picked up message
			parseDropPU(stok);

		} else if(token.equalsIgnoreCase("LEAVE")){
			// if host parse leave message
			if(Game.host == null)
				parseLeave(stok);

		} else if(token.equalsIgnoreCase("PL_LEFT")){
			// if client parse player left message
			if(Game.host != null)
				parseLeft(stok);

		} else if(token.equalsIgnoreCase("STAT")){
			// if client send host current stats
			if(Game.host != null)
			{
				// sent host current stats
				Game.sendToServer("MYSTAT "+new Integer(Game.player.kills).toString()+
						" "+new Integer(Game.player.killed).toString());
			}

		} else if(token.equalsIgnoreCase("MYSTAT")){
			// if host parse my stats message
			if(Game.host == null)
				parseMyStat(stok, context);

		} else if(token.equalsIgnoreCase("END")){
			// parse end message to get rankings
			if(Game.host != null)
				parseEnd(stok, context);

		}

	}

	/**
	 * Parse the whoami message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseWhoAmI(StringTokenizer mystok) {
		Game.sendToServer("YOUARE "+Game.host);
		Game.joinGame();
	}
	/**
	 * Parse YOuare message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseYouAre(StringTokenizer mystok) {
		Game.player.phoneNumber = mystok.nextToken();
		Game.players.add(Game.player);
		for (Player p : Game.players) {
			System.out.println("Player: "+p.name+" "+p.phoneNumber);
		}
		Game.hostInPlayerList = true;		
	}
	/**
	 * Parse JOIN message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseJoin(StringTokenizer mystok)
	{
		if (Game.hostInPlayerList == false) {
			Game.sendToNumber("WHOAMI", from);
			return;
		}
		for (int i = 0; i < Game.players.size(); ++i)
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(from)) {
				if (Game.players.get(i).active) {
					Game.sendToNumber("DJOIN", from);
					System.out.println("Host: deny request from "+from);
					return;
				}
				// Player was already here, initialize to old stats
				Game.sendToNumber("RJOIN "+ Game.players.get(i).toString(), from);
				System.out.println("Host: rejoin player "+Game.players.get(i).phoneNumber);				


				Game.sendToAll("ADD_PL "+Game.players.get(i).team+" "+
						Game.players.get(i).name+" "+Game.players.get(i).phoneNumber);

				Game.players.get(i).active = true;

				System.out.println("Host: sendToAll: add player "+Game.players.get(i).phoneNumber);
				for (int j = 0; j < Game.players.size(); ++j) {
					if (Game.players.get(j).active) {
						Game.sendToNumber("ADD_PL "+Game.players.get(j).team+
								" "+Game.players.get(j).name+" "+
								Game.players.get(j).phoneNumber, from);
						System.out.println("Host: add player "+Game.players.get(j).phoneNumber+" to "+from);						
					}
				}

				return;
			}
		// Player was never here
		Player newPlayer = new Player(mystok.nextToken());
		newPlayer.name = mystok.nextToken();
		newPlayer.phoneNumber = from;
		Game.sendToNumber("ACCEPT " +newPlayer.toString(), from);
		System.out.println("Host: accept player "+from);
		Game.sendToAll("ADD_PL "+newPlayer.team+" "+newPlayer.name+" "+newPlayer.phoneNumber);
		System.out.println("Host: sendToAll: add player "+from);
		newPlayer.active = true;
		Game.players.add(newPlayer);
		for (int j = 0; j < Game.players.size(); ++j) {
			if (Game.players.get(j).active) {
				Game.sendToNumber("ADD_PL "+Game.players.get(j).team+
						" "+Game.players.get(j).name+" "+
						Game.players.get(j).phoneNumber, from);
				System.out.println("Host: add player "+Game.players.get(j).phoneNumber+" to "+from);						
			}
		}


	}

	/**
	 * Parse rejoin message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseRejoin(StringTokenizer mystok)
	{
		Game.player.fromTokens(mystok);
	}

	/**
	 * Parse accept message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseAccept(StringTokenizer mystok)
	{
		Game.player.fromTokens(mystok);
	}

	/**
	 * Parse add_pl message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseAddPl(StringTokenizer mystok)
	{
		// team name #
		Player newPlayer = new Player(mystok.nextToken());
		newPlayer.name = mystok.nextToken();
		newPlayer.phoneNumber = mystok.nextToken();
		for (int i = 0; i < Game.players.size(); ++i)
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(newPlayer.phoneNumber))
				return;
		// Add new player to roster
		newPlayer.active = true;
		Game.players.add(newPlayer);
		System.out.println("Client: added player "+newPlayer.phoneNumber);
	}

	/**
	 * Parse fire message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseFire(StringTokenizer mystok)
	{
		int weapon = new Integer(mystok.nextToken()).intValue();
		Location impact = new Location("na");
		impact.setLongitude(Double.parseDouble(mystok.nextToken()));
		impact.setLatitude(Double.parseDouble(mystok.nextToken()));
		double blast = 0.0;
		double max_dmg = 0.0;
		switch(weapon) {
		case Home.GRENADE_ID:
			blast = Home.GRENADE_BLAST_RADIUS;
			max_dmg = Home.GRENADE_MAX_DAMAGE;
			break;
		case Home.LAUNCHER_ID:
			blast = Home.GRENADE_LAUNCHER_BLAST_RADIUS;
			max_dmg = Home.GRENADE_LAUNCHER_MAX_DAMAGE;
			break;
		case Home.MORTAR_ID:
			blast = Home.MORTAR_BLAST_RADIUS;
			max_dmg = Home.MORTAR_MAX_DAMAGE;
			break;
		}
		double distance = LocationStuff.calculate_distance(RadarThread.player_location, impact);
		if (distance <= blast) {
			// I got hit
			if (Game.player.hasArmor) {
				Game.player.hasArmor = false;
			} else {
				int dmg = (int) Math.round(((blast-distance) / blast) * max_dmg);
				Game.sendToNumber("HIT " + dmg, from);
				Game.player.killed += dmg;
			}
		}
	}

	/**
	 * Parse hit message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseHit(StringTokenizer mystok)
	{
		Game.player.kills += new Integer(mystok.nextToken()).intValue();
	}

	/**
	 * parse mine message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseMine(StringTokenizer mystok)
	{
		Mine newMine = new Mine(from);
		newMine.setLongitude(new Double(mystok.nextToken()).doubleValue());
		newMine.setLatitude(new Double(mystok.nextToken()).doubleValue());
		Game.mines.add(newMine);
	}

	/**
	 * Parse mine_des message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseMineDes(StringTokenizer mystok)
	{
		Location newMine = new Location("na");
		newMine.setLongitude(new Double(mystok.nextToken()).doubleValue());
		newMine.setLatitude(new Double(mystok.nextToken()).doubleValue());
		for (int i = 0; i < Game.mines.size(); ++i) {
			if (Game.mines.get(i).getLatitude() == newMine.getLatitude() &&
					Game.mines.get(i).getLongitude() == newMine.getLongitude()) {
				Game.mines.remove(i);
				return;
			}
		}
	}

	/**
	 * Parse my_pos message (currently unimplemented)
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseMyPos(StringTokenizer mystok)
	{
		// TODO 
		// lat = Double.parseDouble(mystok.nextToken());
		// lon = Double.parseDouble(mystok.nextToken());

	}

	/**
	 * Parse drop message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseDrop(StringTokenizer mystok)
	{
		DropBox box = new DropBox();
		box.fromStok(mystok);
		synchronized(RadarThread.locker) {
			Game.dropBoxes.add(box);
		}
	}

	/**
	 * Parse drop_pu message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseDropPU(StringTokenizer mystok)
	{
		Location loc = new Location("na");
		loc.setLongitude(new Double(mystok.nextToken()).doubleValue());
		loc.setLatitude(new Double(mystok.nextToken()).doubleValue());
		for (int i = 0; i < Game.dropBoxes.size(); ++i) {
			if (loc.getLongitude() == Game.dropBoxes.get(i).getLongitude() &&
					loc.getLatitude() == Game.dropBoxes.get(i).getLatitude()) {
				Game.dropBoxes.remove(i);
				return;
			}
		}
	}

	/**
	 * Parse leave message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseLeave(StringTokenizer mystok)
	{
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(from)) {
				Game.players.get(i).fromTokens(mystok);
				Game.players.get(i).active = false;
				Game.sendToAll("PL_LEFT "+from);
				System.out.println("Host: player "+from+" is leaving");
				return;
			}
		}
	}

	/**
	 * Parse left message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseLeft(StringTokenizer mystok)
	{
		String id = mystok.nextToken();
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(id)) {
				Game.players.get(i).active = false;
				System.out.println("Client: player "+Game.players.get(i).phoneNumber+" left");
				return;
			}
		}
	}

	/**
	 * Parse mystat message
	 * @param mystok rest of the tokesn in message
	 * @param context apps base context
	 */
	private static void parseMyStat(StringTokenizer mystok, Context context)
	{
		System.out.println("Inside parseMySTat");
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(from)) {
				Game.players.get(i).kills = new Integer(mystok.nextToken()).intValue();
				Game.players.get(i).killed = new Integer(mystok.nextToken()).intValue();
				Game.players.get(i).active = false;
				System.out.println("Setting " + i + " to false");
				break;
			}
		}
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).active == true)
				return;
		}
		// Sort players
		int topPlayer = -1;
		int topScore = Game.player.kills - Game.player.killed;
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).kills - Game.players.get(i).killed > topScore) {
				topScore = Game.players.get(i).kills - Game.players.get(i).killed;
				topPlayer = i;
			}
		}
		if (topPlayer == -1) {
			Game.sendToAll("END "+Game.player.name);
			Toast.makeText(context, "Winner was "+Game.player.name, Toast.LENGTH_LONG).show();
			Game.gameOn = false;
		} else {
			Game.sendToAll("END "+Game.players.get(topPlayer).name);
			Toast.makeText(context, "Winner was "+Game.players.get(topPlayer).name, Toast.LENGTH_LONG).show();
			Game.gameOn = false;
		}
	}

	/**
	 * Parse end message
	 * @param mystok rest of the tokesn in message
	 * @param context apps base context
	 */
	private static void parseEnd(StringTokenizer mystok, Context context)
	{
		Toast.makeText(context, "Winner was "+mystok.nextToken(), Toast.LENGTH_LONG).show();
		Game.leave = true;
	}
	/**
	 * Parse newname message
	 * @param mystok rest of the tokesn in message
	 */
	private static void parseNewName(StringTokenizer mystok) {
		// name phonenmber
		String name = mystok.nextToken();
		String phoneNumber = mystok.nextToken();
		for (int i = 0; i < Game.players.size(); ++i) {
			if (Game.players.get(i).phoneNumber.equalsIgnoreCase(phoneNumber)) {
				Game.players.get(i).name = name;
				System.out.println("Client: "+Game.players.get(i).phoneNumber+
						" changed name to "+Game.players.get(i).name);
				return;
			}
		}
	}

}