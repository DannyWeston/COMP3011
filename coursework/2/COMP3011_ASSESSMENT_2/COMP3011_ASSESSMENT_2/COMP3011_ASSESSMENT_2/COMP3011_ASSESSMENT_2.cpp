#define _USE_MATH_DEFINES 
#include <iostream>
#include <SDL.h>
#include <Windows.h>
#undef main

#include "DoNotModify.h"

float vertices[] =
{
    //pos            
    -0.5f, 0.5f, 0.f,         //tl
    0.5f, 0.5f, 0.f,          //tr
    0.0f, 0.0f, 0.f,          //b
};

void ClearColourBuffer(int r, int g, int b)
{
    for (int x = 0; x < PIXEL_W; x++) {
        for (int y = 0; y < PIXEL_H; y++) {
            colour_buffer[x][y].x = r;
            colour_buffer[x][y].y = g;
            colour_buffer[x][y].z = b;
        }
    }
}

triangle* AssembleTriangles(float* verts, int n_verts, int* n_tris)
{
    // Work out how many triangles we have
    *n_tris = (int)(n_verts / 3);

    // Dynamically allocate memory for the correct number of triangles
    triangle *triangles = (triangle*)malloc(sizeof(triangle) * (*n_tris));

    if (triangles == NULL) {
        return NULL;
    }

    // Populate each triangle with their corresponding vertex attribute data (position)
    for (int i = 0; i < (*n_tris); i++) {
        triangles[i].v1.pos = { verts[i * 9], verts[i * 9 + 1], verts[i * 9 + 2] };
        triangles[i].v2.pos = { verts[i * 9 + 3], verts[i * 9 + 4], verts[i * 9 + 5] };
        triangles[i].v3.pos = { verts[i * 9 + 6], verts[i * 9 + 7], verts[i * 9 + 8] };
    }

    return triangles;
}

void TransformToViewport(int width, int height, triangle* tri)
{
    tri->v1.pos.x = (tri->v1.pos.x + 1.0f) * width * 0.5f;
    tri->v1.pos.y = (tri->v1.pos.y + 1.0f) * height * 0.5f;

    tri->v2.pos.x = (tri->v2.pos.x + 1.0f) * width * 0.5f;
    tri->v2.pos.y = (tri->v2.pos.y + 1.0f) * height * 0.5f;

    tri->v3.pos.x = (tri->v3.pos.x + 1.0f) * width * 0.5f;
    tri->v3.pos.y = (tri->v3.pos.y + 1.0f) * height * 0.5f;
    // Only working in 2d, so do not need to worry about z-coordinate
}

inline float bary_line(float ax, float ay, float bx, float by, float px, float py) {
    return (by - ay) * px + (ax - bx) * py + (bx * ay) - (ax * by);
}

void ComputeBarycentricCoordinates(float x, float y, triangle t, float& alpha, float& beta, float& gamma)
{
    // I made use of an inline function just to increase clarity of this function
    // Please see the "bary_line" function for more information
    alpha = bary_line(t.v2.pos.x, t.v2.pos.y, t.v3.pos.x, t.v3.pos.y, x, y);
    alpha /= bary_line(t.v2.pos.x, t.v2.pos.y, t.v3.pos.x, t.v3.pos.y, t.v1.pos.x, t.v1.pos.y);

    beta = bary_line(t.v1.pos.x, t.v1.pos.y, t.v3.pos.x, t.v3.pos.y, x, y);
    beta /= bary_line(t.v1.pos.x, t.v1.pos.y, t.v3.pos.x, t.v3.pos.y, t.v2.pos.x, t.v2.pos.y);

    gamma = 1 - alpha - beta;
}

void ShadeFragment(float alpha, float beta, float gamma, triangle t, int& r, int& g, int& b)
{
    r *= alpha;
    g *= beta;
    b *= gamma;
}

int main()
{
    COMP3011StartSDL();

    // Make the triangles using the vertices
    int tri_num;
    triangle* triangles = AssembleTriangles(vertices, (int)(sizeof(vertices) / sizeof(vertices[0])), &tri_num);

    // Clear the colour buffer
    ClearColourBuffer(0, 0, 0);

    while (1)
    {
        if (COMP3011PressedEscape())
            break;

        // Convert them from NDC space to screen space
        // Should be done during rasterisation, hence inside of this loop
        for (int i = 0; i < tri_num; i++) {
            TransformToViewport(PIXEL_W, PIXEL_H, &(triangles[i]));
        }

        // Loop through each pixel
        for (int x = 0; x < PIXEL_W; x++) {
            for (int y = 0; y < PIXEL_H; y++) {

                // Loop through each triangle
                for (int i = 0; i < tri_num; i++) {
                    // Compute barycentric coordinates
                    float alpha, beta, gamma;
                    ComputeBarycentricCoordinates(x, y, triangles[i], alpha, beta, gamma);

                    // Apply shader fragment if passes inside-triangle test
                    if (0 <= alpha && alpha <= 1 && 0 <= beta && beta <= 1 && alpha + beta <= 1) {
                        // Default to red triangles for now, and pass into shader fragment
                        int r = 255, g = 255, b = 255;
                        ShadeFragment(alpha, beta, gamma, triangles[i], r, g, b);

                        // Set colour buffer accordingly
                        colour_buffer[x][PIXEL_H - y - 1].x = (float)r;
                        colour_buffer[x][PIXEL_H - y - 1].y = (float)g;
                        colour_buffer[x][PIXEL_H - y - 1].z = (float)b;
                    }
                }
            }
        }

        COMP3011DisplayColourBuffer();
    }

    free(triangles);

    COMP3011StopSDL();

    return EXIT_SUCCESS;
}