#include "../include/Pawn.h"

Pawn::Pawn() : LEti::Object()
{

}

Pawn::~Pawn()
{

}



void Pawn::set_status(Status _status)
{
	status = _status;
	if (status >= Status::snap_to_cursor)
	{
		glm::vec3 bsp = get_pos();
		before_snap_x = bsp.x;
		before_snap_y = bsp.y;
	}
}

Pawn::Status Pawn::get_status() const
{
	return status;
}


void Pawn::set_cell_pos(float _x, float _y)
{
	current_cell_x = _x;
	current_cell_y = _y;
}

void Pawn::set_snap_speed(float _speed)
{
	snap_speed = _speed;
}



void Pawn::set_current_cell(const std::pair<int, int> &_cell)
{
    current_cell_index = _cell;
}

const std::pair<int, int>& Pawn::get_current_cell() const
{
    return current_cell_index;
}



void Pawn::snap_to_pos(float _x, float _y)
{
	if (snap_speed < 0.0001f)
	{
		set_pos(_x, _y, 0.0f);
		set_status(Status::idle);
		return;
	}

	glm::vec3 current_pos = get_pos();

    float stride_x = (before_snap_x - _x) / snap_speed * 2.6f * LEti::Event_Controller::get_dt(),
        stride_y = (before_snap_y - _y) / snap_speed * 2.6f * LEti::Event_Controller::get_dt();

    float distance_between_init_and_snap = LEti::Utility::get_distance({before_snap_x, before_snap_y, 0.0f}, {_x, _y, 0.0f});
    float distance_between_curr_and_snap = LEti::Utility::get_distance({before_snap_x, before_snap_y, 0.0f}, current_pos);

    float acceleration = 1.0f - (fabs(0.5f - (distance_between_curr_and_snap/distance_between_init_and_snap)) * 1.8f);
    stride_x *= acceleration;
    stride_y *= acceleration;

    if (distance_between_init_and_snap < distance_between_curr_and_snap + LEti::Utility::vector_length({stride_x, stride_y, 0.0f}))
	{
		set_pos(_x, _y, 0.0f);
		set_status(Status::idle);
	}
	else
    {
		set_pos(current_pos.x - stride_x, current_pos.y - stride_y, 0.0f);
	}

}



void Pawn::update()
{
	if (status == Status::follow_cursor)
    {
		set_pos(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y, 0.0f);
	}
	else if (status == Status::snap_to_cursor)
	{
		/*if (LEti::Event_Controller::get_cursor_position().x < 0.0f || LEti::Event_Controller::get_cursor_position().y < 0.0f
			|| LEti::Event_Controller::get_cursor_position().x > LEti::Event_Controller::get_window_data().width
			|| LEti::Event_Controller::get_cursor_position().y > LEti::Event_Controller::get_window_data().height)
		{
            status = Status::snap_to_cell;
		}*/
		snap_to_pos(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y);
        if(LEti::Utility::get_distance(get_pos(), {LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y, 0.0f}) < 0.001f)
            set_status(Status::follow_cursor);
    }
    else if (status == Status::snap_to_cell)
	{
		snap_to_pos(current_cell_x, current_cell_y);
	}
}
