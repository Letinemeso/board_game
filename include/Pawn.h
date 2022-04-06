#ifndef __PAWN
#define __PAWN

#include "Debug.h"

#include "Utility.h"
#include "Object.h"


class Pawn : public LEti::Object
{
public:
	enum class Status {
		idle = 0,
		follow_cursor,
		snap_to_cursor,
		snap_to_closest,
		snap_to_previous
	};

private:
	Status status = Status::idle;

private:
	float current_cell_x = 0.0f, current_cell_y = 0.0f;
	float closest_cell_x = 0.0f, closest_cell_y = 0.0f;

	float before_snap_x = 0.0f, before_snap_y = 0.0f;
	float snap_speed = 0.0f;

public:
	Pawn();
	~Pawn() override;

public:
	//void init(const char* _object) override;

public:
	void set_status(Status _status);
	Status get_status() const;

	void set_cell_pos(float _x, float _y);
	void set_closest_cell_pos(float _x, float _y);
	void set_snap_speed(float _speed);

private:
	void snap_to_pos(float _x, float _y);

public:
	void update() override;

};


#endif 