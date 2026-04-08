#include "Rectangle.h"
#include "Pet.h"
#include "Music.h"
using namespace std;

int main() {
	Rectangle rect1(4, 6);
	cout << "Rectangle 1 :" << endl;
	//printf("Rectangle 1 : \n");
	cout << "Area :" << rect1.getArea() << endl;
	//printf("Area : %lf\n", rect1.getArea());
	cout << "perimeter :" << rect1.getPerimeter() << endl;
	cout << "Is Spuare? " << boolalpha << rect1.isSquare() << endl;

	printf("==================================================\n");

	Pet myPet("Lucky", 5, "dog");
	cout << "My pet's name is " << myPet.getName() << endl;
	cout << "My pet's age is " << myPet.getAge() << endl;
	Puppy myPuppy("Kong", 3, "dog", "Puddle");
	cout << "My pet's breed is " << myPuppy.getBreed() << endl;

	printf("==================================================\n");

	// Create a music streaming service
	MusicStreamingService myService("Youtube Music");
	// Add some music to the service
	myService.addMusic("Shape of You", "Ed Sheeran", "Divide", 2017);
	myService.addMusic("Swim", "BTS", "ARIRANG", 2026);
	myService.addMusic("Blinding Lights", "The Weeknd", "After Hours", 2019);
	myService.addMusic("Dynamite", "BTS", "BE", 2020);

	// Search for a music by title
	string music_title;
	cout << "Enter the Music Title";
	cin >> music_title;
	Music* result = myService.searchByTitle(music_title);
	if (result != NULL) {
		cout << "Music found: " << result->getTitle() << " by " << result->getArtist() << endl;
	} 
	else {
		cout << "Music not found." << endl;
	}

	// Search for music by artist
	string artist_name;
	cout << "Enter the Artist Name: ";
	cin >> artist_name;

	vector<Music*> artistResult = myService.searchByArtist(artist_name);
	if (artistResult.size() > 0) {
		cout << "Found " << artistResult.size() << " songs by " << artist_name << ":" << endl;
		for (int i = 0; i < artistResult.size(); i++) {
			cout << artistResult[i]->getTitle() << endl;
			}
	}
	else {
		cout << "No music by " << endl;
	}

	return 0;
}