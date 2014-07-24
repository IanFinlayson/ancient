/********************************************************************
*	SimSoundPlayer.cpp			 Ian Finlayson 2005					*
*																	*
*	Implementation of Sim::SoundPlayer								*
*																	*
********************************************************************/

#include "SimSoundPlayer.h"
#include "SimException.h"

Sim::SoundPlayer::SoundPlayer( int numSounds, int numMusics )
{
	Mix_OpenAudio( AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS );

	num_musics = numMusics;
	num_sounds = numSounds;

	musics = new Mix_Music*[ num_musics ];
	sounds = new Mix_Chunk*[ num_sounds ];

	music_playing = false;

	SetMusicVolume( 127 );
	SetSoundVolume( 127 );
}

Sim::SoundPlayer::~SoundPlayer( )
{
	if( music_playing )
	{
		Mix_HaltMusic( );
	}

	for( int i=0; i<num_sounds; i++ )
	{
		Mix_FreeChunk( sounds[i] );
	}

	for( int i=0; i<num_musics; i++ )
	{
		Mix_FreeMusic( musics[i] );
	}

	delete [] sounds;
	delete [] musics;

	Mix_HaltChannel( -1 );

	Mix_CloseAudio( );
}

int Sim::SoundPlayer::AddMusic( char* file_name )
{
	static int curr_music = -1;

	curr_music++;

	musics[ curr_music ] = Mix_LoadMUS( file_name );

	if( musics[ curr_music ] == NULL )
	{
		throw Sim::Exception( "AddMusic", "Music can't load" );
	}

    return curr_music;
}

int Sim::SoundPlayer::AddSound( char* file_name )
{
	static int curr_sound = -1;

	curr_sound++;

	sounds[ curr_sound ] = Mix_LoadWAV( file_name );

	if( sounds[ curr_sound ] == NULL )
	{
		throw Sim::Exception( "AddSound", "Sound could not load" );
	}

	return curr_sound;
}

void Sim::SoundPlayer::PlayMusic( int music_index, int times )
{
	if( music_playing )
	{
		Mix_HaltMusic( );
	}

	Mix_PlayMusic( musics[ music_index ], times );
}

void Sim::SoundPlayer::PlaySound( int sound_index, int loops )
{
	Mix_PlayChannel( -1, sounds[ sound_index ], loops );
}

void Sim::SoundPlayer::StopMusic( )
{
	if( music_playing )
	{
		Mix_HaltMusic( );

		music_playing = false;
	}
}

void Sim::SoundPlayer::SetMusicPause( bool pause )
{
	if( pause )
	{
		Mix_PauseMusic( );
	}
	else
	{
		Mix_ResumeMusic( );
	}
}

void Sim::SoundPlayer::SetMusicVolume( int music_vol )
{
	Mix_VolumeMusic( music_vol );
}

void Sim::SoundPlayer::SetSoundVolume( int sound_vol )
{
	Mix_Volume( -1, sound_vol );
}