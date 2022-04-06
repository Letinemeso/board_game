#include "Event_Controller.h"
#include "Shader.h"
#include "Camera.h"
#include "Resource_Loader.h"

#include "include/Game_Field.h"
#include "include/Pawn.h"

#include <thread>

#include "Object.h"

int main()
{
	LEti::Event_Controller::init_and_create_window(800, 600, "Board Game");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LEti::Shader::init_shader("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
	ASSERT(!LEti::Shader::is_valid());
	LEti::Shader::set_texture_uniform_name("input_texture");
	LEti::Shader::set_transform_matrix_uniform_name("transform_matrix");
	LEti::Shader::set_projection_matrix_uniform_name("projection_matrix");

	LEti::Camera::setup_orthographic_matrix();


	LEti::Resource_Loader::load_object("bgr", "resources/models/background.mdl");
	LEti::Resource_Loader::load_object("field", "resources/models/field.mdl");
	LEti::Resource_Loader::load_object("pawn", "resources/models/pawn.mdl");

	Pawn pawn;
	pawn.init("pawn");
	pawn.set_cell_pos(300.0f, 300.0f);
	pawn.set_snap_speed(1.0f);
	//pawn.set_status(Pawn::Status::snap_to_previous);

	LEti::Object background;
	background.init("bgr");

	Game_Field field;
	field.init("field");

	while (!LEti::Event_Controller::window_should_close())
	{
		glClear(GL_COLOR_BUFFER_BIT);
	
		LEti::Event_Controller::update();
		LEti::Camera::update(true, false);

		pawn.update();

		if (LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "follow\n";
			pawn.set_status(Pawn::Status::follow_cursor);
		}
		if (LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			std::cout << "idle\n";
			pawn.set_status(Pawn::Status::snap_to_previous);
			//pawn.set_status(Pawn::Status::idle);
		}


		background.draw();
		field.draw();
		pawn.draw();

		LEti::Event_Controller::swap_buffers();
	}


	return 0;
}
