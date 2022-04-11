/*#include "Event_Controller.h"
#include "Shader.h"
#include "Camera.h"
#include "Resource_Loader.h"

//#include "Tree.h"

#include "include/Game_Controller.h"

#include <thread>
//#include <map>

#include "Object.h"


enum class direction
{
    up,
    down,
    left,
    right
};

void rotate_cw(direction& _d)
{
    switch(_d)
    {
        case direction::up: _d = direction::right; break;
        case direction::right: _d = direction::down; break;
        case direction::down: _d = direction::left; break;
        case direction::left: _d = direction::up; break;
    }
}


struct intp
{
    int x = 0, y = 0;
    intp(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(const intp& _other) const { return x == _other.x && y == _other.y; }
    bool operator!=(const intp& _other) const { return !(*this == _other); }

    void move(direction _d)
    {
        switch (_d)
        {
            case direction::up:{ ++y; break; }
            case direction::down:{ --y; break; }
            case direction::left:{ --x; break; }
            case direction::right:{ ++x; break; }
        }
    }
    intp after_movement(direction _d)
    {
        switch (_d)
        {
            case direction::up:{ return intp(x, y+1); }
            case direction::down:{ return intp(x, y-1); }
            case direction::left:{ return intp(x-1, y); }
            case direction::right:{ return intp(x+1, y); }
        }
    }
};

std::ostream& operator<<(std::ostream& _stream, const intp& _data)
{
    _stream << _data.x << '\t' << _data.y;
    return _stream;
}


struct list_node
{
    intp data;
    list_node* next = nullptr;
    list_node* prev = nullptr;

    list_node(const intp& _data) : data(_data) { }

//    ~list_node() { delete data; }

    void delete_next() { list_node* ptr = next->next; delete next; next = ptr; }
    list_node* add_next(const intp& _data) { list_node* ptr = next; next = new list_node(_data); next->next = ptr; next->prev = this; if(ptr) ptr->prev = next; return next; }
    list_node* add_prev(const intp& _data) { list_node* ptr = prev; prev = new list_node(_data); prev->next = this; prev->prev = ptr; if(ptr) ptr->next = prev; return prev; }

};

bool is_step_unique(const intp& _step, const list_node* _node)
{
    if(_node == nullptr) return true;
    if(_node->data == _step) return false;
    return is_step_unique(_step, _node->prev);
}


constexpr int size = 4;
bool node[size][size] = {{false}};

bool out_of_bounds(const intp& _ind)
{
    return !(_ind.x < 0 || _ind.y < 0 || _ind.x >= size || _ind.y >= size);
}


bool at(const intp& _ind)
{
    return node[_ind.x][_ind.y];
}


list_node* find_node_with_availible_path(list_node* _prev)
{
    if(!_prev) return _prev;
    direction d = direction::up;

    unsigned int i = 0;
    for(; i<4; ++i)
    {
        intp temp = _prev->data.after_movement(d);
        if(at(temp) || !is_step_unique(temp, _prev) || !out_of_bounds(temp)) rotate_cw(d);
        else break;
    }

    if(i < 4) return _prev;
    else return _prev->prev;
}





int main()
{
//    LEti::Tree<intp, 3> a;
//    intp start = {0, 0};
//    intp dest = {2, 3};

//    node[0][1] = true;
//    node[0][2] = true;
//    node[1][1] = true;
//    node[1][3] = true;
//    node[2][2] = true;

//    node[2][0] = true;


    intp start = {1, 0};
    intp dest = {3, 1};
//    node[1][2] = true;
//    node[2][2] = true;
//    node[2][1] = true;
//    node[2][0] = true;
    node[0][2] = true;
    node[1][2] = true;
    node[2][2] = true;
    node[2][1] = true;


    intp cur = start;

    list_node* head = new list_node(start);
    list_node* last = head;

    while(last->data != dest)
    {
        direction preferable_direction;
        if(last->data.y == dest.y)
        {
            if(cur.x < dest.x) preferable_direction = direction::right;
            else preferable_direction = direction::left;
        }
        else
        {
            if(cur.y < dest.y) preferable_direction = direction::up;
            else preferable_direction = direction::down;
        }

        unsigned int i = 0;
        for(; i<4; ++i)
        {
            intp temp = cur.after_movement(preferable_direction);
            if(at(temp) || !is_step_unique(temp, last) || !out_of_bounds(temp))
                rotate_cw(preferable_direction);
            else break;
        }

        if(i == 4)
        {
            list_node* ptr = find_node_with_availible_path(last);

            if(ptr == nullptr)
            {
                last = last->add_next({-1, -1});
                break;
            }

            for(i=0; i<4; ++i)
            {
                intp temp = ptr->data.after_movement(preferable_direction);
                if(at(temp) || !is_step_unique(temp, last) || !out_of_bounds(temp)) rotate_cw(preferable_direction);
                else
                {
                    last = ptr;
                    while(last->next != nullptr)
                        last->delete_next();
                    cur = last->data;

                    break;
                }
            }

        }

        cur.move(preferable_direction);
        last = last->add_next(cur);

    }

    std::cout << head->data << '\n';
    while(head->next != nullptr)
    {
        std::cout << head->next->data << '\n';
        head->delete_next();

    }
    delete head;

























//	LEti::Event_Controller::init_and_create_window(800, 600, "Board Game");

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	LEti::Shader::init_shader("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
//	ASSERT(!LEti::Shader::is_valid());
//	LEti::Shader::set_texture_uniform_name("input_texture");
//	LEti::Shader::set_transform_matrix_uniform_name("transform_matrix");
//	LEti::Shader::set_projection_matrix_uniform_name("projection_matrix");

//	LEti::Camera::setup_orthographic_matrix();


//	LEti::Resource_Loader::load_object("bgr", "resources/models/background.mdl");
//	LEti::Resource_Loader::load_object("field", "resources/models/field.mdl");
//    LEti::Resource_Loader::load_object("pawn_cross", "resources/models/pawn_cross.mdl");
//    LEti::Resource_Loader::load_object("pawn_circle", "resources/models/pawn_circle.mdl");

//	LEti::Object background;
//	background.init("bgr");

//    Game_Controller controller("field", "pawn_cross", "pawn_circle");

//	while (!LEti::Event_Controller::window_should_close())
//	{
//		glClear(GL_COLOR_BUFFER_BIT);

//		LEti::Event_Controller::update();
//		LEti::Camera::update(true, false);

//        controller.update();

//		background.draw();

//        controller.draw();

//		LEti::Event_Controller::swap_buffers();
//	}


    return 0;
}*/

#include <iostream>
#include "Tree.h"

enum class direction
{
    up,
    down,
    left,
    right
};

void rotate_cw(direction& _d)
{
    switch(_d)
    {
    case direction::up: _d = direction::right; break;
    case direction::right: _d = direction::down; break;
    case direction::down: _d = direction::left; break;
    case direction::left: _d = direction::up; break;
    }
}


struct intp
{
    int x = 0, y = 0;
    intp(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(const intp& _other) const { return x == _other.x && y == _other.y; }
    bool operator!=(const intp& _other) const { return !(*this == _other); }

    void move(direction _d)
    {
        switch (_d)
        {
        case direction::up:{ ++y; break; }
        case direction::down:{ --y; break; }
        case direction::left:{ --x; break; }
        case direction::right:{ ++x; break; }
        }
    }
    intp after_movement(direction _d)
    {
        switch (_d)
        {
        case direction::up:{ return intp(x, y+1); }
        case direction::down:{ return intp(x, y-1); }
        case direction::left:{ return intp(x-1, y); }
        case direction::right:{ return intp(x+1, y); }
        }
    }
};

unsigned int get_distance(const intp& _first, const intp& _second)
{
    return abs(_first.x - _second.x) + abs(_first.y - _second.y);
}

std::ostream& operator<<(std::ostream& _stream, const intp& _data)
{
    _stream << _data.x << '\t' << _data.y;
    return _stream;
}

//LEti::Tree<intp, 3> tree;


bool is_step_unique(const intp& _step, LEti::Tree<intp, 3>& _tree)
{
    auto iter = _tree.create_iterator();

    if(!iter.valid()) return true;

    while(!iter.end())
    {
        if(*iter == _step) return false;
        ++iter;
    }
    if(*iter == _step) return false;

    return true;
}


//constexpr int size = 5;
//bool node[size][size] = {{false}};

bool out_of_bounds(const intp& _ind, unsigned int _size_x, unsigned int _size_y)
{
    return _ind.x < 0 || _ind.y < 0 || _ind.x >= _size_x || _ind.y >= _size_y;
}


bool at(const intp& _ind, const bool* const* const _field)
{
    return _field[_ind.x][_ind.y];
}

bool can_move_here(const intp& _where, const bool* const* const _field, unsigned int _size_x, unsigned int _size_y, LEti::Tree<intp, 3>& _tree)
{
    if(out_of_bounds(_where, _size_x, _size_y)) return false;
    if(at(_where, _field)) return false;
    return is_step_unique(_where, _tree);
}

#include "Utility.h"

std::pair<LEti::Tree<intp, 3>::Iterator, intp> find_closest_pos(const intp& _dest, const bool* const* const _field, unsigned int _size_x, unsigned int _size_y, LEti::Tree<intp, 3>& _tree)
{
    auto iter = _tree.create_iterator();

    unsigned int min_dist = -1;
    intp min_dist_pos = {-1, -1};
    LEti::Tree<intp, 3>::Iterator mdp_parent = iter;

    while(!iter.end())
    {
        direction d = direction::up;
        for(unsigned int i=0; i<4; ++i)
        {
            if(!can_move_here((*iter).after_movement(d), _field, _size_x, _size_y, _tree)) { rotate_cw(d); continue; }
            unsigned int local_min_dist = get_distance(*iter, _dest);
            if(local_min_dist < min_dist)
            {
                min_dist = local_min_dist;
                min_dist_pos = (*iter).after_movement(d);
                mdp_parent = iter;
            }
            rotate_cw(d);
        }
        ++iter;
    }
    direction d = direction::up;
    for(unsigned int i=0; i<4; ++i)
    {
        if(!can_move_here((*iter).after_movement(d), _field, _size_x, _size_y, _tree)) { rotate_cw(d); continue; }
        unsigned int local_min_dist = get_distance(*iter, _dest);
        if(local_min_dist < min_dist)
        {
            min_dist = local_min_dist;
            min_dist_pos = (*iter).after_movement(d);
            mdp_parent = iter;
        }
        rotate_cw(d);
    }

    return {mdp_parent, min_dist_pos};
}

intp find_next_step(const intp& _current_pos, const intp& _destination, const bool* const* const _field, unsigned int _size_x, unsigned int _size_y)
{
    LEti::Tree<intp, 3> l_tree;
    auto iter = l_tree.create_iterator();
    iter.insert_into_availible_index(_current_pos);

    unsigned int count = 1;

    while(true)
    {
        auto a = find_closest_pos(_destination, _field, _size_x, _size_y, l_tree);
        if(a.second == intp(-1, -1))
            break;

        a.first.descend( a.first.insert_into_availible_index(a.second) );
        iter = a.first;

        ++count;

        if(*iter == _destination)
            break;
    }

    if(count < 2) return { -1, -1 };

    unsigned int second_index = 0;
    while(!iter.begin())
        second_index = iter.ascend();

    iter.descend(second_index);
    return *iter;
}






int main()
{
    bool** node = new bool * [5];
    for(unsigned int i=0; i<5; ++i)
    {
        node[i] = new bool[5];
        for(unsigned int j=0; j<5; ++j)
            node[i][j] = false;
    }

    node[1][2] = true;
    node[1][3] = true;
    node[2][3] = true;
    node[3][1] = true;
    node[3][2] = true;

    node[2][1] = true;

    intp cur = {2, 2};
    intp dest = {3, 3};


    std::cout << cur << '\n';
    while(cur != dest && cur != intp(-1, -1))
    {
        cur = find_next_step(cur, dest, node, 5, 5);
        std::cout << cur << "\n";
    }


    return 0;
}

























































