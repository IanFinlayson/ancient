/***************************************************
	CSCI 440  Assignment 4
	Ian Finlayson 2006
	Draws a simple, filled in Penguin,
	and let the user control his motions
***************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>


/* Change in degrees around penguin radially.
   The lower this number, the smoother penguin will be. (but the smaller his appendages!)
   Should dived 90 evenly! */
#define DELTA_THETA 30
#define PI 3.141592653589

/* Allows the speed of the app to be changed in one place 
   (My computer at home is *much* slower than those in the labs */
#define SPEED 10.0


/* global transformation amounts */
GLfloat wing_flap_amount = 0.0;		/* wings flap by rotating around Z axis */
GLfloat foot_move_amount = 0.0;		/* feet walk by rotating around X axis */
GLfloat wobble_amount = 0.0;		/* penguin wobbles as he walks around Z axis */


/* We allow the user to zoom in and out. This is done by scaling */
GLfloat scale_amount = 10.0;
GLint increasing = 0;
GLint decreasing = 0;

/* Lets the user rotate the penguin */
GLfloat rot_amount = 180.0;
GLint spin_left = 0;
GLint spin_right = 0;

/* Whether or not to perform the penguin's walk motion */
GLint walking = 0;

/* The penguin's position */
GLfloat penguin_x = 0.0;
GLfloat penguin_y = 0.0;


/* Used to move the penguin to a new position */
GLfloat desired_x = 0.0;
GLfloat desired_y = 0.0;



/* converts degrees to radians */
GLfloat toRadian( int theta )
{
	/* we always use ints for angles so we know exactly how many tris to draw */
	GLfloat rad = (GLfloat)theta;
	return rad*(PI / 180.0);
}


/* draws the head of the penguin */
void drawHead( void )
{
	int theta;
	GLfloat r = 6.0;	/* we use 6 as the default radius, and multiply it for smaller/larger */

	/* the very top portion is like a cone...	*/
	glBegin( GL_TRIANGLE_FAN );
		glVertex3f( 0.0, 48.0, 0.0 );	/* top of the head */

		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)) );
		}
	glEnd( );

	/* the next portion is like a cylindar going down from that... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)) );
			glVertex3f( r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3 );
		}
	glEnd( );

	/* the next portion is like another cylindar, but with different slope... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3 );
			glVertex3f( r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3 );
		}
	glEnd( );

	/* yet another cylinder like thing */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3  );
			glVertex3f( r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta)) );
		}
	glEnd( );
}


/* draws the body of the penguin */
void drawBody( void )
{
	int theta;
	GLfloat r = 6.0;

	/* this quad strip connects the head with the body some... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta))  );
			glVertex3f( r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2 );
		}
	glEnd( );

	/* this quad strip extends the body out some more... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2  );
			glVertex3f( r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5 );
		}
	glEnd( );

	/* this quad strip extends is much longer, making up much of the torso */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5 );
			glVertex3f( r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8 );
		}
	glEnd( );

	/* this quad strip begins the taper back in */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8 );
			glVertex3f( r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5 );
		}
	glEnd( );

	/* this quad strip slopes the body in even further */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5 );
			glVertex3f( r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9 );
		}
	glEnd( );

	/* this quad strip almost closes the body */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9 );
			glVertex3f( r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)) );
		}
	glEnd( );

	/* this triangle fan closes the penguin at the bottom (it is a circle) */
	glBegin( GL_TRIANGLE_FAN );
		glVertex3f( 0.0, -20.0, 0.0 );	/* bottom of the penguin */

		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			glVertex3f( r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)) );
		}
	glEnd( );
}



/* draws the beak of the bird */
void drawBeak( void )
{
    /* we must calculate where the beak is based on
       what the value of DELTA_THETA is so that
       the penguin remains a properly connected entity
       the downside is that his beak size changes based on this */

	GLfloat theta1 = toRadian( 270 - DELTA_THETA );
	GLfloat theta2 = toRadian( 270 + DELTA_THETA );
	GLfloat r = 6.0;

	/* now we calculate the four corners based on these angles */
	GLfloat up_left_x = r*cos( theta1 )*1.3;
	GLfloat up_left_y = 38.0;
	GLfloat up_left_z = r*sin( theta1 )*1.3;

	GLfloat down_left_x = r*cos( theta1 )*1.3;
	GLfloat down_left_y = 33.0;
	GLfloat down_left_z = r*sin( theta1 )*1.3;


	GLfloat up_right_x = r*cos( theta2 )*1.3;
	GLfloat up_right_y = 38.0;
	GLfloat up_right_z = r*sin( theta2)*1.3;

	GLfloat down_right_x = r*cos( theta2 )*1.3;
	GLfloat down_right_y = 33.0;
	GLfloat down_right_z = r*sin( theta2 )*1.3;
	
	
	/* We draw the beak as a pyramid with a triangle fan... */
	glBegin( GL_TRIANGLE_FAN );
		/* the point of the beak... */
		glVertex3f( 0.0, 35.5, -30.0 );
		
		/* other points... */
		glVertex3f( up_right_x, up_right_y, up_right_z );
		glVertex3f( up_left_x, up_left_y, up_left_z );
		glVertex3f( down_left_x, down_left_y, down_left_z );
		glVertex3f( down_right_x, down_right_y, down_right_z );
		glVertex3f( up_right_x, up_right_y, up_right_z );
	glEnd( );
}


/* draws the left wing of the penguin */
void drawLeftWing( void )
{
	/* first we must find the points to connect the wing to 
	(just like we did for the beak ) */

	GLfloat theta1 = toRadian( 180 - DELTA_THETA );
	GLfloat theta2 = toRadian( 180 + DELTA_THETA );
	GLfloat r = 6.0;

	/* now we calculate the four corners where the wing should connect to */
	GLfloat up_left_x = r*cos( theta1 )*2.5;
	GLfloat up_left_y = 20.0;
	GLfloat up_left_z = -r*sin( theta1 )*2.5;

	GLfloat down_left_x = r*cos( theta1 )*2.8;
	GLfloat down_left_y = 5.0;
	GLfloat down_left_z = -r*sin( theta1 )*2.8;


	GLfloat up_right_x = r*cos( theta2 )*2.5;
	GLfloat up_right_y = 20.0;
	GLfloat up_right_z = -r*sin( theta2)*2.5;

	GLfloat down_right_x = r*cos( theta2 )*2.8;
	GLfloat down_right_y = 5.0;
	GLfloat down_right_z = -r*sin( theta2 )*2.8;


	/* We must perform the rotation here */
	/* We must first translate the wing so that it goes about the joint correctly */
	glPushMatrix( );
	glTranslatef( up_left_x, (up_left_y + down_left_y) / 2.0, (up_left_z + up_right_z) / 2.0 );
	glRotatef( wing_flap_amount, 0.0, 0.0, 1.0 );
	glTranslatef( -up_left_x, (up_left_y + down_left_y) / -2.0, (up_left_z + up_right_z) / -2.0 );

	/* we render the wing as a few parallelograms (quad strip)
	   followed by a pyramid (triangle fan) */

	glBegin( GL_QUAD_STRIP );
		/* first quad */
		glVertex3f( up_left_x, up_left_y, up_left_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
		glVertex3f( down_left_x, down_left_y, down_left_z );		
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_left_z );

		/* second */
		glVertex3f( down_right_x, down_right_y, down_right_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_right_z );

		/* third */
		glVertex3f( up_right_x, up_right_y, up_right_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25, up_right_z );

		/* connect it back to the first */
		glVertex3f( up_left_x, up_left_y, up_left_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
	glEnd( );

	glBegin( GL_TRIANGLE_FAN );
		/* the point of the wing */
		glVertex3f( down_left_x - 20.0, down_left_y - 25.0, 0.0 );

		/* the other points (same as ones above...) */
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_left_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_right_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25, up_right_z );
	glEnd( );

	glPopMatrix( );
}


/* draws the left foot of the penguin */
void drawLeftFoot( void )
{
	/* Again, we must calculate the points to connect to the body */
	GLfloat theta1 = toRadian( 270 - 2*DELTA_THETA );
	GLfloat theta2 = toRadian( 270 - DELTA_THETA );
	GLfloat r = 6.0;

	GLfloat front_left_x = r*cos( theta1 )*1.9;
	GLfloat front_left_y = -18.0;
	GLfloat front_left_z = r*sin( theta1 )*1.9;

	GLfloat back_left_x = r*cos( theta1 );
	GLfloat back_left_y = -20.0;
	GLfloat back_left_z = r*sin( theta1 );

	
	GLfloat front_right_x = r*cos( theta2 )*1.9;
	GLfloat front_right_y = -18.0;
	GLfloat front_right_z = r*sin( theta2 )*1.9;

	GLfloat back_right_x = r*cos( theta2 );
	GLfloat back_right_y = -20.0;
	GLfloat back_right_z = r*sin( theta2 );


	/* We rotate the foot to give the illusion of walking */
	glPushMatrix( );

	/* We must translate to the joint and back so his foot pivots properly */
	glTranslatef( (front_left_x + front_right_x) /2.0, front_left_y, (front_left_z + front_right_z) / 2.0  );
	glRotatef( foot_move_amount, 1.0, 0.0, 0.0 );
	glTranslatef( (front_left_x + front_right_x) /-2.0, -front_left_y, (front_left_z + front_right_z) / -2.0  );


	/* We render a guad strip for the 'leg' part... */
	glBegin( GL_QUAD_STRIP );
		glVertex3f( back_left_x, back_left_y, back_left_z );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( front_left_x, front_left_y, front_left_z );
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );

		glVertex3f( front_right_x, front_right_y, front_right_z );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );

		glVertex3f( back_right_x, back_right_y, back_right_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );

		glVertex3f( back_left_x, back_left_y, back_left_z );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
	glEnd( );

	/* now we draw 2 triangles for the sides of his foot */
	glBegin( GL_TRIANGLES );
		/* left side */
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );

		/* right side */
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );
	glEnd( );

	/* now we draw 2 quads to fill in the gaps */
	glBegin( GL_QUADS );
		/* top of foot */
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );

		/* bottom of foot */
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
	glEnd( );

	glPopMatrix( );
}


/* draws a penguin centered (roughly) at the origin */
void drawPenguin( void )
{
	/* draw main body and head black */
	glColor3f( 0.0, 0.0, 0.0 );

	glPushMatrix( );
		/* Wobble the body a little bit */
		glRotatef( wobble_amount, 0.0, 0.0, 1.0 );
		drawHead( );
		drawBody( );

		/* draw the beak orange */
		glColor3f( 1.0, 0.5, 0.0 );
		drawBeak( );
	glPopMatrix( );

	/* draw penguin's left wing black */
	glColor3f( 0.0, 0.0, 0.0 );
	drawLeftWing( );

	/* draw the left foot (orange) */
	glColor3f( 1.0, 0.5, 0.0 );
	drawLeftFoot( );

	/* to draw the penguins' right appendages, we just invert the world
	   along the X axis, and draw the left ones over again... */
	glScalef( -1.0, 1.0, 1.0 );

	glColor3f( 0.0, 0.0, 0.0 );		/* black wing */
	drawLeftWing( );
	
	/* orange */
	glColor3f( 1.0, 0.5, 0.0 );
	/* to make the left and right rotate on an opposite way, we invert the right one... */
	GLfloat temp = foot_move_amount;
	foot_move_amount = abs( 90.0 - foot_move_amount );
	drawLeftFoot( );
	foot_move_amount = temp;	/* reset the value for the left foot */
}


/* Our display callback  */
void display( void )
{
	/* clear the screen, depth buffer,  and the modelview matrix */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity( );

	
	/* move him */
	glTranslatef( penguin_x, penguin_y, 0.0 );

	/* rotate it */
	glRotatef( rot_amount, 0.0, 1.0, 0.0 );

	/* zoom in/out */
	glScalef( scale_amount, scale_amount, scale_amount );

	drawPenguin( );

	/* flush the buffer and swap */
	glFlush( );
	glutSwapBuffers( );
}


/* Our reshape callback */
void reshape( int w, int h )
{
	glViewport( 0, 0, w, h );
   
	/* Set up an orthographic projection */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	glOrtho( -800.0, 800.0, -800.0, 800.0, -800.0, 800.0 );

	/* Reset to the modelview matrix */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}


/* Animates the penguin */
void animate( void )
{
	/* if we're increasing wing amount */
	static int wings_up = 1;

	/* if we're increasing foot amount */
	static int foot_up = 1;

	/* if the penguin is wobbling left */
	static int wobble_left = 1;

	if( walking )
	{
		/* wings rotate in range -75 to 0 */
		if( wing_flap_amount < -75.0 )
			wings_up = 1;
		else if( wing_flap_amount > 0.0 )
			wings_up = 0;

		if( wings_up )
			wing_flap_amount += SPEED*0.5;
		else
			wing_flap_amount -= SPEED*0.5;


		/* the feet rotate in range 0 to 90 */
		if( foot_move_amount < 0.0 )
			foot_up = 1;
		else if( foot_move_amount > 90.0 )
			foot_up = 0;

		if( foot_up )
			foot_move_amount += SPEED*0.5;
		else
			foot_move_amount -= SPEED*0.5;


		/* the penguin wobbles in the range -5 to 5 */
		if( wobble_amount < -5.0 )
			wobble_left = 0;
		else if( wobble_amount > 5.0 )
			wobble_left = 1;

		if( wobble_left )
			wobble_amount -= SPEED*0.1;
		else
			wobble_amount += SPEED*0.1;
	}

	/* Do scaling */
	if( increasing )
		scale_amount += SPEED*0.005;
	else if( decreasing )
	{
		scale_amount -= SPEED*0.005;
		if( scale_amount < 0.0 )
			scale_amount = 0.0;
	}

	/* Do spinning */
	if( spin_left )
	{
		rot_amount -= SPEED*0.1;
		if( rot_amount > 360.0 )
			rot_amount -= 360.0;
	}
	else if( spin_right )
	{
		rot_amount += SPEED*0.1;
		if( rot_amount < -360.0 )
			rot_amount += 360.0;
	}

	/* Do translation */
	if( walking )
	{
		/* Update left coord */
		if( desired_x > penguin_x )
			penguin_x += SPEED*1.0;
		else if( desired_x < penguin_x )
			penguin_x -= SPEED*1.0;

		/* Update right coord */
		if( desired_y > penguin_y )
			penguin_y += SPEED*1.0;
		else if( desired_y < penguin_y )
			penguin_y -= SPEED*1.0;

		/* Test if he's close enough... */
		if( abs( desired_x - penguin_x ) < 2.0 )
		{
			if( abs( desired_y - penguin_y ) < 2.0 )
			{
				walking = 0;
			}
		}
	}


	/* ask for a redraw */
	glutPostRedisplay( );
}



/* Creates an OpenGL window  */
void createWindow( int w, int h, const char* title )
{
	/* I turned on double buffering for this example, because it was
	   extremely painful to watch without it on */
	/* Also turned on is the depth buffer */
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( w, h ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( title );

	/* set the clear color to be white */
	glClearColor( 1.0, 1.0, 1.0, 0.0 );

	/* Enable hidden surface removal */
	glEnable( GL_DEPTH_TEST );
}


/* Mouse handler */
void mouseHandler( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
	{
		/* get the place the user wants him to go */
		desired_x = x*3.2 - 800;
		desired_y = 800 - y*3.2;

		walking = 1;
	}
}


/* Keyboard handler */
void keyHandler( unsigned char key, int x, int y )
{
	switch( key )
	{
	case 27:
		exit( 0 );		/* escape key */
		break;

	case 'q':		/* Zoom out */
		decreasing = 1;
		increasing = 0;
		break;

	case 'w':		/* Stop zoom */
		decreasing = 0;
		increasing = 0;
		break;

	case 'e':		/* Zoom in */
		decreasing = 0;
		increasing = 1;
		break;

	case 'a':		/* Spin left */
		spin_left = 1;
		spin_right = 0;
		break;

	case 's':		/* Stop spinning */
		spin_left = 0;
		spin_right = 0;
		break;

	case 'd':		/* Spin right */
		spin_left = 0;
		spin_right = 1;
		break;
	}
}

/* Writes text to the screen */
void writeString( int x, int y, const char* text )
{
	unsigned int len = strlen( text );

	glRasterPos2f( x, y );
	
	for( unsigned int i=0; i<len; i++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, text[i] );
	}
}

	
/* displays the intro screen */
void displayIntro( void )
{
	/* clear the screen, depth buffer,  and the modelview matrix */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity( );
	glColor3f( 0.0, 0.0, 0.0 );

	// Write our info...
	writeString( -200, 740, "Penguin!" );

	writeString( -780, 540, "Press the \'A\' key to spin the penguin left." );
	writeString( -780, 480, "Press the \'D\' key to spin the penguin right." );
	writeString( -780, 420, "Press the \'S\' key to stop the penguin from spinning." );

	writeString( -780, 220, "Press the \'Q\' key to zoom out." );
	writeString( -780, 160, "Press the \'E\' key to zoom in." );
	writeString( -780, 100, "Press the \'W\' key to stop zooming." );

	writeString( -780, -100, "Click the mouse to make the penguin walk to that point." );

	writeString( -780, -300, "Press Escape to quit at any time." );

	writeString( -780, -700, "Press any key or click the mouse to begin." );


	/* flush the buffer and swap */
	glFlush( );
	glutSwapBuffers( );
}


/* Process mouse events for intro state */
void mouseIntro( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
	{
		glutDisplayFunc( &display );		/* switch to main displaying */
		glutMouseFunc( &mouseHandler );		/* switch to main mouse handler */
		glutKeyboardFunc( &keyHandler );	/* start processing key presses too */
	}
}

/* Process key events for the intro scene */
void keyIntro( unsigned char key, int x, int y )
{
	if( key == 27 )	/* escape */
		exit( 0 );
	else
	{
		glutDisplayFunc( &display );		/* switch to main displaying */
		glutMouseFunc( &mouseHandler );		/* swithc to main mouse handler */
		glutKeyboardFunc( &keyHandler );	/* start processing key presses too */
	}
}


/* main */
int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	createWindow( 500, 500, "Penguin!" );
	
	/* register our call backs */
	glutDisplayFunc( &displayIntro );	/* Start in the intro */
	glutReshapeFunc( &reshape ); 
	glutIdleFunc( &animate );
	glutMouseFunc( &mouseIntro );		/* Start in intro */
	glutKeyboardFunc( &keyIntro );		/* Start in intro */

	/* run the program... */
	glutMainLoop( );

	return 0;
}




