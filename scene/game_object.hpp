//
// Created by Garrett on 4/27/22.
//

#ifndef GLW_GAME_OBJECT_HPP
#define GLW_GAME_OBJECT_HPP

#include "component.hpp"
#include "../util/exception.hpp"

#include <vector>

namespace glw {

class game_object {
public:
    using id_t = unsigned int;

    virtual ~game_object();

    game_object(const game_object &) = delete;
    game_object &operator=(const game_object &) = delete;
    game_object(game_object &&) = default;
    game_object &operator=(game_object &&) = default;

    template
    <typename T>
    inline T get_component()
    {
        for (auto c: this->components_) {
            if (dynamic_cast<T>(c))
                return c;
        }

        return nullptr;
    }

    static inline game_object *create()
    {
        static id_t curr_id = 0;
        return new game_object{curr_id++};
    }

private:
    explicit game_object(id_t);

    std::vector<component *> components_;
    id_t id_;
};

}

#endif //GLW_GAME_OBJECT_HPP
