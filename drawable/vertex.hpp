//
// Created by Garrett on 3/29/22.
//

#ifndef GLW_VERTEX_HPP
#define GLW_VERTEX_HPP

#include <vector>

#include "../ext/glm/glm/glm.hpp"

namespace glw {

struct vertex {
public:
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};

    inline bool operator==(const vertex &other) const
    {
        return position == other.position &&
                color == other.color &&
                normal == other.normal &&
                uv == other.uv;
    }

    static inline std::vector<float> coalesce(std::vector<vertex> &vertices)
    {
        std::vector<float> data;
        data.reserve(vertices.size() * 11);

        for (auto &v: vertices) {
            data.push_back(v.position.x);
            data.push_back(v.position.y);
            data.push_back(v.position.z);
        }

        for (auto &v: vertices) {
            data.push_back(v.color.x);
            data.push_back(v.color.y);
            data.push_back(v.color.z);
        }

        for (auto &v: vertices) {
            data.push_back(v.normal.x);
            data.push_back(v.normal.y);
            data.push_back(v.normal.z);
        }

        for (auto &v: vertices) {
            data.push_back(v.uv.x);
            data.push_back(v.uv.y);
        }

        return data;
    }

    static inline std::vector<float> coalesce_interleaved(std::vector<vertex> &vertices)
    {
        std::vector<float> data;
        data.reserve(vertices.size() * 11);

        for (auto &v: vertices) {
            data.push_back(v.position.x);
            data.push_back(v.position.y);
            data.push_back(v.position.z);

            data.push_back(v.color.x);
            data.push_back(v.color.y);
            data.push_back(v.color.z);

            data.push_back(v.normal.x);
            data.push_back(v.normal.y);
            data.push_back(v.normal.z);

            data.push_back(v.uv.x);
            data.push_back(v.uv.y);
        }

        return data;
    }
};

}

#endif //GLW_VERTEX_HPP
