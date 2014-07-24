/***************************************************
	CSCI 440  Assignment 6
	Ian Finlayson 2006

	The penguin walks around the Y axis again, this
	time with perspective. Clincking each mouse button
	will set up a different frustrum to be used.
***************************************************/

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


/*  Change in degrees around penguin radially.
	The lower this number, the smoother penguin will be. (but the smaller his appendages!)
	Possible values (1, 2, 3, 5, 6, 9, 10, 15, 18, 30 45, 90) but 30 looks the best! */
#define DELTA_THETA 30
#define PI 3.141592653589

/* Controls how fast the penguin walsk */
#define SPEED .75

/* Which frustum is being used now */
int current_frustum = 1;

/* global transformation amounts */
GLfloat wing_flap_amount = 0.0;		/* wings flap by rotating around Z axis */
GLfloat foot_move_amount = 0.0;		/* feet walk by rotating around X axis */
GLfloat wobble_amount = 0.0;		/* penguin wobbles as he walks around Y axis */

/* The penguin walks tirelessly about the Y axis */
#define PENGUIN_WALK_RADIUS 75		/* How far he is from the axis */
GLfloat penguin_x = 0.0;			/* His coords (y always is the same */
GLfloat penguin_z = 0.0;
GLfloat penguin_heading = 0.0;	/* we must rotate the penguin so he looks where he is going */


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
/* Also moves him around in a circle */
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

	/* Move out so that the penguin is in the frustum */
	glTranslatef( 0, 0, -200.0 );

	/* transoformations that apply to whole bird */
	glTranslatef( penguin_x, 0.0, penguin_z );		/* move the penguin to his point on the orbit */
	glRotatef( penguin_heading, 0.0, 1.0, 0.0 );	/* face him the right way */
	drawPenguin( );

	/* flush the buffer and swap */
	glFlush( );
	glutSwapBuffers( );
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


	/* wings rotate in range -75 to 0 */
	if( wing_flap_amount < -75.0 )
		wings_up = 1;
	else if( wing_flap_amount > 0.0 )
		wings_up = 0;

	if( wings_up )
		wing_flap_amount += 0.5 * SPEED;
	else
		wing_flap_amount -= 0.5 * SPEED;


	/* the feet rotate in range 0 to 90 */
	if( foot_move_amount < 0.0 )
		foot_up = 1;
	else if( foot_move_amount > 90.0 )
		foot_up = 0;

	if( foot_up )
		foot_move_amount += 0.5 * SPEED;
	else
		foot_move_amount -= 0.5 * SPEED;


	/* the penguin wobbles in the range -5 to 5 */
	if( wobble_amount < -5.0 )
		wobble_left = 0;
	else if( wobble_amount > 5.0 )
		wobble_left = 1;

	if( wobble_left )
		wobble_amount -= 0.1 * SPEED;
	else
		wobble_amount += 0.1 * SPEED;

	/* calculate penguin position based off of angle */
	static GLfloat angle = 0.0;
	penguin_x = PENGUIN_WALK_RADIUS * cos( angle*(PI / 180.0) );
	penguin_z = PENGUIN_WALK_RADIUS * sin( angle*(PI / 180.0) );

	/* calculate heading based off this angle */
	/* The penguin must turn in the opposite direction from his rotation
	so that he faces where he is going */
	penguin_heading = -angle - 180.0;

	/* increment the angle */
	angle += 0.2 * SPEED;
	if( angle > 360.0 )
		angle -= 360.0;

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


/* Keyboard handler */
void keyHandler( unsigned char key, int x, int y )
{
	if( key == 27 )
		exit( 0 );	/* escape key */
}

/* defines the first frustum
   This one makes the penguin look relatively normal */
void frustum1( void )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glFrustum( -5, 5, -5, 5, 10, 1000 );

	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}

/* defines the second frustum
   This one makes the penguin appear taller
   It also makes the penguin seem bigger because the near plane is closer */
void frustum2( void )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glFrustum( -2, 2, -1, 1, 5, 500 );

	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}



/* Mouse handler */
void mouseHandler( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
	{
		if( button == GLUT_LEFT )
		{
			frustum1( );
			current_frustum = 1;
		}
		else
		{
			frustum2( );
			current_frustum = 2;
		}
	}
}

/* Our reshape callback */
void reshape( int w, int h )
{
	glViewport( 0, 0, w, h );
   
	if( current_frustum == 1 )
		frustum1( );
	else
		frustum2( );
}


/* main */
int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	createWindow( 600, 600, "Penguin!" );
	
	/* register our call backs */
	glutDisplayFunc( &display ); 
	glutReshapeFunc( &reshape ); 
	glutIdleFunc( &animate );
	glutKeyboardFunc( &keyHandler );
	glutMouseFunc( &mouseHandler );

	/* run the program... */
	glutMainLoop( );

	return 0;
}




