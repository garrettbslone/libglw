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

    vertex(const glm::vec3 &position = glm::vec3(0.f),
            const glm::vec3 &color = glm::vec3(0.f),
            const glm::vec3 &normal = glm::vec3(0.f),
            const glm::vec2 &uv = glm::vec3(0.f))
            : position(position), color(color), normal(normal), uv(uv) {}

    inline bool operator==(const vertex &other) const
    {
        return position == other.position &&
                color == other.color &&
                normal == other.normal &&
                uv == other.uv;
    }

    static inline std::vector<vertex> from_floats(const std::vector<float> &data)
    {
        assert(data.size() % 11 == 0 && "Data must contain position, color, normal, and uv values for each vertex!");

        std::vector<vertex> vertices;
        vertices.reserve(data.size() / 11);

        for (auto i = 0; i <= data.size() - 11; i += 11) {
            vertices.push_back({
                    {data[i], data[i + 1], data[i + 2]},
                    {data[i + 3], data[i + 4], data[i + 5]},
                    {data[i + 6], data[i + 7], data[i + 8]},
                    {data[i + 9], data[i + 10]}
            });
        }

        return vertices;
    }

    static inline std::vector<vertex> from_floats(
            const std::vector<float> &positions,
            const std::vector<float> &colors,
            const std::vector<float> &normals,
            const std::vector<float> &uvs
    )
    {
        assert(positions.size() == colors.size() && colors.size() == normals.size() \
            && positions.size() / 3 == uvs.size() >> 1 \
            && positions.size() % 3 == 0 && uvs.size() % 2 == 0 \
            && "There must be the same number of vertices for each type of data!");

        std::vector<vertex> vertices;
        vertices.reserve(uvs.size() / 2);

        for (auto i = 0, j = 0; i <= positions.size() - 3; i += 3, j += 2) {
            vertices.push_back({
                    {positions[i], positions[i + 1], positions[i + 2]},
                    {colors[i], colors[i + 1], colors[i + 2]},
                    {normals[i], normals[i + 1], normals[i + 2]},
                    {uvs[j], uvs[j + 1]}
            });
        }

        return vertices;
    }

    static inline std::vector<float> to_floats(const std::vector<vertex> &vertices)
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
