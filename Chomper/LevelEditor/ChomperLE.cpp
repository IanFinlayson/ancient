// Chomper Level Editor
// No more hand manipulating text files!

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include <string>
#include <fstream>

#include <SimException.h>
#include <SimApp.h>
#include <SimImage.h>
#include <SimTextField.h>
#include <SimFont.h>
#include <SimTextWriter.h>
#include <SimButton.h>

#include "Level.h"

Sim::App app( 800, 600, false, "Chomper Level Editor", NULL );
Level level;
SDL_Event event;

Sim::Font ARIAL_12( "CHdata/arial.ttf", 12 );
Sim::TextField block_choice( 690, 10, 35, 20, 1, 0,0,0, ARIAL_12.font, 3, 230,230,230, 0,0,0, 3,0, "15", true );

Sim::TextField file_name( 680, 90, 95, 20, 1, 0,0,0, ARIAL_12.font, 30, 230,230,230, 0,0,0, 3,0, "", false );

Sim::Button undo( 610, 35, 180, 40, 1, 0,0,0, 70, 10 );

Sim::Button save( 610, 120, 60, 30, 1, 0,0,0, 10, 6 );

Sim::Button load( 680, 120, 60, 30, 1, 0,0,0, 10, 6 );

Sim::Button validate( 610, 250, 180, 40, 1, 0,0,0, 65, 10 );

int main( int argc, char** argv )
{
	if( argc > 1 ) //user passed an argument
	{
		level.Load( argv[1] );	//load the level passed
	}

	undo.SetBackGround( 230,230,230 );
	save.SetBackGround( 230,230,230 );
	load.SetBackGround( 230,230,230 );
	validate.SetBackGround( 230,230,230 );

	undo.SetForeGround( "UNDO", ARIAL_12.font, 0,0,0 );
	save.SetForeGround( "SAVE", ARIAL_12.font, 0,0,0 );
	load.SetForeGround( "LOAD", ARIAL_12.font, 0,0,0 );
	validate.SetForeGround( "VALIDATE", ARIAL_12.font, 0,0,0 );

	unsigned char undone = 200;
	int und_x, und_y;

	bool done = false;

	while( !done )
	{
		if( SDL_WaitEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				done = true;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				block_choice.HandleKeyDown( event.key.keysym );
				file_name.HandleKeyDown( event.key.keysym );

				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					done = true;
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				block_choice.HandleMouseDown( event.button.x, event.button.y );
				file_name.HandleMouseDown( event.button.x, event.button.y );

				undo.HandleMouseDown( event.button.x, event.button.y );
				save.HandleMouseDown( event.button.x, event.button.y );
				load.HandleMouseDown( event.button.x, event.button.y );
				validate.HandleMouseDown( event.button.x, event.button.y );

				if( event.button.x < 600 )
				{
					und_x = event.button.x / 40;
					und_y = event.button.y / 40;
					
					if( atoi( block_choice.GetText().c_str() ) < 16 )
					{
						undone = level.GetPiece( und_x, und_y );
						level.SetPiece( und_x, und_y, atoi( block_choice.GetText().c_str() ) );
					}
					else if( atoi( block_choice.GetText().c_str() ) < 21 )
					{
						undone = 200;
						level.SetBonus( und_x, und_y, atoi( block_choice.GetText().c_str() ) );
					}
					else if( atoi( block_choice.GetText().c_str() ) < 23 )
					{
						undone = 200;

						if( atoi( block_choice.GetText().c_str() ) == 21 )
						{
							level.SetChomperStart( und_x, und_y );
						}
						else
						{
							level.SetGhostysStart( und_x, und_y );
						}
					}
				}
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				block_choice.HandleMouseUp( event.button.x, event.button.y );
				file_name.HandleMouseUp( event.button.x, event.button.y );

				if( undo.HandleMouseUp( event.button.x, event.button.y ) )
				{
					//User did undo
					if( undone != 200 )
					{
						level.SetPiece( und_x, und_y, undone );
						undone = 200;
					}
				}
				if( save.HandleMouseUp( event.button.x, event.button.y ) )
				{
					std::string fileName;

					fileName = file_name.GetText( ) + ".chl";

					level.Save( fileName.c_str() );
				}
				if( load.HandleMouseUp( event.button.x, event.button.y ) )
				{
					std::string fileName;

					fileName = file_name.GetText( ) + ".chl";

					level.Load( fileName.c_str() );
				}
				if( validate.HandleMouseUp( event.button.x, event.button.y ) )
				{
					level.Validate( );
				}
			}
		}

		app.Clear( 255, 255, 255 );


		level.Draw( app.screen );

		Sim::TextWriter::WriteText( app.screen, "CHdata/arial.ttf", 12, "Tile type:", 610, 10, 0,0,0, 255,255,255 );
		block_choice.Draw( app.screen );

		Sim::TextWriter::WriteText( app.screen, "CHdata/arial.ttf", 12, "File name:", 610, 90, 0,0,0, 255,255,255 );
		file_name.Draw( app.screen );
		
		undo.Draw( app.screen );
		save.Draw( app.screen );
		load.Draw( app.screen );
		validate.Draw( app.screen );

		app.Flip( );
	}
	return 0;
}