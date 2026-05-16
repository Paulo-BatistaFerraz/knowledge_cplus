#include <iostream>
using namespace std;

// Write your solution here
// C++23 using GCC 14.2
// Debug with std::cerr or std::clog.
// !!! IMPORTANT !!!
// 99% of headers are pre-compiled for you server-side.
// If your submission fails to compile due to a missing header, add it to your submission.
#include <vector>
#include <cmath>

struct Point { double x, y; };

double shortest_path(Point university, Point accommodation, std::vector<Point> tasks) {
    double shortest_distance = std::numeric_limits<double>::infinity();
    for (const Point& task : tasks) {
        double distance = std::sqrt(std::pow(task.x - university.x, 2) + std::pow(task.y - university.y, 2));
        if (distance < shortest_distance) {
            shortest_distance = distance;
        }
    }
    return shortest_distance;
    double distance_to_accommodation = std::sqrt(std::pow(accommodation.x - university.x, 2) + std::pow(accommodation.y - university.y, 2));
    return shortest_distance + distance_to_accommodation;
    
}

int main(){


}