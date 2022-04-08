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


struct tree_node
{
    intp data;

    tree_node* parent = nullptr;

    tree_node* next = nullptr;
    tree_node* next_alt = nullptr;

    tree_node(const intp& _data) : data(_data) { }
};


bool is_step_unique(const intp& _step, const tree_node* _node)
{
    if(_node == nullptr) return true;
    if(_node->data == _step) return false;
    if(_node->parent == nullptr) return true;

    if(_node->parent->next_alt == _node)
    {
        tree_node* ptr = _node->parent;
        if(ptr->next == nullptr) return is_step_unique(_step, _node->parent);
        ptr = ptr->next;
        while(ptr->next_alt != nullptr)
        {
            ptr = ptr->next;
        }
        return is_step_unique(_step, ptr);
    }
    else
        return is_step_unique(_step, _node->parent);

    return is_step_unique(_step, _node->parent);
}

void add_after(tree_node* _node, bool alt, const intp& _data)
{
    if(alt)
    {
        tree_node* ptr = _node->next_alt;
        _node->next_alt = new tree_node(_data);
        _node->next_alt->parent = _node;
        _node->next_alt->next = ptr;
        ptr->parent = _node->next_alt;
    }
    else
    {
        tree_node* ptr = _node->next;
        _node->next = new tree_node(_data);
        _node->next->parent = _node;
        _node->next->next = ptr;
        ptr->parent = _node->next;
    }
}

void delete_branch(tree_node* _node)
{
    if(_node->next_alt == nullptr && _node->next == nullptr)
    {
        delete _node;
        return;
    }
    delete_branch(_node->next_alt);
    delete_branch(_node->next);
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

#include "Utility.h"

//tree_node* find_closer_alt_path(const intp& _dest, float _next_step_dist, tree_node* _node, tree_node* _last)
//{
//    if(!_node) return nullptr;

//    direction d = direction::up;

//    unsigned int i = 0;
//    for(; i<4; ++i)
//    {
//        intp temp = _node->data.after_movement(d);
//        if(at(temp) || !is_step_unique(temp, _last) || !out_of_bounds(temp))
//            rotate_cw(d);
//        else break;
//    }

//    if(i==4)
//    {
//        if(_node->parent->next_alt == _node)
//        {
//            tree_node* ptr = _node->parent;
//            while(ptr->next != nullptr)
//                ptr = ptr->next;
//            return find_closer_alt_path(_step, ptr);
//        }
//        else
//            return find_closer_alt_path(_step, _node->parent);

//        return find_closer_alt_path(_step, _node->parent);
//    }


//}

int main()
{
    intp start = {1, 0};
    intp dest = {3, 1};
    node[0][2] = true;
    node[1][2] = true;
    node[2][2] = true;
    node[2][1] = true;
    intp cur = {1, 0};



    tree_node* head = new tree_node(cur);
    tree_node* last = head;






    return 0;
}

























































