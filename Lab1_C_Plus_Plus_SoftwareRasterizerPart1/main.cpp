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
#include <algorithm>
#include <vector>

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
bool validPixel(int x, int y, Vec2 v0, Vec2 v1, Vec2 v2);

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






// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2,TGA& image, ColorRGB c){
   
    if(glFillMode==LINE || true){
        drawLine(v0,v1,image,c);
        drawLine(v1,v2,image,c);
        drawLine(v2,v0,image,c);
    }

    // TODO: Draw a filled triangle
    std::vector<Vec2> triVec;

    triVec.push_back(v0);
    triVec.push_back(v1);
    triVec.push_back(v2);

    //boundaries to iterate over box to fill
    int left = v0.x; //leftmost x value to be set
    int right = v0.x; //rightmost x value to be set
    int top = v0.y; //uppermost y value to be set
    int bottom = v0.y; //uppermost y value to be set

    for (Vec2 &v : triVec) {
        if (v.x < left) {
            left = v.x;
        }
        if (v.x > right) {
            right = v.x;
        }
        if (v.y > bottom) {
            bottom = v.y;
        }
        if (v.y < top) {
            top = v.y;
        }
    }


    std::cout << std::endl << "left is " << left << std::endl;
    std::cout << "right is " << right << std::endl;
    std::cout << "bottom is " << bottom << std::endl;
    std::cout << "top is " << top << std::endl;


    if (true) {

        for (int row = top; row <= bottom; row++) {
            for (int col = left; col <= right; col++) {

                if (validPixel(row, col, v0, v1, v2)) {
                    canvas.setPixelColor(row, col, c);
                }


            }
        }
    }

    
}


//returns the area of a triangle define by three points
float area(int x1, int y1, int x2, int y2, int x3, int y3) {

    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);

}

//test in the point (x,y) is inside the triandle define by v0,v1 and v2
bool validPixel(int x, int y, Vec2 v0, Vec2 v1, Vec2 v2) {


    float mainArea = area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

    float a1 = area(x, y, v1.x, v1.y, v2.x, v2.y);

    float a2 = area(v0.x, v0.y, x, y, v2.x, v2.y);

    float a3 = area(v0.x, v0.y, v1.x, v1.y, x, y);

    return (mainArea == a1 + a2 + a3);

}

// Main
int main(){

    // A sample of color(s) to play with
    ColorRGB red;
    red.r = 255; red.g = 0; red.b = 0;
        
    
    // Points for our Line
    Vec2 line[2] = {Vec2(0,0), Vec2(100,100)};

    // Set the fill mode
    glPolygonMode(FILL);

    // Draw a line
    drawLine(line[0],line[1],canvas,red);

    // Data for our triangle
    Vec2 tri[3] = {Vec2(160,60),Vec2(15,50),Vec2(75,190)};

    // Draw a triangle
    triangle(tri[0],tri[1],tri[2],canvas,red);

    // Output the final image
    canvas.outputTGAImage("graphics_lab2.ppm");

    return 0;
}
