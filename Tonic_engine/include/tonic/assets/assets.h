#pragma once

#include <cstdint>

namespace tonic::assets
{

    namespace mesh_cells
    { 

        // 2D common mesh cell types

        namespace triangle
        {
            constexpr uint32_t vertex_count = 3;
            constexpr float vertices[] = {
                 0.0f,  0.5f, 0.f, // top
                -0.5f, -0.5f, 0.f, // left
                 0.5f, -0.5f, 0.f  // right
            };
            constexpr uint32_t element_count = 3;
            constexpr uint32_t elements[] = {
                0, 1, 2
            };
        }

        namespace rectangle /* i will always be using up-left X down-right diagonal with front facing quads */
        {
            constexpr uint32_t vertex_count = 4;
            constexpr float vertices[] = {
                -0.5f,  0.5f, 0.f, // up-left
                 0.5f, -0.5f, 0.f, // down-right
                -0.5f, -0.5f, 0.f, // down-left
                 0.5f,  0.5f, 0.f  // up-right
            };

            constexpr uint32_t element_count = 6;
            constexpr uint32_t elements[] = {
                0,2,1, // left
                0,3,2  // right
            };
        }



        // 3D common cell types

        namespace tetrahedron
        {
            constexpr uint32_t vertex_count = 4;
            constexpr float vertices[] = {
                 0.0f,  0.5f,  0.0f, // top
                -0.5f, -0.5f,  0.5f, // front-left
                 0.5f, -0.5f,  0.5f, // front-right
                 0.0f,  0.0f, -0.5f  // back
            };
            
            constexpr uint32_t element_count = 12;
            constexpr uint32_t elements[] = {
                0,2,1, // front
                0,1,3, // left
                0,3,2, // right

                1,3,2  // bottom
            };
        }

        namespace pyramid
        {
            constexpr uint32_t vertex_count = 5;
            constexpr float vertices[] = {
                 0.0f,  0.5f,  0.0f,  // top
                -0.5f, -0.5f,  0.5f,  // front-left
                 0.5f, -0.5f,  0.5f,  // front-right
                 0.5f, -0.5f, -0.5f,  // back-right
                -0.5f, -0.5f, -0.5f,  // back-left
            };

            constexpr uint32_t element_count = 18;
            constexpr uint32_t elements[] = {
                0,1,2, // front
                0,2,3, // right
                0,3,4, // back
                0,4,1, // left
                2,1,4, // bot left
                2,4,3  // bot right
            };
        }

        namespace triangular_prism
        {
            constexpr uint32_t vertex_count = 6;
            constexpr float vertices[] = {
                 0.0f,  0.5f,  0.5f, // front-top
                -0.5f, -0.5f,  0.5f, // front-left
                 0.5f, -0.5f,  0.5f, // front- right

                 0.0f,  0.5f, -0.5f, // back-top
                 0.5f, -0.5f, -0.5f, // back-right
                -0.5f, -0.5f, -0.5f, // back-left
            };

            constexpr uint32_t element_count = 32;
            constexpr uint32_t elements[] = {
                0,1,2, // front
                3,2,0, // left-left
                3,5,
            };
        }

        namespace hexahedron
        {
            constexpr uint32_t vertex_count = 8;
            constexpr float vertices[] = {
                 -0.5f,  0.5f,  0.5f,  // front-up-left 
                 -0.5f, -0.5f,  0.5f,  // front-down-left  
                  0.5f, -0.5f,  0.5f,  // front-down-right
                  0.5f,  0.5f,  0.5f,  // front-up-right   

                  0.5f,  0.5f, -0.5f,  // back-up-right    
                  0.5f, -0.5f, -0.5f,  // back-down-right  
                 -0.5f, -0.5f, -0.5f,  // back-down-left   
                 -0.5f,  0.5f, -0.5f,  // back-up-left     
            };

            constexpr uint32_t element_count = 36;
            constexpr uint32_t elements[] = {
                0,2,1,  0,3,2, // front
                7,3,0,  7,4,3, // top
                1,5,6,  1,2,5, // bot
                7,1,6,  7,0,1, // left
                3,5,2,  3,4,5, // right
                4,6,5,  4,7,6  // back
            };
        }
    }
}