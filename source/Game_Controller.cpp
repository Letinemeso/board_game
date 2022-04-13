#include "../include/Game_Controller.h"


Game_Controller::Game_Controller(const char* _field_object, const char* _1_player_pawn_object, const char* _2_player_pawn_object)
{
	field.init(_field_object);

	pawns = new Pawn * [2];
	pawns[0] = new Pawn[pawns_per_player];
	for (unsigned int i = 0; i < pawns_per_player; ++i)
	{
		pawns[0][i].init(_1_player_pawn_object);
		field.put_pawn_into_cell(i / 3, i % 3, &pawns[0][i]);
		pawns[0][i].set_status(Pawn::Status::snap_to_cell);
	}
	pawns[1] = new Pawn[pawns_per_player];
	for (unsigned int i = 0; i < pawns_per_player; ++i)
	{
		pawns[1][i].init(_2_player_pawn_object);
		field.put_pawn_into_cell(field.get_cells_count() - 3 + (i / 3), field.get_cells_count() - 3 + (i % 3), &pawns[1][i]);
		pawns[1][i].set_status(Pawn::Status::snap_to_cell);
	}

	ai.set_field_size(field.get_cells_count(), field.get_cells_count());
	for (unsigned int i = 0; i < field.get_cells_count(); ++i)
		for (unsigned int j = 0; j < field.get_cells_count(); ++j)
			if (field.peek_at_pawn_in_cell(i, j)) ai.field_element(i, j) = true;

	ai.set_pawns(pawns[1], 9, pawns[0], 9);
	ai.set_bases({ {5, 5}, {7, 7} } , { {0, 0}, {2, 2} });

	/*ai.set_pawn_and_destination();
	auto& pwn = ai.get_pawn_to_move_next();
	ai.set_start_pos(pwn.get_current_cell().first, pwn.get_current_cell().second);

	ai.build_path();
	auto test = ai.get_full_path();*/
}

Game_Controller::~Game_Controller()
{
	for (unsigned int i = 0; i < 2; ++i)
		delete[] pawns[i];
}



bool Game_Controller::pawn_is_in_this_cell(const std::pair<int, int>& _cell, const Pawn* _pawn) const
{
	return _pawn->get_current_cell().first == _cell.first && _pawn->get_current_cell().second == _cell.second;
}

bool Game_Controller::cell_is_next_to_pawn(const std::pair<int, int>& _cell, const Pawn* _pawn) const
{
	auto ppos = _pawn->get_current_cell();
	return (abs(ppos.first - _cell.first) == 1 && abs(ppos.second - _cell.second) == 0)
		|| (abs(ppos.first - _cell.first) == 0 && abs(ppos.second - _cell.second) == 1);
}

bool Game_Controller::pawn_belongs_to_current_player(const Pawn* _pawn) const
{
	if (_pawn == nullptr) return false;
	for (unsigned int i = 0; i < pawns_per_player; ++i)
		if (*_pawn == pawns[current_player][i]) return true;
	return false;
}


int Game_Controller::player_won()
{
    bool won = true;

    for(unsigned int i=0; i<=2; ++i)
    {
        for(unsigned int j=0; j<=2; ++j)
        {
            for(unsigned int p=0; p < pawns_per_player; ++p)
            {
                if(field.peek_at_pawn_in_cell(i, j) == &pawns[0][p] || field.peek_at_pawn_in_cell(i, j) == nullptr)
                {
                    won = false;
                    break;
                }
            }
            if(!won) break;
        }
        if(!won) break;
    }
    if(won) return 1;

    won = true;

    for(unsigned int i=5; i<=7; ++i)
    {
        for(unsigned int j=5; j<=7; ++j)
        {
            for(unsigned int p=0; p < pawns_per_player; ++p)
            {
                if(field.peek_at_pawn_in_cell(i, j) == &pawns[1][p] || field.peek_at_pawn_in_cell(i, j) == nullptr)
                {
                    won = false;
                    break;
                }
            }
            if(!won) break;
        }
        if(!won) break;
    }
    if(won) return 0;

    return -1;
}



void Game_Controller::update()
{
	for (unsigned int i = 0; i < 2; ++i)
		for (unsigned int j = 0; j < pawns_per_player; ++j)
			pawns[i][j].update();

    if(winner != -1) return;
	if (current_player == 1)
	{
		bool can_make_move = true;
		for (unsigned int i = 0; i < pawns_per_player; ++i)
		{
			if (pawns[0][i].get_status() == Pawn::Status::snap_to_cell)
			{
				can_make_move = false;
				break;
			}
		}

		if (can_make_move)
		{
			//update map
			for (unsigned int i = 0; i < field.get_cells_count(); ++i)
				for (unsigned int j = 0; j < field.get_cells_count(); ++j)
					if (field.peek_at_pawn_in_cell(i, j)) ai.field_element(i, j) = true;
					else ai.field_element(i, j) = false;

			ai.set_pawn_and_destination();
			auto& pwn = ai.get_pawn_to_move_next();
			ai.set_start_pos(pwn.get_current_cell().first, pwn.get_current_cell().second);
			//ai.build_path();
			auto test = ai.get_next_step();

			held_pawn = field.get_pawn_from_cell(pwn.get_current_cell().first, pwn.get_current_cell().second);

			if (held_pawn->get_current_cell().first == test.first && held_pawn->get_current_cell().second == test.second)
				held_pawn->set_snap_speed(-0.5f);
			else
				held_pawn->set_snap_speed(0.5f);

			field.put_pawn_into_cell(test.first, test.second, held_pawn);

			held_pawn->set_status(Pawn::Status::snap_to_cell);
			held_pawn = nullptr;

            winner = player_won();

			current_player = (current_player == 0 ? 1 : 0);
		}
	}
	else
	{
		if (held_pawn != nullptr)
		{
			if (LEti::Event_Controller::mouse_button_was_released(GLFW_MOUSE_BUTTON_LEFT) && held_pawn->get_status() != Pawn::Status::idle)
			{
				std::pair<int, int> closest_cell = field.get_closest_cell(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y);
				if (closest_cell.first != -1 && field.is_cell_empty(closest_cell.first, closest_cell.second) &&
					!pawn_is_in_this_cell(closest_cell, held_pawn) && cell_is_next_to_pawn(closest_cell, held_pawn))
				{
					field.put_pawn_into_cell(closest_cell.first, closest_cell.second, held_pawn);
					current_player = (current_player == 0 ? 1 : 0);
				}
				else
					field.put_pawn_into_cell(held_pawn->get_current_cell().first, held_pawn->get_current_cell().second, held_pawn);

				held_pawn->set_snap_speed(0.5f);
				held_pawn->set_status(Pawn::Status::snap_to_cell);
				held_pawn = nullptr;

                winner = player_won();
			}
		}
		if (LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::pair<int, int> closest_cell = field.get_closest_cell(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y);
			if (closest_cell.first != -1)
			{
				if (pawn_belongs_to_current_player(field.peek_at_pawn_in_cell(closest_cell.first, closest_cell.second)))
				{
					held_pawn = field.get_pawn_from_cell(closest_cell.first, closest_cell.second);
					held_pawn->set_snap_speed(0.05f);
					held_pawn->set_status(Pawn::Status::snap_to_cursor);
				}
			}
		}
	}
}

void Game_Controller::draw()
{
	field.draw();
	for (unsigned int i = 0; i < 2; ++i)
		for (unsigned int j = 0; j < pawns_per_player; ++j)
			if (&pawns[i][j] != held_pawn) pawns[i][j].draw();
	if (held_pawn != nullptr) held_pawn->draw();
}
