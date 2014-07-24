// Implements a HighList	-	Copyright Ian Finlayson 2005

#include "HighList.h"
#include <sstream>
#include <fstream>
#include <SDL/SDL_ttf.h>

//I am sure there is a better solution than repeating this function
//(it is in CrazyBlocks.cpp) - but this was the obvious thing...
void PrintText( const char*, const char*, int, int, int, SDL_Color, SDL_Color, SDL_Surface** );

HighList::HighList()
{
	head = NULL;
}

void HighList::Load()
{
	std::fstream inFile;
	head = NULL;		//must do it again because we can load the list a few times...
	std::string Name;
	int Score;

	inFile.open( "CBdata/scores.dat", std::ios::in );

	inFile >> Name;
	inFile >> Score;

	while( !inFile.eof() )
	{
		Add( Name, Score );
		
		inFile >> Name;
		inFile >> Score;		
	}

	inFile.close();
}

void HighList::Add( std::string Name, int Score )
{
	Node* newNode = new( Node );
	newNode->name = Name;
	newNode->score = Score;

	//Find Predasessor:
	Node* curr = head;
	Node* prev = NULL;
	while( (curr != NULL) && ( curr->score > Score ) )
	{
		prev = curr;
		curr = curr->next;
	}

	if( prev == NULL )	//adding to top
	{
		newNode->next = head;
		head = newNode;
	}
	else
	{
		newNode->next = prev->next;
		prev->next = newNode;
	}
}

void HighList::Display( SDL_Surface** screen )	//This Method Displays our List for the user when they type enter, return
{
	SDL_Event ourEvent;
	bool Return = false;

	SDL_Color red, white;
	red.r = 195;
	red.b = red.g = 0;

	white.b = white.g = white.r = 255;

	SDL_FillRect( *screen, NULL, SDL_MapRGB( (*screen)->format, 255, 255, 255 ) );	//Fill to White

	PrintText( "CBdata/adlib.ttf", "High Scores", 15, 15, 20, red, white, screen );

	Node* curr = head;
	for( int i=0; ((i<10) && (curr!=NULL)); i++ )	//Only show top 10
	{
		std::stringstream num, score;

		num << (i+1) << ": ";
		score << curr->score;

		PrintText( "CBdata/arial.ttf", (const char*)num.str().c_str(),     8, 40+(19*i), 15, red, white, screen );
		PrintText( "CBdata/arial.ttf", (const char*)(curr->name).c_str(), 30, 40+(19*i), 15, red, white, screen );
		PrintText( "CBdata/arial.ttf", (const char*)score.str().c_str(), 185, 40+(19*i), 15, red, white, screen ); 

		curr = curr->next;
	}

	PrintText( "CBdata/arial.ttf", "Press Enter to Continue or Escape to Quit", 12, 320, 12, red, white, screen );

	SDL_Flip( *screen );

	while( !Return )
	{
		if( SDL_PollEvent( &ourEvent ) )
		{
			if( ( ourEvent.type == SDL_KEYDOWN ) )
			{
				if( ourEvent.key.keysym.sym == SDLK_RETURN )
				{
					Return = true;
				}
				else if( ourEvent.key.keysym.sym == SDLK_ESCAPE )
				{
					Save();
					exit( 0 );
				}
			}
		}
	}
}

void HighList::DeleteList()
{
	head = NULL;	//Destroy our List - decapitation :)

	std::fstream blah;	//Blank out the data file
	blah.open( "CBdata/scores.dat", std::ios::out );

	blah.close();
}

void HighList::Save()//Overwrite the data file
{
	std::fstream outFile;
	outFile.open( "CBdata/scores.dat", std::ios::out );

	int i=0;
	Node* curr = head;

	while( ( i < 10 ) && ( curr != NULL ) )	//Only Save Top Ten
	{
		outFile << curr->name << " " << curr->score << std::endl;

		i++;
		curr = curr->next;
	}

	outFile << " \n";

	outFile.close();
}

void PrintText( const char* fontName, const char* text, int x, int y, int size, SDL_Color fore, SDL_Color back, SDL_Surface** screen )
{
	TTF_Font* theFont = NULL;
	theFont = TTF_OpenFont( fontName, size );


	SDL_Surface* temp;
	temp = TTF_RenderText_Shaded( theFont, text, fore, back );

	SDL_Rect desti;
	desti.h = 0;
	desti.w = 0;
	desti.x = x;
	desti.y = y;
	
	SDL_BlitSurface( temp, NULL, *screen, &desti );

	SDL_FreeSurface( temp );

	TTF_CloseFont( theFont );
}

