// SkyBox.h
// Ian Finlayson

// A sky box is a small cube drawn around the camera with the insides
// textured with a landscape scene.  This gives the illusion of a vast
// outdoor place.  Wheras, in reality, the camera is in a tiny box.

#ifndef SKY_BOX
#define SKY_BOX

class SkyBox
{
private:
	BmpTexture** textures;

public:
	// 1 is bottom (the ground) and 6 is top (the sky) the rest are the sides...
	SkyBox( const char* f1, const char* f2, const char* f3, const char* f4, const char* f5, const char* f6 );
	~SkyBox( );

	void Draw( );
};

#endif