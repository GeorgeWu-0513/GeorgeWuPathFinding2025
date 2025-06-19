Objective:
Implement a path-finding algorithm for batle units in a Real-Time Strategy (RTS) game. This project allows the user to run path-finding algorithm for multiple battle units for moving from a randomly generated start position to a randomly generated target position.

Input to the Algorithm:
1. The size of a Batlefield Map:
o A binary map of at least 32 x 32 fields, where each field represents a position on the batlefield.
o Each position is either:
  Ground Terrain: Occupiable by a single batle unit.
  Elevated Terrain: Unreachable by batle units.
2. Number of Batle Units:
o Both the starting position and target position of a Battle Unit is randomly generated. Multiple Battle Units may have same target position but mush have different starting positions
3. Percentage of fields that are Elevated Terrain on the battlefiled map.

Output of the Algorithm:
• A list of positions for the batle unit to travel from its starting position to the target position on the battlefield map.
