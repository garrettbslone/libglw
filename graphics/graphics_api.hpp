//
// Created by Garrett on 3/27/22.
//

#ifndef GLW_GRAPHICS_API_HPP
#define GLW_GRAPHICS_API_HPP

namespace glw {

/*
 * Primitive topology types for drawing geometry.
 */
enum class topology {
  POINTS,
  LINES,
  LINE_LOOP,
  LINE_STRIP,
  TRIANGLES,
  TRIANGLE_STRIP,
  TRIANGLE_FAN,
};

#define POINTS_TOPO topology::POINTS
#define LINES_TOPO topology::LINES
#define LINE_LOOP_TOPO topology::LINE_LOOP
#define LINE_STRIP_TOPO topology::LINES_STRIP
#define TRIANGLES_TOPO topology::TRIANGLES
#define TRIANGLE_STRIP_TOPO topology::TRIANGLE_STRIP
#define TRIANGLE_FAN_TOPO topology::TRIANGLE_FAN

/*
 * Different API backends.
 */
enum class graphics_api {
  OPEN_GL,
#ifdef HAVE_VULKAN
  VULKAN,
#endif
  NONE,
};

#define API_OPEN_GL graphics_api::OPEN_GL
#ifdef HAVE_VULKAN
#define API_VULKAN graphics_api::VULKAN
#endif
#define API_NONE graphics_api::NONE

}

#endif //GLW_GRAPHICS_API_HPP
