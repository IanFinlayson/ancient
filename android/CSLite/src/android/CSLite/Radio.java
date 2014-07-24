package android.CSLite;

import android.content.Context;
import android.media.MediaPlayer;


/**
 * This class plays several Sound effects on the
 * players phone.  It doesn't send anything to other
 * players.  THat is done by sending an appropriate
 * text message and the decoding it in message receiver
 */
public class Radio {
	public static int BOMB_PLANTED = R.raw.bombpl;
	public static int TERRORISTS_WIN = R.raw.terwin;
	public static int COUNTER_TERRORISTS_WIN = R.raw.ctwin;

	public static int ENEMY_SPOTTED = R.raw.ct_enemys;
	public static int FIRE_IN_THE_WHOLE = R.raw.ct_fireinhole;
	public static int FALL_BACK = R.raw.fallback;
	public static int FOLLOW_ME = R.raw.followme;
	public static int MOVE_OUT = R.raw.moveout;
	public static int HOLD_POSITION = R.raw.position;
	public static int REGROUP = R.raw.regroup;
	public static int ROGER = R.raw.regroup;

	private static MediaPlayer media_player = null;

	/**
	 * Plays a sound file on the players phone
	 * @param ctxt the context we are in to play
	 * @param id the id of the sound effect to play (given above)
	 */
	public static void play(Context ctxt, int id) {
		media_player = MediaPlayer.create(ctxt, id);
		media_player.start( );
	}
}
