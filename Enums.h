#ifndef ENUMS_GLOBAL
#define ENUMS_GLOBAL

enum GhostType {
	BLINKY,
	INKY,
	PINKY,
	CLYDE
};


enum Orientation {
	UP,
	LEFT,
	DOWN,
	RIGHT,
};

enum GhostState {
	SCATTER,
	CHASE,
	FRIGHTENED,
	EATEN
};

enum LevelState {
	SCATTERPHASE,
	CHASEPHASE,
	FRIGHTENEDPHASE
};

enum FruitType {
	CHERRIES,
	STRAWBERRY,
	PEACH,
	APPLE,
	GRAPES,
	GALAXIAN,
	BELL,
	KEY
};


#endif // !ENUMS_GLOBAL
