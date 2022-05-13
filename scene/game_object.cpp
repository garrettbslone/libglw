//
// Created by Garrett on 4/27/22.
//

#include "game_object.hpp"

namespace glw {

game_object::~game_object()
{

}

game_object::game_object(id_t id)
{
    this->components_.push_back(new transform());
    this->id_ = id;
}

}
