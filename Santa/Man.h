// Man.h
// Ian Finlayson

// A man has several models which are cycled through
// to draw him
#ifndef MAN_H
#define MAN_H

// Forward declaration
class Model;

class Man
{
private:
	static const int NUM_FRAMES = 14;
	Model* frames[NUM_FRAMES];
	int current_frame;

	BmpTexture* texture;

public:
	Man( );
	~Man( );

	void Update( float elapsed_time );
	void Draw( );
};

#endif