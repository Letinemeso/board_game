#include "Event_Controller.h"
#include "Shader.h"
#include "Camera.h"
#include "Resource_Loader.h"

#include "include/Game_Controller.h"

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
    LEti::Resource_Loader::load_object("pawn_cross", "resources/models/pawn_cross.mdl");
    LEti::Resource_Loader::load_object("pawn_circle", "resources/models/pawn_circle.mdl");

	LEti::Object background;
	background.init("bgr");

    Game_Controller controller("field", "pawn_cross", "pawn_circle");

	while (!LEti::Event_Controller::window_should_close())
	{
		glClear(GL_COLOR_BUFFER_BIT);
	
		LEti::Event_Controller::update();
		LEti::Camera::update(true, false);

        controller.update();

		background.draw();

        controller.draw();

		LEti::Event_Controller::swap_buffers();
	}


	return 0;
}
