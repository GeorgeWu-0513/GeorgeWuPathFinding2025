// GeorgeWuPathFinding.cpp : Defines the entry point for the application.
//

#include "GeorgeWuPathFinding.h"

using namespace std;

int main()
{
    // Create an object (instance) of the PathFinding class
    auto path_finding = make_unique<PathFinding>();

    // Display game help information
    path_finding->PrintHelpInfo();

    auto game_number = 0;
    while (true) {
        cout << endl << "GAME <<" << ++game_number << ">>";

        // Play one round of the path finding game
        path_finding->Play();

        // Game exit control: quit or continue playing
        cout << endl << "Enter Q or q to quit this game, any other key to continue playing: ";
        char want_quit;
        cin >> want_quit; // Read a single character from stdin
        if (want_quit == 'Q' || want_quit == 'q') {
            break;
        }

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}
