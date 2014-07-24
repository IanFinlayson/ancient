package fsu.cs.alpha;
import android.app.*;
import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * A simple dialog to get the name from the user
 *
 */
public class NameDialog extends Dialog {

	/**
	 * make the dialog
	 * @param context the context we will run under - don't pass getBaseContext, pass 'this' from activity
	 * @param msg the message you want to send player - besides 'enter name'
	 */
	public NameDialog(Context context, String msg) {
		super(context);
		this.setContentView(R.layout.name_dialog);		
		final NameDialog me = this;
		
		/* can't get around it */
		setCancelable(false);

		/* get our controls */
		final EditText entry = (EditText) findViewById(R.id.EditText01);
		final Button button = (Button) findViewById(R.id.Button01);
		final TextView heading = (TextView) findViewById(R.id.TextView01);
		
		/* clear the text */
		entry.setText("");
			
		heading.setText(msg + " Enter Your Name:");
		
		button.setOnClickListener(new Button.OnClickListener( ) {
            public void onClick(View v) {
            	Game.player_name = entry.getText( ).toString( );
            	me.dismiss( );
            }
        });
		
	}
}
