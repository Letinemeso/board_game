#ifndef __GAME_FIELD
#define __GAME_FIELD

#include "Event_Controller.h"
#include "Resource_Loader.h"
#include "Object.h"

#include "Debug.h"


class Game_Field : protected LEti::Object
{
private:
	struct Cell { float x = 0, y = 0; bool occupied = false; };

private:
	float lu_cell_x = 0.0f, lu_cell_y = 0.0f;
	float cell_size = 0.0f;
	unsigned int cells_count = 0;
	Cell** cells = nullptr;

public:
	Game_Field();
	~Game_Field() override;

private:
	void setup_cells();
	void clear_cells();

public:
	void init(const char* _object) override;

public:
	void draw() const override;

};

#endif