package fsu.tictactoe;

import android.app.Activity;
import android.graphics.drawable.AnimationDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;


/* animated button class */
class AnimatedButton {
	
	public ImageButton button;		/* which button we control */
	Game the_game;			/* refernce to the game object */
	int cell_number;		/* which cell do I refer to */
	boolean taken;			/* each cell can only be taken once */
	
	static Object animation_mutex = new Object( );
	
	/* callback handler */
	final Handler animation_handler = new Handler( );
	
	/* runnable for posting refresh messages */
	final Runnable update_gui = new Runnable() {
	    public void run() {
	    	setImage( );
	    }
	};

	/* how long to hold each image */
	static final long DELAY_MS = 125;
	
	/* which stage each animation is on */
	int stage;
	
	/* X goes first, so we know which turn is which */
	static int global_turn = 0;
	
	int turn;
	
	public void setImage( ) {
		if((turn % 2) == 0) {
			switch(stage) {
				case 0: button.setImageResource(R.drawable.x0); break;
				case 1:	button.setImageResource(R.drawable.x1);	break;
				case 2:	button.setImageResource(R.drawable.x2);	break;
				case 3: button.setImageResource(R.drawable.x3); break;
				case 4:	button.setImageResource(R.drawable.x4);	break;
				case 5:	button.setImageResource(R.drawable.x5);	break;
				case 6: button.setImageResource(R.drawable.x6); break;
				case 7:	button.setImageResource(R.drawable.x7);	break;
			}
		} else {
			switch(stage) {
				case 0:	button.setImageResource(R.drawable.o0);	break;
				case 1:	button.setImageResource(R.drawable.o1);	break;
				case 2:	button.setImageResource(R.drawable.o2);	break;
				case 3:	button.setImageResource(R.drawable.o3);	break;
				case 4:	button.setImageResource(R.drawable.o4);	break;
				case 5:	button.setImageResource(R.drawable.o5);	break;
				case 6:	button.setImageResource(R.drawable.o6);	break;
				case 7:	button.setImageResource(R.drawable.o7);	break;
		}
		}
	}
	
	AnimatedButton(ImageButton b, Game g, int cell) {
		/* store references */
		button = b;
		the_game = g;
		cell_number = cell;
		global_turn = 0;
		taken = false;
		
		/* event handler */
		button.setOnClickListener(new Button.OnClickListener( ) {
            public void onClick(View v) {
            	/* prevent double takes */
            	if(taken) {
            		the_game.status.setText("This Cell is Already Taken");
            		return;
            	}
            	taken = true;
            	
            	/* Fire off a thread to do animation */
            	synchronized(animation_mutex) {
    				/* next players turn */
    				turn = global_turn;
    				global_turn++;
    				
            		Thread animation_thread = new Thread() {
            			public void run() {	           				
            				/* run the animation */
            				for(stage = 0; stage<8; stage++) {
            					animation_handler.post(update_gui);
            					SystemClock.sleep(DELAY_MS);
            				}
            			}
            		};
            		animation_thread.start( );
            	
            		/* update the game */
            		the_game.update(cell_number, turn);
            	}
            }
        });
	}
}


public class Game extends Activity {
	
	/* number of players */
	int num_players;
	
	enum Cell {
		OPEN,
		X,
		O
	}
	
	enum Outcome {
		NONE,
		P1_WON,
		P2_WON,
		COMPUTER_WON,
		CAT
	}
	
	private Cell [] cells;
	
	public void setupGame( ) {
		cells = new Cell [9];
		
		for(int i=0; i<9; i++) {
			cells[i] = Cell.OPEN;
		}
	}
	
	public boolean checkTriple(int c1, int c2, int c3, Cell value) {
		if((cells[c1] == cells[c2]) && (cells[c2] == cells[c3]) && (cells[c3] == value))
			return true;
		else
			return false;
	}
	
	public Outcome checkGame( ) {
		/* I wish Java had macros :( */
		
		/* top row */
		if(checkTriple(0, 1, 2, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(0, 1, 2, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* middle row */
		if(checkTriple(3, 4, 5, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(3, 4, 5, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* bottom row */
		if(checkTriple(6, 7, 8, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(6, 7, 8, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* left col */
		if(checkTriple(0, 3, 6, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(0, 3, 6, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* middle col */
		if(checkTriple(1, 4, 7, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(1, 4, 7, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* right col */
		if(checkTriple(2, 5, 8, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(2, 5, 8, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/*     /     */
		if(checkTriple(2, 4, 6, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(2, 4, 6, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/*     \     */
		if(checkTriple(0, 4, 8, Cell.X)) return Outcome.P1_WON;
		else if(checkTriple(0, 4, 8, Cell.O)) {
			return (num_players == 1) ? Outcome.COMPUTER_WON : Outcome.P2_WON;
		}
		
		/* check for CATS game */
		boolean cats = true;
		for(int i = 0; i < 9; i++) {
			if(cells[i] == Cell.OPEN)
				cats = false;
		}
		
		if(cats)
			return Outcome.CAT;
		else
			return Outcome.NONE;
	}
	
	public void finishGame(Outcome out, int call_no, String p1_name, String p2_name) {
		
		/* new status */
		final TextView status2 = (TextView) findViewById(R.id.TextView01);
		
		if(call_no == 1) {
			/* set the win animation */
			setContentView(R.layout.win);
			status2.setText(status.getText( ));
			
			final ImageView img = (ImageView) findViewById(R.id.ImageView01);
			AnimationDrawable ad = (AnimationDrawable) getBaseContext( ).getResources( )
				.getDrawable(R.anim.fworks);
			
			img.setImageDrawable(ad);
			ad.start( );
			
			
			/* this dialog will call back with call_no = 2 and p1_name in place*/
			NameDialog diag = new NameDialog(this, out, 1, "", num_players);
			diag.show( );
			return;
		} else if(call_no == 2) {
			if(num_players == 2) {
				/* this dialog will call back with call_no = 3 and p1_name and p2_name in place*/
				NameDialog diag = new NameDialog(this, out, 2, p1_name, num_players);
				diag.show( );
				return;
			}
		}
		
		/* we have the names we need! */
		if(p2_name == "") p2_name = "Computer";
		
		/* set debug text */
		String winner = (out == Outcome.P1_WON) ? p1_name : p2_name;
		String loser = ((out == Outcome.P2_WON) || (out == Outcome.COMPUTER_WON)) ? p1_name : p2_name;
		if(out == Outcome.CAT)
			status2.setText(p1_name + " tied " + p2_name);
		else
			status2.setText(winner + " beat " + loser);
		
		
		/* open up the stats database */
		StatsDatabase db = new StatsDatabase(this);
		db.open( );
		
		/* record p1 stats */
		switch(out) {
			case P1_WON:
				db.recordWin(p1_name);
				break;
			case P2_WON:
			case COMPUTER_WON:
				db.recordLoss(p1_name);
				break;
			case CAT:
				db.recordTie(p1_name);
				break;
		}	
		
		if(num_players == 2) {
			/* record p2 stats */
			switch(out) {
				case P2_WON:
					db.recordWin(p2_name);
					break;
				case P1_WON:
					db.recordLoss(p2_name);
					break;
				case CAT:
					db.recordTie(p2_name);
					break;
			}
		}

		/* close up the database */
		db.close();
        
		/* leave this activity */
		finish( );
	}
	
	public boolean canWin(int index, Cell player) {
		/* save old val */
		Cell old = cells[index];
		
		/* overwrite */
		cells[index] = player;
		
		/* does this result in win? */
		boolean can;
		Outcome out = checkGame( );
		if((out == Outcome.NONE) || (out == Outcome.CAT))
			can = false;
		else
			can = true;
		
		/* restore value */
		cells[index] = old;
		
		return can;
	}
	
	public int rankMove(int index) {
		/* if the space is taken, it is a 0 */
		if(cells[index] != Cell.OPEN)
			return 0;
		
		/* check if we can win here */
		if(canWin(index, Cell.O))
			return 100;
		
		/* check if opponent could win here */
		if(canWin(index, Cell.X))
			return 50;
		
		/* center square */
		if(index == 4)
			return 25;
		
		/* corner */
		if((index == 0) || (index == 2) || (index == 6) || (index == 8))
			return 10;
		
		/* meh */
		return 5;
		
	}
	
	public void doAi( ) {
		int rankings [] = new int[9];
	
		/* get the rankings */
		for(int i = 0; i < 9; i++) {
			if(cells[i] == Cell.OPEN) {
				rankings[i] = rankMove(i);
			}
		}
		
		/* choose best ranking */
		int best_ranking = 0;
		for(int i = 0; i < 9; i++) {
			if(rankings[i] > rankings[best_ranking])
				best_ranking = i;
		}
		
		/* go with the best ranking */
		buttons[best_ranking].button.performClick( );
	}
	
	/* called when a button is clicked */
	public void update(int cell, int turn) {
		/* set the cell */
		cells[cell] = ((turn % 2) == 0) ? Cell.X : Cell.O;
		
		/* check for a winner */
		Outcome o;
		switch(o = checkGame( )) {
			case P1_WON:
				status.setText("Player 1 Won!!");
				finishGame(o, 1, "", "");
				break;
			case P2_WON:
				status.setText("Player 2 Won!!");
				finishGame(o, 1, "", "");
				break;
			case COMPUTER_WON:
				status.setText("You Have Lost!!");
				finishGame(o, 1, "", "");
				break;
			case CAT:
				status.setText("It's a CAT Game!!");
				finishGame(o, 1, "", "");
				break;
			case NONE:
				status.setText(((turn % 2) == 0) ? "O's Turn" : "X's Turn"); 
				break;
		}
		
		/* time to do computer move if needed */
		if((num_players == 1) && ((turn % 2) == 0) && (o == Outcome.NONE))
			doAi( );
	}
	
	
	/* our buttons */
	private AnimatedButton buttons [] = new AnimatedButton [9];
	
	/* text view for debugging */
	public TextView status;
	
	/* setup buttons and interface */
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game);
        
        setupGame( );
        
        status = (TextView) findViewById(R.id.TextView01);
        
        /* set number of players */
        if(getIntent( ).getType( ).equalsIgnoreCase("1"))
        	num_players = 1;
        else 
        	num_players = 2;
        
        status.setText("X's Turn");
        
        
        /* setup buttons */
        int button_ids [] = {R.id.ImageButton01, R.id.ImageButton02, R.id.ImageButton03,
        					 R.id.ImageButton04, R.id.ImageButton05, R.id.ImageButton06,
        					 R.id.ImageButton07, R.id.ImageButton08, R.id.ImageButton09}; 

        for(int i=0; i<9; i++) {
        	buttons[i] = new AnimatedButton((ImageButton) findViewById(button_ids[i]), this, i);
        }   
	}
}













