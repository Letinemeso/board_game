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

void Pawn::set_closest_cell_pos(float _x, float _y)
{
	closest_cell_x = _x;
	closest_cell_y = _y;
}

void Pawn::set_snap_speed(float _speed)
{
	snap_speed = _speed;
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

	float stride_x = (before_snap_x - _x) / snap_speed * LEti::Event_Controller::get_dt(),
		stride_y = (before_snap_y - _y) / snap_speed * LEti::Event_Controller::get_dt();
	//LEti::Utility::get_distance(current_pos, glm::vec3(stride_x, stride_y, 0.0f))
	if (LEti::Utility::vector_length(glm::vec3(stride_x, stride_y, 0.0f)) > LEti::Utility::get_distance(current_pos, glm::vec3(_x, _y, 0.0f)))
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
		std::cout << LEti::Event_Controller::get_cursor_position().x << '\t' << LEti::Event_Controller::get_cursor_position().y
			<< '\n' << get_pos().x << '\t' << get_pos().y << "\n\n";
		set_pos(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y, 0.0f);
	}
	else if (status == Status::snap_to_cursor)
	{
		/*if (LEti::Event_Controller::get_cursor_position().x < 0.0f || LEti::Event_Controller::get_cursor_position().y < 0.0f
			|| LEti::Event_Controller::get_cursor_position().x > LEti::Event_Controller::get_window_data().width
			|| LEti::Event_Controller::get_cursor_position().y > LEti::Event_Controller::get_window_data().height)
		{
			status = Status::snap_to_previous;
		}*/
		snap_to_pos(LEti::Event_Controller::get_cursor_position().x, LEti::Event_Controller::get_cursor_position().y);
	}
	else if (status == Status::snap_to_closest)
	{
		snap_to_pos(closest_cell_x, closest_cell_y);
	}
	else if (status == Status::snap_to_previous)
	{
		snap_to_pos(current_cell_x, current_cell_y);
	}
}
