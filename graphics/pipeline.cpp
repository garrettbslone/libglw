//
// Created by Garrett on 3/18/22.
//

#include "pipeline.hpp"

#ifdef HAVE_VULKAN
#include "vulkan/vk_pipeline.hpp"
#endif

namespace glw {

pipeline *pipeline::create(
        pipeline_config_info *info,
        const std::string &vert_path,
        const std::string &frag_path
)
{
    switch (api::active) {
#ifdef HAVE_VULKAN
    case API_VULKAN:
        return reinterpret_cast<pipeline *>(new vk_pipeline(info, vert_path, frag_path));
#endif
    case API_OPEN_GL:
    default:
        return nullptr;
    }}

}