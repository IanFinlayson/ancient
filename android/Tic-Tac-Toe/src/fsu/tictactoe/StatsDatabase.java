package fsu.tictactoe;

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
	public int ties;
}


/* database for storing stats of wins/losses/ties for each player */
public class StatsDatabase { 

	/* our rows */
    public static final String KEY_ROWID = "_id";
    public static final String KEY_NAME = "name";
    public static final String KEY_WINS = "wins";
    public static final String KEY_LOSSES = "losses";
    public static final String KEY_TIES = "ties";
    
    @SuppressWarnings("unused")
	private static final String TAG = "DBAdapter";
    
    /* db info */
    private static final String DATABASE_NAME = "stats4";
    private static final String DATABASE_TABLE = "stats";
    private static final int DATABASE_VERSION = 1;

    /* command to make the db  */
    private static final String DATABASE_CREATE =
        "create table stats (_id integer primary key autoincrement, "
        + "name text not null, wins text not null, " 
        + "losses text not null, ties text not null);";
        
    private final Context context; 
    private DatabaseHelper DBHelper;
    private SQLiteDatabase db;

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
    
    /* open db */
    public StatsDatabase open( ) throws SQLException 
    {
        db = DBHelper.getWritableDatabase();
        return this;
    }

    /* close db */    
    public void close( ) 
    {
        DBHelper.close( );
    }
    
    /* insert stat into db */
    public long insertStat(String name, String wins, String losses, String ties) 
    {
        ContentValues initialValues = new ContentValues( );
        initialValues.put(KEY_NAME, name);
        initialValues.put(KEY_WINS, wins);
        initialValues.put(KEY_LOSSES, losses);
        initialValues.put(KEY_TIES, ties);
        return db.insert(DATABASE_TABLE, null, initialValues);
    }

    /* delete stat from db */
    public boolean deleteStat(long rowId) 
    {
        return db.delete(DATABASE_TABLE, KEY_ROWID + "=" + rowId, null) > 0;
    }

    /* get all stats */
    public Cursor getAllStats( ) 
    {
        return db.query(DATABASE_TABLE, new String[] {
        		KEY_ROWID, 
        		KEY_NAME,
        		KEY_WINS,
                KEY_LOSSES,
                KEY_TIES}, 
                null, 
                null, 
                null, 
                null, 
                null);
    }

    /* get a particular stat */
    public Cursor getStat(long rowId) throws SQLException 
    {
        Cursor mCursor =
                db.query(true, DATABASE_TABLE, new String[] {
                		KEY_ROWID,
                		KEY_NAME, 
                		KEY_WINS,
                		KEY_LOSSES,
                		KEY_TIES
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

    /* updates a stat */
    public boolean updateStat(long rowId, String name, String wins, String losses, String ties) 
    {
        ContentValues args = new ContentValues( );
        args.put(KEY_NAME, name);
        args.put(KEY_WINS, wins);
        args.put(KEY_LOSSES, losses);
        args.put(KEY_TIES, ties);
        return db.update(DATABASE_TABLE, args, KEY_ROWID + "=" + rowId, null) > 0;
    }
    
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
            		d.ties = Integer.parseInt(c.getString(4));
            		
            		return d;
            	}
            } while (c.moveToNext( ));
        }
        
        return null;
    }
    
    public void doEntry(String name, int plus_wins, int plus_losses, int plus_ties) {
    	NumberFormat nf = NumberFormat.getInstance( );
    	nf.setGroupingUsed(false);
    	
    	Data entry = findName(name);
    	if(entry == null) {
    		/* make new record */
    		insertStat(name,
            		nf.format(plus_wins),
            		nf.format(plus_losses),
            		nf.format(plus_ties));
    		
    	} else {
    		/* update existing record */
    		updateStat(entry.index, entry.name,
    				nf.format(entry.wins + plus_wins),
    				nf.format(entry.losses + plus_losses),
    				nf.format(entry.ties + plus_ties));
    		
    	}
    	
    }
    
    public void recordWin(String name) {
    	doEntry(name, 1, 0, 0);
    }
    
    public void recordLoss(String name) {
    	doEntry(name, 0, 1, 0);
    }
    
    public void recordTie(String name) {
    	doEntry(name, 0, 0, 1);
    }
}








