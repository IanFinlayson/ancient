package fsu.cs.alpha;


import java.text.NumberFormat;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

class Data {
	public long index;
	public String name;
	public int wins;
	public int losses;
}


 
/**
 * database for storing stats of wins/losses/ties for each player
 *
 */
public class StatsDatabase { 

	/* our rows */
    public static final String KEY_ROWID = "_id";
    public static final String KEY_NAME = "name";
    public static final String KEY_WINS = "wins";
    public static final String KEY_LOSSES = "losses";
    
    @SuppressWarnings("unused")
	private static final String TAG = "DBAdapter";
    
    /* db info */
    private static final String DATABASE_NAME = "statsdb";
    private static final String DATABASE_TABLE = "stats";
    private static final int DATABASE_VERSION = 1;

    /* command to make the db  */
    private static final String DATABASE_CREATE =
        "create table stats (_id integer primary key autoincrement, "
        + "name text not null, wins text not null, " 
        + "losses text not null);";
        
    private final Context context; 
    private DatabaseHelper DBHelper;
    private SQLiteDatabase db;

    
    /**
     * constructor
     * @param ctx the context to use
     */
    public StatsDatabase(Context ctx) 
    {
        this.context = ctx;
        DBHelper = new DatabaseHelper(context);
    }
        
    private static class DatabaseHelper extends SQLiteOpenHelper 
    {
        DatabaseHelper(Context context) 
        {
            super(context, DATABASE_NAME, null, DATABASE_VERSION);
        }

        public void onCreate(SQLiteDatabase db) 
        {
            db.execSQL(DATABASE_CREATE);
        }

		public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) { }
    }    
    

    /**
     * open the db
     * @return A databse to put stats in
     * @throws SQLException error checking??
     */
    public StatsDatabase open( ) throws SQLException 
    {
        db = DBHelper.getWritableDatabase();
        return this;
    }

    
    /**
     * closes the database
     */
    public void close( ) 
    {
        DBHelper.close( );
    }
    
    
    /**
     * insert a statistic into the database
     * @param name player name
     * @param wins number of wins
     * @param losses number of losses
     * @return the index it was put at
     */
    public long insertStat(String name, String wins, String losses) 
    {
        ContentValues initialValues = new ContentValues( );
        initialValues.put(KEY_NAME, name);
        initialValues.put(KEY_WINS, wins);
        initialValues.put(KEY_LOSSES, losses);
        return db.insert(DATABASE_TABLE, null, initialValues);
    }


    /**
     * delete a statistic from the db
     * @param rowId the row to remove from
     * @return who knows?
     */
    public boolean deleteStat(long rowId) 
    {
        return db.delete(DATABASE_TABLE, KEY_ROWID + "=" + rowId, null) > 0;
    }


    
    /**
     * get a cursor to all stats in the db
     * @return the cursror
     */
    public Cursor getAllStats( ) 
    {
        return db.query(DATABASE_TABLE, new String[] {
        		KEY_ROWID, 
        		KEY_NAME,
        		KEY_WINS,
                KEY_LOSSES}, 
                null, 
                null, 
                null, 
                null, 
                null);
    }

    

    /**
     * get a stat from the db
     * @param rowId the row you want to get
     * @return a cursor to the reqested
     * @throws SQLException
     */
    public Cursor getStat(long rowId) throws SQLException 
    {
        Cursor mCursor =
                db.query(true, DATABASE_TABLE, new String[] {
                		KEY_ROWID,
                		KEY_NAME, 
                		KEY_WINS,
                		KEY_LOSSES
                		}, 
                		KEY_ROWID + "=" + rowId, 
                		null,
                		null, 
                		null, 
                		null, 
                		null);
        if (mCursor != null) {
            mCursor.moveToFirst();
        }
        return mCursor;
    }

    

    /**
     * update a field from the db
     * @param rowId row to update
     * @param name player name
     * @param wins number wins
     * @param losses number losses
     * @return who knows?
     */
    public boolean updateStat(long rowId, String name, String wins, String losses) 
    {
        ContentValues args = new ContentValues( );
        args.put(KEY_NAME, name);
        args.put(KEY_WINS, wins);
        args.put(KEY_LOSSES, losses);
        return db.update(DATABASE_TABLE, args, KEY_ROWID + "=" + rowId, null) > 0;
    }
    
    
    /**
     * find a name in the db and return info
     * @param name name
     * @return Data object to look at
     */
    public Data findName(String name) {
    	Data d = new Data( );
        Cursor c = getAllStats( );
        if(c.moveToFirst( ))
        {
            do {
            	if(c.getString(1).equals(name)) {
            		d.index = Long.parseLong(c.getString(0));
            		d.name = c.getString(1);
            		d.wins = Integer.parseInt(c.getString(2));
            		d.losses = Integer.parseInt(c.getString(3));
            		
            		return d;
            	}
            } while (c.moveToNext( ));
        }
        
        return null;
    }
    
    /**
     * helper for the two functions below
     * @param name player name
     * @param plus_wins amount to add to current wins
     * @param plus_losses amount to subtract from current losses
     */
    public void doEntry(String name, int plus_wins, int plus_losses) {
    	NumberFormat nf = NumberFormat.getInstance( );
    	nf.setGroupingUsed(false);
    	
    	Data entry = findName(name);
    	if(entry == null) {
    		/* make new record */
    		insertStat(name,
            		nf.format(plus_wins),
            		nf.format(plus_losses));
    		
    	} else {
    		/* update existing record */
    		updateStat(entry.index, entry.name,
    				nf.format(entry.wins + plus_wins),
    				nf.format(entry.losses + plus_losses));
    		
    	}
    	
    }
    
    /**
     * add a win to player or start them at 1-0
     * @param name player name
     */
    public void recordOneWin(String name) {
    	doEntry(name, 1, 0);
    }
    
    /**
     * add a loss to player or start them at 0-1
     * @param name player name
     */
    public void recordOneLoss(String name) {
    	doEntry(name, 0, 1);
    }
}



