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



unsigned int Game_Field::get_cells_count() const
{
    return cells_count;
}

Pawn* Game_Field::get_pawn_from_cell(unsigned int _row, unsigned int _column)
{
    ASSERT(_row >= cells_count || _column >= cells_count || cells == nullptr);

    Pawn* pawn = cells[_row][_column].pawn_in_cell;
    cells[_row][_column].pawn_in_cell = nullptr;

    return pawn;
}

bool Game_Field::put_pawn_into_cell(unsigned int _row, unsigned int _column, Pawn *_pawn)
{
    ASSERT(_row >= cells_count || _column >= cells_count || cells == nullptr);

    if(cells[_row][_column].pawn_in_cell != nullptr) return false;

    cells[_row][_column].pawn_in_cell = _pawn;
    _pawn->set_cell_pos(cells[_row][_column].x, cells[_row][_column].y);
    _pawn->set_current_cell({_row, _column});
    return true;
}



const Pawn* Game_Field::peek_at_pawn_in_cell(unsigned int _row, unsigned int _column) const
{
    ASSERT(_row >= cells_count || _column >= cells_count || cells == nullptr);

    return cells[_row][_column].pawn_in_cell;
}

bool Game_Field::is_cell_empty(unsigned int _row, unsigned int _column) const
{
    ASSERT(_row >= cells_count || _column >= cells_count || cells == nullptr);

    return cells[_row][_column].pawn_in_cell == nullptr;
}



std::pair<int, int> Game_Field::get_closest_cell(float _x, float _y) const
{
    std::pair<int, int> result;
    float x = (_x - lu_cell_x) / cell_size;
    float y = (lu_cell_y - _y) / cell_size;

    int x_int = (int)x;
    int y_int = (int)y;

    result.first = ( (float)x_int + 0.5f > x ? x_int : x_int + 1 );
    result.second = ( (float)y_int + 0.5f > y ? y_int : y_int + 1 );

    if(result.first < 0 || result.first >= cells_count || result.second < 0 || result.second >= cells_count || x < -0.5f || y < -0.5f)
    {
        result.first = -1;
        result.second = -1;
    }

    return result;
}
