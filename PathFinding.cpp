#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "PathFinding.h"
#include "Utilities.h"

// Print the help information for playing the game and reading the game (path finding) results
void PathFinding::PrintHelpInfo()
{
	cout << "Welcome to the Path Finding game!" << endl;
	cout << "Instruction For Use:" << endl;
	cout << "1. Enter the battlefield map size between " << MIN_MAP_SIZE << " x " << MIN_MAP_SIZE << " and " << MAX_MAP_SIZE << " x " << MAX_MAP_SIZE << endl;
	cout << "2. Enter the number of battle units between " << MIN_NUM_UNITS << " and " << MAX_NUM_UNITS << endl;
	cout << "3. Enter the percentage of elevated terrain positions between " << MIN_ELEVATED_PERCENT << " and " << MAX_ELEVATED_PERCENT << endl;
	cout << "4. The found path for each battle unit will be displayed in the following ways:" << endl;
	cout << "   a.  S  - starting position" << endl;
	cout << "   b.  T  - target position" << endl;
	cout << "   c. 0-9 - battle unit moving steps in the path" << endl;
	cout << "   d.  w  - battle unit stays at one position for multiple steps to wait for other battle unit move out the next move position" << endl;
	cout << "   c.  ^  - elevated terrain" << endl;
	cout << "   d.  .  - ground terrain" << endl;
	cout << endl;
}

// Play one round of path finding game
void PathFinding::Play()
{
	// Get the game parameters from the user
	GetGameParameter();

	battlefield_map = CreateBattlefieldMap(map_size, map_size);

	// Choose the start and target positions for each batte unit randomly. Units may move towards a common target position, but must have different start positions.
	GenerateStartAndTargetPositions();

	// Find the path from the starting position to the target position for each battle unit
	vector<vector<Coordinate>> paths;
	for (int i = 0; i < num_units; ++i) {
		paths.emplace_back(FindPath(i, start_positions[i], target_positions[i]));
	}

	// Add waiting steps since at any given moment, each ground terrain position (except target positions) may be occupied by at most one battle unit
	AddWaitingStepsBecauseOfCollision(paths);

	// Print path finding results for each battle unit on battlefield map
	for (int i = 0; i < num_units; ++i) {
		PrintPathFindingResults(i, paths[i], start_positions[i], target_positions[i]);
	}
}

// Read inputs from users for controlling/manupilating one round of the game
void PathFinding::GetGameParameter()
{
	do {
		cout << endl << "Please enter battlefield map size between " << MIN_MAP_SIZE << " and " << MAX_MAP_SIZE << ": ";
		while (!(cin >> map_size)) { // Loop until valid integer is entered
			cout << "Invalid input. Please enter a number: ";
			cin.clear(); // Clear the error flags
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
		}
	} while (map_size < MIN_MAP_SIZE || map_size > MAX_MAP_SIZE);
	
	do {
		cout << endl << "Please enter number of attle units between " << MIN_NUM_UNITS << " and " << MAX_NUM_UNITS << ": ";
		while (!(cin >> num_units)) { // Loop until valid integer is entered
			cout << "Invalid input. Please enter a number: ";
			cin.clear(); // Clear the error flags
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
		}
		//cin >> num_units;
	} while (num_units < MIN_NUM_UNITS || num_units > MAX_NUM_UNITS);

	do {
		cout << endl << "Please enter the percentage of elevated terrain positions between " << MIN_ELEVATED_PERCENT << " and " << MAX_ELEVATED_PERCENT << ": ";
		while (!(cin >> elevated_terrain_rate)) { // Loop until valid integer is entered
			cout << "Invalid input. Please enter a number: ";
			cin.clear(); // Clear the error flags
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
		}
	} while (elevated_terrain_rate < MIN_ELEVATED_PERCENT || num_units > MAX_ELEVATED_PERCENT);

	return;
}

// Create the battlefield map for path finding game
vector<vector<unsigned char>> PathFinding::CreateBattlefieldMap(unsigned int num_rows, unsigned int num_cols)
{
	vector<vector<unsigned char>> battlefield_map(num_rows, vector<unsigned char>(num_cols, GROUND_TERRAIN));
	for (auto& r : battlefield_map) {
		for (auto& p : r) {
			if (IsElevatedTerrain()) { // generate elevated terrain randomly
				p = ELEVATED_TERRAIN;
			}
		}
	}
	return battlefield_map;
}


// Statistically select a position as Elevated Terrain
bool PathFinding::IsElevatedTerrain() {
	// Define the desired range (inclusive)
	int min_val = 0;
	int max_val = 100;

	// 1. Create a random_device to seed the generator
	//    random_device provides non-deterministic random numbers (from hardware or OS)
	std::random_device rd;

	// 2. Create a Mersenne Twister engine (a common and good pseudo-random number generator)
	//    Seed it with the random_device for better randomness
	std::mt19937 gen(rd());

	// 3. Create a uniform_int_distribution for the desired range
	//    This ensures numbers are distributed evenly within the range [min_val, max_val]
	std::uniform_int_distribution<> distrib(min_val, max_val);

	// 4. Generate the random number
	int random_number = distrib(gen);

	return random_number < elevated_terrain_rate;
}

// Generate the start positions and target positions for each battle unit on the battlefield map
void PathFinding::GenerateStartAndTargetPositions()
{
	auto row_size = static_cast<int>(size(battlefield_map));
	auto column_size = static_cast<int>(size(battlefield_map[0]));

	for (int i = 0; i < num_units; ++i) {
		// Choose a start position randomly. A valid start position is on Ground Terrain, and different from the start positions of the existing battle units
		auto s = Coordinate{ Utilities::NumberGenerator(0, row_size - 1), Utilities::NumberGenerator(0, column_size - 1) };
		while (battlefield_map[s.x][s.y] != GROUND_TERRAIN || find(begin(start_positions), end(start_positions), s) != start_positions.end())
		{
			s = Coordinate{ Utilities::NumberGenerator(0, row_size - 1), Utilities::NumberGenerator(0, column_size - 1) };
		}
		start_positions.emplace_back(s);

		// Choose a target position randomly. A valid target position is on Ground Terrain and different from the start position of the same battle unit
		auto t = Coordinate{ Utilities::NumberGenerator(0, row_size - 1), Utilities::NumberGenerator(0, column_size - 1) };
		while (battlefield_map[t.x][t.y] != GROUND_TERRAIN || t == s)
		{
			t = Coordinate{ Utilities::NumberGenerator(0, row_size - 1), Utilities::NumberGenerator(0, column_size - 1) };
		}
		target_positions.emplace_back(t);
	}

	return;
}

// Support multiple Units
// Find a list of positions for the battle unit to travel from its starting position to the target position
vector<Coordinate> PathFinding::FindPath(const unsigned char unit_id, const Coordinate start, const Coordinate target) {
	vector<Coordinate> path;
	FindPathHelper(unit_id, start, target, &path);
	return path;
}

// Perform Deep First Search (DFS) to find a feasible path ( a list of positions) from current position to target position on the battlefield map
bool PathFinding::FindPathHelper(const unsigned char unit_id, const Coordinate& cur, const Coordinate& target, vector<Coordinate>* path_ptr) {
	const unsigned char MyVisitedTerrain = 1 << unit_id;  // use this bit to mark visited positions, i.e., not visit the same position more than 1 time

	// Check cur is within battlefield map and is not a Elevated Terrain and not visited before.
	if (cur.x < 0 || cur.x >= size(battlefield_map) || cur.y < 0 || cur.y >= size(battlefield_map[cur.x]) || 
		battlefield_map[cur.x][cur.y] == ELEVATED_TERRAIN || battlefield_map[cur.x][cur.y] & MyVisitedTerrain) {
		return false;
	}

	auto& path = *path_ptr;
	path.emplace_back(cur);
	battlefield_map[cur.x][cur.y] |= MyVisitedTerrain;
	if (cur == target) {
		return true;
	}

	for (const Coordinate& next_move : { Coordinate{cur.x, cur.y + 1}, Coordinate{cur.x, cur.y - 1}, Coordinate{cur.x + 1, cur.y}, Coordinate{cur.x - 1, cur.y} }) {
		if (FindPathHelper(unit_id, next_move, target, path_ptr)) {
			return true;
		}
	}

	// Cannot find a path, remove the entry added in path.emplace_back(cur)
	path.pop_back();
	return false;
}

// For handling multiple units moving simultaneously, the following two rules are followed:
// 1. Units may move towards a common target posi􀆟on or to individual distinct target positions.
// 2. At any given moment, each ground terrain posi􀆟on may be occupied by at most one unit.
// To solve this position collision problem, if the next move position of a battle unit is currently occupied by other battle unit (except by another unit at its target position),
//  the unit will wait at its current position for one step
void PathFinding::AddWaitingStepsBecauseOfCollision(vector<vector<Coordinate>>& paths)
{
	int step = 1;								// start positions of different battle field units are already at different positions, start with 1 instead of 0
	int num_of_paths_to_process = num_units;
	while (num_of_paths_to_process > 1) {		// position collision can happen only when there are multiple battle unit paths
		for (int i = 0; i < num_units; ++i) {	// multiple units move simultaneously
			if (size(paths[i]) == 0)			// if a battle unit cannot find a path, its path will be empty, no position collision
			{
				--num_of_paths_to_process;
				continue;
			}

			if (step >= size(paths[i]) - 1) {	// done with the path process of this battle unit, don't check position collision at target position
				--num_of_paths_to_process;
				continue;
			}

			for (int j = 0; j < i; ++j) {
				if (size(paths[j]) == 0)			// if a battle unit j cannot find a path, its path will be empty, no position collision with this unit
				{
					continue;
				}
				if (step < size(paths[j]) - 1 && paths[i][step] == paths[j][step]) {	// check collision with another battle unit on occupying a ground terrain position, ignore target positions
					paths[i].insert(paths[i].begin() + step, paths[i][step]);			// stay at same position for waiting
				}
			}

			for (int j = i + 1; j < num_units; ++j) {
				if (size(paths[j]) == 0)			// if a battle unit j cannot find a path, its path will be empty, no position collision with this unit
				{
					continue;
				}
				if (step < size(paths[j]) && paths[i][step] == paths[j][(step - 1)]) {	// check collision with another battle unit on occupying a ground terrain position, ignore target positions
					paths[i].insert(paths[i].begin() + step, paths[i][step]);			// stay at same position for waiting
				}
			}
		}
		++step;
	}
}

// Print the found path for each battle unit on the battlefield map
void PathFinding::PrintPathFindingResults(unsigned char unit_id, const vector<Coordinate>& path, Coordinate start, Coordinate target) 
{
	vector<vector<char>> printable_map(size(battlefield_map), vector<char>(size(battlefield_map[0]), '.'));	// initialize with Ground Terrain
	for (int i = 0; i < size(battlefield_map); ++i) {
		for (int j = 0; j < size(battlefield_map[0]); ++j) {
			if (battlefield_map[i][j] == ELEVATED_TERRAIN) {
				printable_map[i][j] = '^';
			}
		}
	}

	// Add the battle unit path overlay on the battlefield map
	auto step_count = 0;
	Coordinate previous_position{ -1, -1 };		// an invalid position, so cannot be same as any position in the battle unit path
	for (auto& p : path) {
		if (p == previous_position) {
			printable_map[p.x][p.y] = 'w';		// the unit stay at same position for multiple steps
		}
		else {
			printable_map[p.x][p.y] = step_count + '0';	// use 0 - 9 to show the step sequence in the battle unit path
		}
		step_count = (++step_count) % 10;
		previous_position = p;
	}

	// auto s = path.front(), t = path.back();
	printable_map[start.x][start.y] = 'S';
	printable_map[target.x][target.y] = 'T';

	if (size(path) == 0) {		// when a battle unit cannot find a path from start position to target position, the path is empty (0 step) 
		cout << endl << "Unit " << static_cast<int>(unit_id) << " Path: " << "Start (" << start.x << "," << start.y << "); Target (" << target.x << "," << target.y << "); Path Not Found!" << endl;
	}
	else {
		cout << endl << "Unit " << static_cast<int>(unit_id) << " Path: " << "Start (" << start.x << "," << start.y << "); Target (" << target.x << "," << target.y << "); Number of Steps (including waiting steps): " << size(path) - 1 << endl;
	}
	for (auto& r : printable_map) {
		for (auto& p : r) {
			cout << p << " ";
		}
		cout << endl;
	}
}
