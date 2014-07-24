/***************************************************
	CSCI 440  Assignment 10
	Ian Finlayson 2006

	Now the scene is foggy and has a snowfield effect.
	Also the penguin has a motion blur effect applied
	to him. The motion blurring KILLS the speed of the program!
***************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

/*  Change in degrees around penguin radially.
	The lower this number, the smoother penguin will be */
#define DELTA_THETA 18
#define PI 3.141592653589

/* Controls how fast the penguin walks */
#define SPEED 10
#define MOTION_BLUR 1	/* Whether or not to do motion blur */

/* Location of the light source
   This is used for the shadow and for where to draw the light */
#define LIGHT_X 30
#define LIGHT_Y 120
#define LIGHT_Z 0

/* global transformation amounts */
GLfloat wing_flap_amount = 0.0;		/* wings flap by rotating around Z axis */
GLfloat foot_move_amount = 0.0;		/* feet walk by rotating around X axis */
GLfloat wobble_amount = 0.0;		/* penguin wobbles as he walks around Y axis */

/* The penguin walks tirelessly about the Y axis */
#define PENGUIN_WALK_RADIUS 75		/* How far he is from the axis */
GLfloat penguin_x = 0.0;			/* His coords (y always is the same */
GLfloat penguin_z = 0.0;
GLfloat penguin_heading = 0.0;	/* we must rotate the penguin so he looks where he is going */

/* Our texture handles */
GLuint ground_handle;
GLuint bg_handle;

/* One of the affects we do is a particle affect
   to simulate snow. We draw a lot of snow flakes */
struct SnowFlake
{
	GLfloat x, y, dx, dy;
};

#define NUM_FLAKES 600
SnowFlake flakes[NUM_FLAKES];


/* creates a normal from three points and gives it to OpenGL (adapted from class code) */
void createNormal( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 )
{
	float nx, ny, nz;

	float vx, vy, vz, wx, wy, wz;
	double length;

	/* compute the vectors V and W */
	vx = x2 - x1;
	vy = y2 - y1;
	vz = z2 - z1;
	wx = x3 - x1;
	wy = y3 - y1;
	wz = z3 - z1;
	
	/* compute the normal vector */
	nx = wy*vz - wz*vy;
	ny = wz*vx - wx*vz;
	nz = wx*vy - wy*vx;
	
	/* normalize the normal vector */
	length = sqrt( nx*nx + ny*ny + nz*nz );

	nx /= length;
	ny /= length;
	nz /= length;

	glNormal3f( nx, ny, nz );
}

/* loads a RAW image and gives it to OpenGL as a texture
   it returns the Opengl texture handle */
GLuint loadTexture( const char* file_name, int w, int h )
{
	glEnable( GL_TEXTURE_2D );

	FILE* file = fopen( file_name, "rb" );
	if( !file )
		printf( "File could not be opened" );
	
	/* allocate space for the image and read it from the file */
	GLubyte* image = (GLubyte*) malloc( w * h * 3 );
	fread( image, w * h * 3, 1, file );
	fclose( file );

	/* Get a valid texture handle and bind it */
	GLuint handle;
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );

	/* send the data to OpenGL */
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image );

	/* Specify filtering */
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
	/* Now we can free the image from ram */
	free( image );

	return handle;
}


/* loadShadow creates the shadow matrix and applies it as a transformation */
void applyShadowMatrix( void )
{
	GLfloat sm[16];
	int i;

	for( i=0; i<16; i++ )
	{
		sm[i] = 0.0;
	}

	sm[0] = sm[5] = sm[10] = 1.0;
	sm[7] = -1.0/LIGHT_Y;

	glMultMatrixf( sm );
}


/* converts degrees to radians */
GLfloat toRadian( int theta )
{
	/* we always use ints for angles so we know exactly how many tris to draw */
	GLfloat rad = (GLfloat)theta;
	return rad*(PI / 180.0);
}

/* Draws all of the snow flakes */
void drawSnowFlakes( void )
{
	glPointSize( 2.0 );		/* larger points */
	glColor3f( 1.0, 1.0, 1.0 );		/* white */
	glBegin( GL_POINTS );
		for( int i=0; i<NUM_FLAKES; i++ )
			glVertex3f( flakes[i].x, flakes[i].y, -100.0 );
	glEnd( );
}


/* draws the head of the penguin */
/* now with normals! */
void drawHead( void )
{
	int theta;
	GLfloat r = 6.0;	/* we use 6 as the default radius, and multiply it for smaller/larger */

	/* the very top portion is like a cone...	*/
	glBegin( GL_TRIANGLE_FAN );
		glVertex3f( 0.0, 48.0, 0.0 );	/* top of the head */

		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( 0.0, 48.0, 0.0,
				r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta + DELTA_THETA)), 45.0, r*sin(toRadian(theta + DELTA_THETA)) );
			
				glVertex3f( r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)) );
		}
	glEnd( );

	/* the next portion is like a cylindar going down from that... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3,
				r*cos(toRadian(theta + DELTA_THETA)), 45.0, r*sin(toRadian(theta + DELTA_THETA)) );
                                

			glVertex3f( r*cos(toRadian(theta)), 45.0, r*sin(toRadian(theta)) );
			glVertex3f( r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3 );
		}
	glEnd( );

	/* the next portion is like another cylindar, but with different slope... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3,
				 r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3,
				 r*cos(toRadian(theta + DELTA_THETA))*1.3, 38.0, r*sin(toRadian(theta + DELTA_THETA))*1.3 );

			glVertex3f( r*cos(toRadian(theta))*1.3, 38.0, r*sin(toRadian(theta))*1.3 );
			glVertex3f( r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3 );
		}
	glEnd( );

	/* yet another cylinder like thing */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3,
				r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta + DELTA_THETA))*1.3, 33.0, r*sin(toRadian(theta + DELTA_THETA))*1.3 );

			glVertex3f( r*cos(toRadian(theta))*1.3, 33.0, r*sin(toRadian(theta))*1.3 );
			glVertex3f( r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta)) );
		}
	glEnd( );
}


/* draws the body of the penguin */
/* now with normals! */
void drawBody( void )
{
	int theta;
	GLfloat r = 6.0;

	/* this quad strip connects the head with the body some... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2,
				r*cos(toRadian(theta + DELTA_THETA)), 26.0, r*sin(toRadian(theta + DELTA_THETA)) );

			glVertex3f( r*cos(toRadian(theta)), 26.0, r*sin(toRadian(theta))  );
			glVertex3f( r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2 );
		}
	glEnd( );

	/* this quad strip extends the body out some more... */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2,
				r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5,
				r*cos(toRadian(theta + DELTA_THETA))*2, 24.0, r*sin(toRadian(theta + DELTA_THETA))*2 );

			glVertex3f( r*cos(toRadian(theta))*2, 24.0, r*sin(toRadian(theta))*2  );
			glVertex3f( r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5 );
		}
	glEnd( );

	/* this quad strip extends is much longer, making up much of the torso */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5,
				r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8,
				r*cos(toRadian(theta + DELTA_THETA))*2.5, 20.0, r*sin(toRadian(theta + DELTA_THETA))*2.5 );

			glVertex3f( r*cos(toRadian(theta))*2.5, 20.0, r*sin(toRadian(theta))*2.5 );
			glVertex3f( r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8 );
		}
	glEnd( );

	/* this quad strip begins the taper back in */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8,
				r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5,
				r*cos(toRadian(theta + DELTA_THETA))*2.8, 5.0, r*sin(toRadian(theta + DELTA_THETA))*2.8 );

			glVertex3f( r*cos(toRadian(theta))*2.8, 5.0, r*sin(toRadian(theta))*2.8 );
			glVertex3f( r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5 );
		}
	glEnd( );

	/* this quad strip slopes the body in even further */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5,
				r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9,
				r*cos(toRadian(theta + DELTA_THETA))*2.5, -10.0, r*sin(toRadian(theta + DELTA_THETA))*2.5 );

			glVertex3f( r*cos(toRadian(theta))*2.5, -10.0, r*sin(toRadian(theta))*2.5 );
			glVertex3f( r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9 );
		}
	glEnd( );

	/* this quad strip almost closes the body */
	glBegin( GL_QUAD_STRIP );
		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9,
				r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta + DELTA_THETA))*1.9, -18.0, r*sin(toRadian(theta + DELTA_THETA))*1.9 );

			glVertex3f( r*cos(toRadian(theta))*1.9, -18.0, r*sin(toRadian(theta))*1.9 );
			glVertex3f( r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)) );
		}
	glEnd( );

	/* this triangle fan closes the penguin at the bottom (it is a circle) */
	glBegin( GL_TRIANGLE_FAN );
		glVertex3f( 0.0, -20.0, 0.0 );	/* bottom of the penguin */

		for( theta=0; theta<=360; theta+=DELTA_THETA )
		{
			createNormal( 0.0, -20.0, 0.0,
				r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)),
				r*cos(toRadian(theta + DELTA_THETA)), -20.0, r*sin(toRadian(theta + DELTA_THETA)) );

			glVertex3f( r*cos(toRadian(theta)), -20.0, r*sin(toRadian(theta)) );
		}
	glEnd( );
}



/* draws the beak of the bird */
/* now with normals! */
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

		createNormal( 0.0, 35.5, -30.0,
			up_right_x, up_right_y, up_right_z,
			up_left_x, up_left_y, up_left_z );
		glVertex3f( up_right_x, up_right_y, up_right_z );
		glVertex3f( up_left_x, up_left_y, up_left_z );
		
		createNormal( 0.0, 35.5, -30.0,
			up_left_x, up_left_y, up_left_z,
			down_left_x, down_left_y, down_left_z );
		glVertex3f( down_left_x, down_left_y, down_left_z );
		

		createNormal( 0.0, 35.5, -30.0,
			down_left_x, down_left_y, down_left_z,
			down_right_x, down_right_y, down_right_z );
		glVertex3f( down_right_x, down_right_y, down_right_z );
		

		createNormal( 0.0, 35.5, -30.0,
			down_right_x, down_right_y, down_right_z,
			up_right_x, up_right_y, up_right_z );
		glVertex3f( up_right_x, up_right_y, up_right_z );
	glEnd( );
}


/* draws the left wing of the penguin */
/* now with normals! */
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
		createNormal( up_left_x - 15.0, up_left_y - 25.0, up_left_z,
			down_left_x, down_left_y, down_left_z,
						up_left_x, up_left_y, up_left_z );

		glVertex3f( up_left_x, up_left_y, up_left_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
		glVertex3f( down_left_x, down_left_y, down_left_z );		
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_left_z );

		/* second */
		createNormal( up_left_x - 15.0, down_left_y - 20.0, down_left_z,
			up_left_x - 15.0, down_left_y - 20.0, down_right_z,
			down_right_x, down_right_y, down_right_z );		

		glVertex3f( down_right_x, down_right_y, down_right_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_right_z );

		/* third */
		createNormal(  up_left_x - 15.0, down_left_y - 20.0, down_right_z,
			up_left_x - 15.0, up_left_y - 25, up_right_z,
			up_right_x, up_right_y, up_right_z );

		glVertex3f( up_right_x, up_right_y, up_right_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25, up_right_z );

		/* connect it back to the first */
		createNormal( up_left_x - 15.0, up_left_y - 25, up_right_z,
			up_left_x - 15.0, up_left_y - 25.0, up_left_z,
			up_left_x, up_left_y, up_left_z );

		glVertex3f( up_left_x, up_left_y, up_left_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
	glEnd( );

	glBegin( GL_TRIANGLE_FAN );
		/* the point of the wing */
		glVertex3f( down_left_x - 20.0, down_left_y - 25.0, 0.0 );

		/* the other points (same as ones above...) */
		createNormal( down_left_x - 20.0, down_left_y - 25.0, 0.0,
			up_left_x - 15.0, down_left_y - 20.0, down_left_z,
			up_left_x - 15.0, up_left_y - 25.0, up_left_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_left_z );

		createNormal( down_left_x - 20.0, down_left_y - 25.0, 0.0,
			up_left_x - 15.0, down_left_y - 20.0, down_left_z,
			up_left_x - 15.0, down_left_y - 20.0, down_right_z );
		glVertex3f( up_left_x - 15.0, down_left_y - 20.0, down_right_z );

		createNormal( down_left_x - 20.0, down_left_y - 25.0, 0.0,
			up_left_x - 15.0, up_left_y - 25, up_right_z,
			up_left_x - 15.0, down_left_y - 20.0, down_right_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25, up_right_z );

		createNormal( down_left_x - 20.0, down_left_y - 25.0, 0.0,
			up_left_x - 15.0, up_left_y - 25.0, up_left_z,
			up_left_x - 15.0, up_left_y - 25, up_right_z );
		glVertex3f( up_left_x - 15.0, up_left_y - 25.0, up_left_z );
	glEnd( );

	glPopMatrix( );
}


/* draws the left foot of the penguin */
/* now with normals */
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
		createNormal( back_left_x, back_left_y, back_left_z,
			back_left_x, back_left_y - 10.0, back_left_z,
			front_left_x, front_left_y, front_left_z );
		glVertex3f( back_left_x, back_left_y, back_left_z );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( front_left_x, front_left_y, front_left_z );
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );

		createNormal( front_right_x, front_right_y, front_right_z,
			front_left_x, front_left_y - 7.0, front_left_z,
			front_right_x, front_right_y - 7.0, front_right_z );
		glVertex3f( front_right_x, front_right_y, front_right_z );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );

		createNormal( back_right_x, back_right_y, back_right_z,
			front_right_x, front_right_y - 7.0, front_right_z,
			back_right_x, back_right_y - 10.0, back_right_z );
		glVertex3f( back_right_x, back_right_y, back_right_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );

		createNormal( back_left_x, back_left_y, back_left_z,
			back_right_x, back_right_y - 10.0, back_right_z,
			back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( back_left_x, back_left_y, back_left_z );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
	glEnd( );

	/* now we draw 2 triangles for the sides of his foot */
	glBegin( GL_TRIANGLES );
		/* left side */
		createNormal( front_left_x, back_left_y - 10.0, back_left_z - 20.0,
			front_left_x, front_left_y - 7.0, front_left_z,
			back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );

		/* right side */
		createNormal( front_right_x, back_right_y - 10.0, back_right_z - 20.0,
			back_right_x, back_right_y - 10.0, back_right_z,
			front_right_x, front_right_y - 7.0, front_right_z );
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );
	glEnd( );

	/* now we draw 2 quads to fill in the gaps */
	glBegin( GL_QUADS );
		/* top of foot */
		createNormal( front_left_x, back_left_y - 10.0, back_left_z - 20.0,
			front_right_x, back_right_y - 10.0, back_right_z - 20.0,
			front_left_x, front_left_y - 7.0, front_left_z );
		glVertex3f( front_left_x, front_left_y - 7.0, front_left_z );
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_right_x, front_right_y - 7.0, front_right_z );

		/* bottom of foot */
		createNormal( back_right_x, back_right_y - 10.0, back_right_z,
			front_right_x, back_right_y - 10.0, back_right_z - 20.0,
			back_left_x, back_left_y - 10.0, back_left_z );			
		glVertex3f( back_left_x, back_left_y - 10.0, back_left_z );
		glVertex3f( back_right_x, back_right_y - 10.0, back_right_z );
		glVertex3f( front_right_x, back_right_y - 10.0, back_right_z - 20.0 );
		glVertex3f( front_left_x, back_left_y - 10.0, back_left_z - 20.0 );
	glEnd( );

	glPopMatrix( );
}


/* draws a penguin centered (roughly) at the origin
   Also moves him around in a circle
   It now takes a parameter that indicates whether or
   no the penguin is a shadow (just for coloring purposes) */
void drawPenguin( int is_shadow )
{
	/* the colors we apply to the bird */
	/* Black does not really show lighting very well,
	   (or at all!) so the bird is now a blusih colour */
	GLfloat bluish[3];
	GLfloat orange[3];

	if( is_shadow )
	{
		/* bluish and orange are both grey for a shadow */
		bluish[0] = bluish[1] = bluish[2] = 0.2;
		orange[0] = orange[1] = orange[2] = 0.2;
	}
	else
	{
		/* for normal penguin, normal colors */
		bluish[0] = 0.08;
		bluish[1] = 0.2;
		bluish[2] = 0.35;

		orange[0] = 1.0;
		orange[1] = 0.5;
		orange[2] = 0.0;
	}

	/* draw main body and head bluish */
	glColor3fv( bluish );

	glPushMatrix( );
		/* Wobble the body a little bit */
		glRotatef( wobble_amount, 0.0, 0.0, 1.0 );
		drawHead( );
		drawBody( );

		/* draw the beak orange */
		glColor3fv( orange );
		drawBeak( );
	glPopMatrix( );

	/* draw penguin's left wing bluish */
	glColor3fv( bluish );
	drawLeftWing( );

	/* draw the left foot orange */
	glColor3fv( orange );
	drawLeftFoot( );

	/* to draw the penguins' right appendages, we just invert the world
	   along the X axis, and draw the left ones over again... */
	glScalef( -1.0, 1.0, 1.0 );

	glColor3fv( bluish );		/* bluish wing */
	drawLeftWing( );
	
	/* orange */
	glColor3fv( orange );
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

	/* Move out so that the penguin is in the frustum! */
	glTranslatef( 0, -50, -300.0 );

	/* Rotate the scene so that we can see the shadow */
	glRotatef( 15.0, 1.0, 0.2, 0.0 );

	GLfloat pos[4];		/* We must set up the light position here because it is translated */
	pos[0] = LIGHT_X;
	pos[1] = LIGHT_Y;
	pos[2] = LIGHT_Z;
	pos[3] = 1.0;
	glLightfv( GL_LIGHT0, GL_POSITION, pos );

	glPushMatrix( );
		/* transoformations that apply to whole bird */
		glTranslatef( penguin_x, 0.0, penguin_z );		/* move the penguin to his point on the orbit */
		glRotatef( penguin_heading, 0.0, 1.0, 0.0 );	/* face him the right way */

		/* Now we draw the penguin himself.
		   we must shift him up because drawPenguin
		   draws a penguin at the origin...we need him standing on Y=0 */
		glPushMatrix( );
			glTranslatef( 0.0, 30.0, 0.0 );
			glEnable( GL_LIGHTING );	/* Turn on lighting */
			drawPenguin( 0 );

			/* Motion blur time! */
#if MOTION_BLUR
			float q = .40;
			glAccum( GL_MULT, q );
			glAccum( GL_ACCUM, 1-q );
			glAccum( GL_RETURN, 1.0 );
#endif

		glPopMatrix( );			/* Undo the shift up operation */

		/* Now we have to render the shadow! */
		glTranslatef( LIGHT_X, LIGHT_Y, LIGHT_Z );		/* Move light back to its position */
		applyShadowMatrix( );							/* Multiply the current matrix by shadow matrix */
		glTranslatef( -LIGHT_X, -LIGHT_Y, -LIGHT_Z  );	/* move light to origin */
		glDisable( GL_LIGHTING );						/* turn lighing off */
		drawPenguin( 1 );								/* redraw the penguin as his shadow */
	glPopMatrix( );	/* Undo penguin-specific transformations */

	/* Draw the ground surface */
	glColor3f( 1.0, 1.0, 1.0 );
	glBindTexture( GL_TEXTURE_2D, ground_handle );
	glBegin( GL_QUADS );
		// The ground is drawn at negative to eliminate z-fighting with the shadow
		glTexCoord2f( 0.0, 1.0 );	glVertex3f( -200.0, -2.0, -100.0 );
		glTexCoord2f( 1.0, 1.0 );	glVertex3f( 220.0, -2.0, -100.0 );
		glTexCoord2f( 1.0, 0.0 );	glVertex3f( 220.0, -2.0, 200.0 );
		glTexCoord2f( 0.0, 0.0 );	glVertex3f( -200.0, -2.0, 200.0 );
	glEnd( );

	/* Lastly, we draw the light as a yellow sphere */
	glTranslatef( LIGHT_X, LIGHT_Y, LIGHT_Z );
	glColor3f( 1.0, 1.0, 0.0 );
	glutSolidSphere( 7.0, 12, 12 );

	/* Now draw the background image and snowflakes - which are positioned absolutely,
	   so we need to  reset to the identity matrix */
	glLoadIdentity( );
	
	/* Draw the background image */
	glColor3f( 0.8, 0.8, 0.8 );	/* We dull it a bit so we can see the flakes */
	glDisable( GL_FOG );		/* Turn off fog (It will totally grey out background) */
	glBindTexture( GL_TEXTURE_2D, bg_handle );
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0, 1.0 );	glVertex3f( -200.0, -200.0, -400.0 );
		glTexCoord2f( 1.0, 1.0 );	glVertex3f( 200.0, -200.0, -400.0 );
		glTexCoord2f( 1.0, 0.0 );	glVertex3f( 200.0, 200.0, -400.0 );
		glTexCoord2f( 0.0, 0.0 );	glVertex3f( -200.0, 200.0, -400.0 );
	glEnd( );
	glEnable( GL_FOG );		/* Turn fog back on */

	/* Draw the snow flakes */
	drawSnowFlakes( );

	/* flush the buffer and swap */
	glFlush( );
	glutSwapBuffers( );
}

/* Updates the postion of the snow flakes */
void updateFlakes( void )
{
	for( int i=0; i<NUM_FLAKES; i++ )
	{
		/* update position */
		flakes[i].x += flakes[i].dx * SPEED;
		flakes[i].y += flakes[i].dy * SPEED;

		/* If a flake falls past the bottom, re-initialize it */
		if( flakes[i].y < -50 )
		{
			flakes[i].y += 100;
			flakes[i].x = rand( ) % 50;
			if( rand( ) % 2 )
				flakes[i].x *= -1;
		}
	}
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

	updateFlakes( );

	/* ask for a redraw */
	glutPostRedisplay( );
}


/* Creates an OpenGL window  */
void createWindow( int w, int h, const char* title )
{
	/* Turn on the depth buffer and double buffering */
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( w, h ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( title );

	/* set the clear color to be white */
	glClearColor( 1.0, 1.0, 1.0, 0.0 );

	/* Enable hidden surface removal */
	glEnable( GL_DEPTH_TEST );
}


/* Sets up the light */
void setupLight( void )
{
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	glShadeModel( GL_SMOOTH );
	
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
	
	GLfloat amb [] = {0.7, 0.7, 0.7, 1.0};
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb );

	/* Set up the bird's materials */
	GLfloat spec [] = {0.3, 0.3, 0.3, 1.0};	/* mostly dull specular (feathers!) */
	GLfloat diff [] = {0.4, 0.4, 0.4, 1.0};	/* mormal looking ambient/diffuse */
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spec );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diff );
}



/* Keyboard handler */
void keyHandler( unsigned char key, int x, int y )
{
	if( key == 27 )
		exit( 0 );	/* escape key */
}



/* Our reshape callback */
void reshape( int w, int h )
{
	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glFrustum( -5, 5, -5, 5, 10, 1000 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}

/* loads the textures */
void setupTextures( void )
{
	/* the ground texture I made */
	ground_handle = loadTexture( "ground.raw", 64, 64 );

	/* the background I took from  http://www.uweb.ucsb.edu/~ryan_kinkade/index_files/Page939.htm */
	bg_handle = loadTexture( "bg.raw", 256, 256 );

}

/* We also use fog on the penguin to make him lose color
   as he walks further away from the camera */
void setupFog( void )
{
	glEnable( GL_FOG );
	GLfloat fog_color [] = {0.2, 0.2, 0.25, 1.0};
	glFogfv( GL_FOG_COLOR, fog_color );

	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_START, 200.0 );
	glFogf( GL_FOG_END, 400.0 );
}

/* sets the snopw flakes inital position & velocity */
void setupSnow( void )
{
	for( int i=0; i<NUM_FLAKES; i++ )
	{
		flakes[i].x = rand( ) % 50;

		/* make ~half of them negative */
		if( rand( ) % 2 )
			flakes[i].x *= -1;

		flakes[i].y = rand( ) % 50;
		if( rand( ) % 2 )
			flakes[i].y *= -1;

		flakes[i].dx = (rand( ) % 100) / 40000.0;
		if( rand( ) % 2 )
			flakes[i].dx *= -1;

		/* Y velocity should alwyas be negative */
		flakes[i].dy = (rand( ) % 100) / -10000.0;
		flakes[i].dy -= .002;
	}

}

/* main */
int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	createWindow( 800, 800, "Penguin!" );
	setupLight( );
	setupTextures( );
	setupSnow( );
	setupFog( );
	glAccum( GL_LOAD, 1.0 );
	
	/* register our call backs */
	glutDisplayFunc( &display ); 
	glutReshapeFunc( &reshape ); 
	glutIdleFunc( &animate );
	glutKeyboardFunc( &keyHandler );

	/* run the program */
	glutMainLoop( );

	return 0;
}




