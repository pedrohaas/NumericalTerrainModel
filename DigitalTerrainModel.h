#ifndef DIGITAL_TERRAIN_MODEL_H
#define DIGITAL_TERRAIN_MODEL_H

#include <vector>
#include <string>

#include <proj.h>

using namespace std;

class Point;

// Represents a digital terrain model composed of elevation points.
class DigitalTerrainModel {
public:
    
    bool loadFromFile(const string& filePath); // Load elevation points from a file
    void projectPoints(); // Project geographic coordinates to cartesian coordinates
    ~DigitalTerrainModel(); // Destructor to clean up projection resources
    void BoundingBox(int imageWidth, int min_x, int max_x, int min_y, int max_y); // Compute the bounding box of the terrain model

    // Getter for the points vector
    const vector<Point>& getPoints() const {
        return points_;
    }

private:
    vector<Point> points_;
    PJ* projectionHandle_ = nullptr; // Member to hold the projection context
    
    double min_x_ = 0.0;
    double max_x_ = 0.0;
    double min_y_ = 0.0;
    double max_y_ = 0.0;
    int imageHeight_ = 0;

    bool initializeProjection();
};

#endif // DIGITAL_TERRAIN_MODEL_H