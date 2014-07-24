/***********************************************
	CSCI 440  Assignment 1
	Ian Finlayson 2006
	Draws a simple, wire frame Penguin.
***********************************************/

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


/* Change in degrees around penguin radially.
   The lower this number, the smoother penguin will be. (but the smaller his appendages!)
   Should dived 90 evenly! */
/* Possible values (1, 2, 3, 5, 6, 9, 10, 15, 18, 30 45, 90) but 30 looks the best! */
#define DELTA_THETA 30
#define PI 3.141592653589


/* global spin amount */
GLfloat spin = 0.0;


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
}


/* draws a penguin centered (roughly) at the origin */
void drawPenguin( void )
{
	/* draw main body and head black */
	glColor3f( 0.0, 0.0, 0.0 );
	drawHead( );
	drawBody( );

	/* draw the beak orange */
	glColor3f( 1.0, 0.5, 0.0 );
	drawBeak( );

	/* draw penguin's left appendages (wing black, foot orange) */
	glColor3f( 0.0, 0.0, 0.0 );
	drawLeftWing( );
	glColor3f( 1.0, 0.5, 0.0 );
	drawLeftFoot( );

	/* to draw the penguins' right appendages, we just invert the world
	   along the X axis, and draw the left ones over again... */
	glColor3f( 0.0, 0.0, 0.0 );
	glScalef( -1.0, 1.0, 1.0 );
	drawLeftWing( );
	glColor3f( 1.0, 0.5, 0.0 );
	drawLeftFoot( );
}


/* Our display callback  */
void display( void )
{
	/* we want it drawn in wireframe mode */
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	/* clear the screen and the modelview matrix */
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity( );

	/* spin it */
	glRotatef( spin, 1.0, 1.0, 1.0 );

	/* I messed up the Z axis, and mininterpreted which
	   way was negative (I did not realize because back-face
	   culling was turned off) This is a hackish fix.	*/
	glScalef( 1.0, 1.0, -1.0 );
	drawPenguin( );

	/* flush the buffer */
	glFlush( );
}


/* Our reshape callback */
void reshape( int w, int h )
{
	glViewport( 0, 0, w, h );
   
	/* Set up an orthographic projection */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	glOrtho( -50.0, 50.0, -50.0, 50.0, -50.0, 50.0 );
	
	/* Reset to the modelview matrix */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}


/* Spins the penguin */
void spinPenguin( void )
{
	spin += 0.2;

	/* Wrap around after one whole cycle */
	if( spin >= 360.0 ) spin -= 360.0;

	/* ask for a redraw */
	glutPostRedisplay( );
}


/* Our mouse handler */
void mouse( int button, int state, int x, int y )
{
	static int spinning = 0;

	if( state == GLUT_DOWN )
	{
		if( !spinning )
		{
			spinning = 1;
			glutIdleFunc( &spinPenguin );
		}
		else
		{
			spinning = 0;
			glutIdleFunc( NULL );
		}
	}
}


/* Creates an OpenGL window  */
void createWindow( int w, int h, const char* title )
{
	glutInitDisplayMode( GLUT_RGB );
	glutInitWindowSize( w, h ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( title );

	/* set the clear color to be white */
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
}


int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	createWindow( 500, 500, "Penguin!" );
	
	/* register our call backs */
	glutDisplayFunc( &display ); 
	glutReshapeFunc( &reshape ); 
	glutMouseFunc( &mouse );

	/* run the program... */
	glutMainLoop( );

	return 0;
}




