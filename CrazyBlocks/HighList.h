//A linked list used for the high scores	-	Copyright Ian Finlayson 2005
//When I first wrote this class, I used
//char* instead of std::string (I'm not real fond of STL)
//Of course the fact that I wrote my own Linked List class
//for somthing so sipmle already told you that :)
//I changed to string because I kept getting segmentation
//faults wit char*. I guess STL has its place after all
// - though I still hate the ugly error messages templates cause!

#include <SDL/SDL.h>
#include <string>

class HighList
{
private:
	struct Node
	{
		std::string name;
		int  score;
		Node* next;
	};

	Node* head;
public:
	HighList();
	void Load();
	void Add( std::string, int );
	void DeleteList();
	void Display( SDL_Surface** screen );
	void Save();
};
