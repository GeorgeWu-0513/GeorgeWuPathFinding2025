#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>

using namespace std;

struct Coordinate {
	bool operator==(const Coordinate& that) const {
		return x == that.x && y == that.y;
	}
	int x, y;
};

// Define a class named 'PathFinding' to perform DFS to find a feasible path
class PathFinding {
public:
	PathFinding():
		elevated_terrain_rate(0),
		map_size(8),
		num_units(1)
	{}

	void PrintHelpInfo();
	void Play();

private:
	void GetGameParameter();
	vector<vector<unsigned char>> CreateBattlefieldMap(unsigned int num_rows, unsigned int num_cols);
	bool IsElevatedTerrain();
	void GenerateStartAndTargetPositions();
	vector<Coordinate> FindPath(const unsigned char unit_id, const Coordinate start, const Coordinate target);
	bool FindPathHelper(const unsigned char unit_id, const Coordinate& cur, const Coordinate& target, vector<Coordinate>* path_ptr);
	void AddWaitingStepsBecauseOfCollision(vector<vector<Coordinate>>& paths);
	void PrintPathFindingResults(unsigned char unit_id, const vector<Coordinate>& path, Coordinate start, Coordinate target);

	vector<vector<unsigned char>> battlefield_map;
	int map_size;
	int elevated_terrain_rate;						// percentage of positions are elevated terrain, rest is gound terrain
	int num_units;									// number of battle units
	vector<Coordinate> start_positions;
	vector<Coordinate> target_positions;

	const static unsigned char ELEVATED_TERRAIN = numeric_limits<unsigned char>::max();
	const static unsigned char GROUND_TERRAIN = 0;
	const static int MIN_MAP_SIZE = 7;
	const static int MAX_MAP_SIZE = 64;
	const static int MIN_NUM_UNITS = 1;
	const static int MAX_NUM_UNITS = 4;
	const static int MIN_ELEVATED_PERCENT = 0;
	const static int MAX_ELEVATED_PERCENT = 80;
};
