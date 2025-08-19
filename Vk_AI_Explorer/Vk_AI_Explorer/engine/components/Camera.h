#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace ecs::components
{
    struct Camera
    {
        glm::vec3 target{0.0f};
        float distance = 5.0f;
        float theta = 0.0f;  // Horizontal angle
        float phi = glm::pi<float>() * 0.5f;  // Vertical angle
        float fov = glm::radians(45.0f);
        float near_plane = 0.1f;
        float far_plane = 100.0f;
        
        // Limits
        float min_distance = 0.5f;
        float max_distance = 50.0f;
        float min_phi = 0.01f;
        float max_phi = glm::pi<float>() - 0.01f;
        
        // Default values for reset
        glm::vec3 default_target{0.0f};
        float default_distance = 5.0f;
        float default_theta = 0.0f;
        float default_phi = glm::pi<float>() * 0.5f;
        
        bool is_active = false;

        glm::vec3 get_position() const
        {
            float x = target.x + distance * sin(phi) * cos(theta);
            float y = target.y + distance * cos(phi);
            float z = target.z + distance * sin(phi) * sin(theta);
            return glm::vec3(x, y, z);
        }

        glm::mat4 get_view_matrix() const
        {
            return glm::lookAt(get_position(), target, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::mat4 get_projection_matrix(float aspect_ratio) const
        {
            return glm::perspective(fov, aspect_ratio, near_plane, far_plane);
        }

        glm::vec3 screen_to_world_ray(float screen_x, float screen_y, 
                                     float screen_width, float screen_height,
                                     float aspect_ratio) const
        {
            // Convert screen coordinates to NDC
            float ndc_x = (2.0f * screen_x) / screen_width - 1.0f;
            float ndc_y = 1.0f - (2.0f * screen_y) / screen_height;
            
            // Create ray in view space
            glm::vec4 ray_clip = glm::vec4(ndc_x, ndc_y, -1.0f, 1.0f);
            glm::vec4 ray_eye = glm::inverse(get_projection_matrix(aspect_ratio)) * ray_clip;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
            
            // Transform to world space
            glm::vec3 ray_world = glm::vec3(glm::inverse(get_view_matrix()) * ray_eye);
            return glm::normalize(ray_world);
        }

        void reset()
        {
            target = default_target;
            distance = default_distance;
            theta = default_theta;
            phi = default_phi;
        }
    };
}
