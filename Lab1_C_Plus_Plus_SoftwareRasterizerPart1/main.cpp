/** @file main.cpp
 *  @brief Entry point into our program.
 *  
 *  Welcome to the Great Looking Software Render 
 *  code base (Yes, I needed something with a gl prefix).
 *
 *  This is where you will implement your graphics API.
 *
 *  Compile on the terminal with: 
 *
 *  clang++ -std=c++11 main.cpp -o main
 *
 *  @author Mike Shah
 *  @bug No known bugs.
 */

// Load our libraries
#include <iostream>

// Some define values
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 320

// C++ Standard Libraries
#include <iostream>

// User libraries
#include "GL.h"
#include "Color.h"
#include "TGA.h"
#include "Maths.h"

// Create a canvas to draw on.
TGA canvas(WINDOW_WIDTH,WINDOW_HEIGHT);


// Implementation of Bresenham's Line Algorithm
// The input to this algorithm is two points and a color
// This algorithm will then modify a canvas (i.e. image)
// filling in the appropriate colors.
void drawLine(Vec2 v0, Vec2 v1, TGA& image, ColorRGB c){
    bool steep = false;
    if(std::abs(v0.x-v1.x)<std::abs(v0.y-v1.y)){
        // If the line is steep we want to transpose the image.
        std::swap(v0.x,v0.y);
        std::swap(v1.x,v1.y);
        steep = true;
    }
    if(v0.x>v1.x){  // make it left-to-right
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }
    for(int x = v0.x; x <= v1.x; ++x){
        float t = (x-v0.x)/(float)(v1.x-v0.x);
        int y = v0.y*(1.0f-t) + v1.y*t;
        if(steep){
            canvas.setPixelColor(y,x,c);
        }else{
            canvas.setPixelColor(x,y,c);
        }
    }
}

int maxEl(int* arr, int len) {
    int max = 0;
    if (len > 0) {
        max = arr[0];
    }

    for (int i = 1; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

int minEl(int* arr, int len) {
    int min = 0;
    if (len > 0) {
        min = arr[0];
    }

    for (int i = 1; i < len; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    return min;
}

// perpDotProduct(v1, v2) = vx1 * vy2 - vy1 * vx2
int crossProduct(Vec2 v0, Vec2 v1) {
    return v0.x * v1.y - v0.y * v1.x;
}

// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2, TGA& image, ColorRGB c){
    if(glFillMode==LINE){
        drawLine(v0,v1,image,c);
        drawLine(v1,v2,image,c);
        drawLine(v2,v0,image,c);
    }
    else {
        int xs[3] = { v0.x, v1.x, v2.x };
        int ys[3] = { v0.y, v1.y, v2.y };

        int maxX = maxEl(xs, 3);
        int maxY = maxEl(ys, 3);
        int minX = minEl(xs, 3);
        int minY = minEl(ys, 3);

        Vec2 vs0 = Vec2(v1.x - v0.x, v1.y - v0.y);
        Vec2 vs1 = Vec2(v2.x - v0.x, v2.y - v0.y);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                Vec2 q = Vec2(x - v0.x, y - v0.y);

                float s = (float) crossProduct(q, vs1) / crossProduct(vs0, vs1);
                float t = (float) crossProduct(vs0, q) / crossProduct(vs0, vs1);

                if (s >= 0 && t >= 0 && s + t <= 1) {
                    canvas.setPixelColor(x, y, c);
                }
            }
        }
    }
}



// Main
int main(){

    // A sample of color(s) to play with
    ColorRGB red;
    red.r = 255; red.g = 0; red.b = 0;
    
    ColorRGB blue;
    blue.r = 0; blue.g = 0; blue.b = 255;

    ColorRGB green;
    green.r = 0; green.g = 255; green.b = 0;
    
    // Points for our Line
    Vec2 line[2] = {Vec2(0,0), Vec2(100,100)};

    // Set the fill mode
    glPolygonMode(FILL);

    // Draw a line
    drawLine(line[0],line[1],canvas,red);

    // Data for our triangle
    Vec2 tri[3] = {Vec2(160,60),Vec2(150,10),Vec2(75,190)};

    // Draw a triangle
    triangle(tri[0],tri[1],tri[2],canvas,red);
    triangle(Vec2(20, 200), Vec2(200, 280), Vec2(100, 260), canvas, blue);
    triangle(Vec2(200, 150), Vec2(150, 200), Vec2(250, 200), canvas, green);

    // Output the final image
    canvas.outputTGAImage("graphics_lab2.ppm");

    return 0;
}
