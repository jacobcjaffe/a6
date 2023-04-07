#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <vector>
#include <set>
#include <cstdlib>

using namespace std;

ofstream outstream;
ifstream instream;

// tuple for x value, y value, weight for djikstra's algorithm
typedef tuple<int, int, int> p;

int main(int argc, char * argv[]) {

    
    if (argc != 3) {
        cout << "ERROR: Input and output files must be specified." << endl;
        return -1;
    }

    instream.open(argv[1]);
    outstream.open(argv[2]);

    int radius, circleX, circleY, numLines, subdiv = 1;
    instream >> radius >> circleX >> circleY >> numLines;

    /* 
    Do a bottom up approach, start adding to a line to set, set subdivisions to 2.
    for each new line, iterate through set.
    Everytime you add a line, increase subdivisions by 1.
    If intersection lies inside circle, not on the edge, increment subdivisions by 1.
    */

    while(radius != 0 || circleX != 0 || circleY != 0 || numLines != 0) {

        // vector of integer pairs, first int being slope, second int being y-intercept
        std::vector<std::pair<double, double>> lines;
        double x1, x2, y1, y2;
        double slope1, intercept1, distance;
        // get the lines, add them to the vector by calculating slope and intercept
        // if line doesn't intersect with the circle, skip since it has no effect on number of subdivisions.
        for(int i = 0; i < numLines; i++) {
            instream >> x1 >> y1 >> x2 >> y2;

            // distance formula
            distance = std::abs(((x2 - x1) * (y1 - circleY)) - ((x1 - circleX) * (y2 - y1))) /
            std::sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
            if (distance > radius) {
                // std::cout << "DEBUG: skipping line: " << i << std::endl;
                continue;
            }
            else {
                slope1 = (y1 - y2) / (x1 - x2);
                intercept1 = y1 - (slope1 * x1);

                // if vertical line, store equation as just the intercept, and slope as infinity
                if(std::abs(slope1) == std::numeric_limits<double>::infinity()) {
                    intercept1 = x1;
                    slope1 = abs(slope1);
                }
                // std::cout << "DEBUG: slope is " << slope1 << std::endl;
                // std::cout << "DEBUG: intercept is " << intercept1 << std::endl;
                lines.push_back(std::make_pair(slope1, intercept1));
            }
        }

        double slope2, intercept2, ix, iy;
        bool found = false;

        // if lines is 0, no lines go through the circle
        if(lines.size() == 0) {
            subdiv = 1;
        }
        // if lines is 1, only one line runs through the circle
        else if(lines.size() == 1) {
            subdiv = 2;
        }
        else {
            subdiv++;
            // iterate throuh the input for lines vector to find intercepts.
            // if intercepts lie in circle, add 4 subdivisions, if it lies on circle, add 3
            for(int i = 0; i < lines.size() - 1; i++) {
                subdiv++;
                slope1 = lines[i].first;
                intercept1 = lines[i].second;
                found = false;
                for(int j = i + 1; j < lines.size(); j++) {
                    slope2 = lines[j].first;
                    intercept2 = lines[j].second;

                    // if they run parallel, skip because division by 0
                    if(slope1 == slope2) {
                        continue;
                    }
                    
                    // std::printf("intercept1: %f, intercepts 2: %f, slope1: %f, slope2: %f \n", intercept1, intercept2, slope1, slope2);
                    // if either line is a vertical line, modify the intersection equation
                    if (slope1 == std::numeric_limits<double>::infinity()) {
                        ix = intercept1;
                        iy = slope2 * ix + intercept2;
                    }
                    else if (slope2 == std::numeric_limits<double>::infinity()) {
                        ix = intercept2;
                        iy = slope1 * ix + intercept1;
                    }
                    else {
                        ix = (intercept2 - intercept1) / (slope1 - slope2);
                        iy = (slope1 * ix) + intercept1;
                    }
                    // debug
                    // std::printf("intersection lies at (%f, %f) \n", ix, iy);
                    // calculate distance from center of the circle
                    distance = std::sqrt((ix - circleX) * (ix - circleX)  + (iy - circleY) * (iy - circleY));
                    // std::printf("Distance between lines %i and %i is: %f \n", i, j, distance);
                    if(distance < radius) {
                        // std::printf("Lines: %i and %i intersect in circle \n", i, j); 
                        subdiv += 1;
                        
                        // found = true;
                        // break;
                    }
                    else if(distance == radius) {
                        // std::printf("Lines: %i and %i intersect on circle \n", i, j); 
                        // subdiv += 1 - (found);
                        // break;
                        // found = true;
                    }
                    //if we reach the end without finding an intersection in/on circle, add 2
                    else if(j == lines.size()-1 && found == false) {
                        // std::printf("Lines: %i doesn't intersect in circle \n", i); 
                        // subdiv += 1;
                    }
                }
            }
        }

        outstream << subdiv << std::endl;

        // prepare for next case
        instream >> radius >> circleX >> circleY >> numLines;
        // std::cout << radius << " " << circleX << " " << circleY << " " << numLines;
        subdiv = 1;
    }

    // get the intersections between the lines
    // if intersection lies in circle, 4 sections created, if lies on circle, 3 sections
    // if intersection outside of circle, check to see if line intersects with circle at all
    instream.close();
    outstream.close();
    return 0;
}
