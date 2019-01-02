#include <iostream>
#include "Pong.h"
#include <string>
#undef main

using namespace std;

void intro(string& in);
void sightsIntro();

int main() {
	string input;
	bool playing = 1;
	bool looking = 0;
	intro(input);

	if (input == "sightseeing") {
		looking = 1;
		sightsIntro();
	}
	Pong go(looking);
	while (playing == 1) {
		while (go.play());
		bool champ = go.Winner();
		if (go.forcequit != 1) {
			if (champ == 1) {
				cout << "Player 2 Wins" << endl;
			}
			else {
				cout << "Player 1 Wins" << endl;
			}
			cout << "Play Again? Y/N" << endl;
			input = "";
			while (input != "Y" && input != "N") {
				getline(cin, input);
				if (input == "Y") {
					go.Reset();
				}
				if (input == "N") {
					playing = 0;
				}
			}
		}
		else {
			playing = 0;
		}
	}

}

void intro(string& in) {
	cout << "Welcome to 3D Pong, controls for player 1 are W and S and controls for Player 2 are UP and DOWN arrows." << endl;
	cout << "To boost paddle speed during game:" << endl;
	cout << "Player 1: Hold Left Shift" << endl;
	cout << "Player 2: Hold Numpad 0" << endl;
	cout << "Press Enter to start or type \"sightseeing\" to just move around" << endl;
	cout << "You may press Escape at any time during the match to close the program" << endl;
	getline(cin, in);
}

void sightsIntro() {
	string nothing;
	cout << "Note: In sightseeing mode you can also move the camera" << endl;
	cout << "Camera Controls: " << endl;
	cout << "Move Up: Numperpad 8" << endl;
	cout << "Move Down: Numperpad 2" << endl;
	cout << "Move Left: Numperpad 4" << endl;
	cout << "Move Right: Numperpad 6" << endl;
	cout << "Move Forward: Numperpad +" << endl;
	cout << "Move Back: Numberpad -" << endl;
	cout << "Rotate Left: Numberpad 7" << endl;
	cout << "Rotate Right: Numberpad 9" << endl << endl;
	cout << "Press Enter when you're ready" << endl;
	getline(cin, nothing);
}