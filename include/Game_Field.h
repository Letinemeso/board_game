#ifndef __GAME_FIELD
#define __GAME_FIELD

#include "Event_Controller.h"
#include "Resource_Loader.h"
#include "Object.h"
#include "Pawn.h"

#include "Debug.h"


class Game_Field : protected LEti::Object
{
private:
    struct Cell { float x = 0, y = 0; Pawn* pawn_in_cell = nullptr; };

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

public:
    unsigned int get_cells_count() const;
    Pawn* get_pawn_from_cell(unsigned int _row, unsigned int _column);
    bool put_pawn_into_cell(unsigned int _row, unsigned int _column, Pawn* _pawn);

    const Pawn* peek_at_pawn_in_cell(unsigned int _row, unsigned int _column) const;
    bool is_cell_empty(unsigned int _row, unsigned int _column) const;

    std::pair<int, int> get_closest_cell(float _x, float _y) const;

};

#endif
