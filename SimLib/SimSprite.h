/********************************************************************
*	SimSprite.h					Ian Finlayson 2005					*
*																	*
*	This class provides for the easy creation and use of Sprites	*
*																	*
********************************************************************/

#ifndef SIM_SPRITE
#define SIM_SPRITE

#include <SDL/SDL.h>

namespace Sim
{
	class Sprite
	{
	private:

		class Animation
		{
		private:
			SDL_Rect* frames;
			int num_frames;
			int curr_frame;
			int default_frame;

			SDL_Surface* image;
			unsigned int ms_tween_frames;
			unsigned int timer;

			bool motionly;
			bool forward;

			void UpdateCircle( );
			void UpdateMotion( );

		public:
			Animation( int x, int y, int w, int h, int numFrames, bool left_to_right, SDL_Surface* img, unsigned int msTweenFlips, bool motion, int default_frame );
			~Animation( );

			void Draw( SDL_Surface* screen, SDL_Rect* dest );
			void Update( );
			
			int GetNumFrames( ){ return num_frames; }
			void SetFrame( int frame ){ curr_frame = frame; }
			void SetToDefault( ){ curr_frame = default_frame; }
		};//class Animation

		Animation** animations;
		int num_animations;
		int curr_animation;

		unsigned int start_time, end_time;
		bool paused;

		float X, Y, dX, dY;		// dX and dY measured in pixels/second
		SDL_Rect position;

	public:

		Sprite( int x, int y, int w, int h, int numAnimations );
		~Sprite( );

		void AddAnimation( int x, int y, int numFrames, bool left_to_right, SDL_Surface* img, unsigned int msTweenFlips, bool motion, int default_frame=0 );
		void SetAnimation( int anim );
		int GetAnimation( ){ return curr_animation; }

		void Update( );
		void Draw( SDL_Surface* screen );

		void SetDx( float dx ){ dX=dx; }
		float GetDx( ){ return dX; }
		void SetDy( float dy ){ dY=dy; }
		float GetDy( ){ return dY; }
		void SetMovement( float dx, float dy ){ dX=dx; dY=dy; }

		SDL_Rect GetPosition( ){ return position; }
		void SetPosition( int x, int y );

		void SetPause( bool pause );

	};//class Sprite
}//Sim namespace

#endif
