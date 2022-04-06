#include "../include/Game_Field.h"

Game_Field::Game_Field() : LEti::Object()
{

}

Game_Field::~Game_Field()
{
	clear_cells();
}



void Game_Field::setup_cells()
{
	ASSERT(cells != nullptr);

	cells = new Cell * [cells_count];
	for (unsigned int i = 0; i < cells_count; ++i)
	{
		cells[i] = new Cell[cells_count];
		for (unsigned int j = 0; j < cells_count; ++j)
		{
			cells[i][j].x = lu_cell_x + cell_size * i;
			cells[i][j].y = lu_cell_y - cell_size * j;
		}
	}
}

void Game_Field::clear_cells()
{
	if (cells != nullptr)
	{
		for (unsigned int i = 0; i < cells_count; ++i)
			delete[] cells[i];
		delete cells;
	}
}



void Game_Field::init(const char* _object)
{
	LEti::Object::init(_object);

	const float* lu_cell_data = LEti::Resource_Loader::get_data<float>(_object, "lu_cell_pos").first;
	lu_cell_x = lu_cell_data[0];
	lu_cell_y = lu_cell_data[1];

	cell_size = LEti::Resource_Loader::get_data<float>(_object, "cell_size").first[0];
	ASSERT(cell_size < 0.0001f);

	cells_count = LEti::Resource_Loader::get_data<unsigned int>(_object, "cell_count").first[0];
	ASSERT(cells_count == 0);

	setup_cells();
}



void Game_Field::draw() const
{
	LEti::Object::draw();
}
