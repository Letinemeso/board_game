#ifndef __GAME_CONTROLLER
#define __GAME_CONTROLLER

#include "Debug.h"

#include "Event_Controller.h"
#include "Resource_Loader.h"

#include "Game_Field.h"
#include "Pawn.h"


class Game_Controller
{
private:
    Pawn* held_pawn = nullptr;
    Pawn** pawns = nullptr;
    const unsigned int pawns_per_player = 9;

    Game_Field field;

    int current_player = 0;

public:
    Game_Controller(const char* _field_object, const char* _1_player_pawn_object, const char* _2_player_pawn_object);
    ~Game_Controller();

private:
    bool pawn_was_in_this_cell(const std::pair<int, int>& _cell, const Pawn* _pawn) const;
    bool cell_is_next_to_pawn(const std::pair<int, int>& _cell, const Pawn* _pawn) const;

public:
    void update();
    void draw();

};

#endif // __GAME_CONTROLLER
