#include <fstream>
#include <sstream>
#include <iostream>

#include "DigitalTerrainModel.h"
#include "Point.h" 

using namespace std;

// Destructor to clean up projection resources
DigitalTerrainModel::~DigitalTerrainModel() {
    if (projectionHandle_) {
        proj_destroy(projectionHandle_);
    }
}

// Initialize the projection context
bool DigitalTerrainModel::initializeProjection() {
    if (projectionHandle_ != nullptr) {
        return true; // Already initialized
    }
    
    // Use the member variable
    projectionHandle_ = proj_create_crs_to_crs(
        PJ_DEFAULT_CTX,
        "+proj=longlat +datum=WGS84",
        "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
        NULL);

    if (projectionHandle_ == nullptr) {
        cerr << "Error: Failed to initialize Proj context." << endl;
        return false;
    }
    return true;
}

// Load elevation points from a file
bool DigitalTerrainModel::loadFromFile(const string& filePath) {

    points_.clear(); // Clear any existing points
    ifstream inputFile(filePath); // Open the file

    // Check if the file opened successfully
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the data file " << filePath << endl;
        return false;
    }

    string line; // To hold each line
    while (getline(inputFile, line)) {
        stringstream ss(line); // String stream to parse the line
        Point p;
        
        if (ss >> p.latitude >> p.longitude >> p.altitude) {
            points_.push_back(p);
        } else {
            cerr << "Warning: Skipping malformed line: " << line << endl;
        }
    }

    if (points_.empty()) {
        cerr << "Error: Data file was read, but no valid points were found." << endl;
        return false;
    }

    cout << "Successfully loaded " << points_.size() << " elevation points." << endl;
    return true;
}

// Project geographic coordinates to cartesian coordinates 
void DigitalTerrainModel::projectPoints() {
    // This code uses the model disponibilized in Moodle
    PJ* P = proj_create_crs_to_crs(
        PJ_DEFAULT_CTX,
        "+proj=longlat +datum=WGS84",
        "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
        NULL);

    PJ_COORD geo_coord, cartesian_coord;

    for (int i = 0; i < points_.size(); i++) {
        geo_coord.lpzt.lam = points_[i].longitude; // X axis
        geo_coord.lpzt.phi = points_[i].latitude; // Y axis
        geo_coord.lpzt.z = 0.; // The Z axis is not relevant for this project

        cartesian_coord = proj_trans(P, PJ_FWD, geo_coord); // Converts the data

        points_[i].x = cartesian_coord.xy.x;
        points_[i].y = cartesian_coord.xy.y;

        cout << "(" << geo_coord.lpzt.lam << "," << geo_coord.lpzt.phi << ")"
        << "CONVERTED FROM index > " << i << " < IN ---> " 
        << "(" << cartesian_coord.xy.x << "," << cartesian_coord.xy.y << ")\n";
    }
}

// Compute the bounding box of the terrain model with linear complexity O(n) because it iterates through all points once
void DigitalTerrainModel::BoundingBox(int imageWidth) {
    if (points_.empty()) {
        cerr << "Error: No points available to compute bounding box." << endl;
        return;
    }   

    // Initialize Bounding Box with the first point's coordinates
    min_x_ = max_x_ = points_[0].x;
    min_y_ = max_y_ = points_[0].y;
    
    // Iterate over all points to find min/max
    for (const auto& p : points_) {
        // Find min/max X
        if (p.x < min_x_) min_x_ = p.x;
        if (p.x > max_x_) max_x_ = p.x;
        // Find min/max Y
        if (p.y < min_y_) min_y_ = p.y;
        if (p.y > max_y_) max_y_ = p.y;
    }

    // Calculate the dimensions of the terrain in projected units (meters)
    double terrain_width = max_x_ - min_x_;
    double terrain_height = max_y_ - min_y_;

    // Handle case where width might be zero (though unlikely with MNT data)
    if (terrain_width == 0.0) {
        cerr << "Error: Terrain width is zero. Cannot calculate image height." << endl;
        imageHeight_ = 0;
        return;
    }

    // Calculate Image Height preserving the aspect ratio
    // imageHeight = (imageWidth * terrain_height) / terrain_width
    imageHeight_ = static_cast<int>(
        (static_cast<double>(imageWidth) * terrain_height) / terrain_width
    );
    
    // Verif log
    cout << "Terrain Bounding Box (x, y): (" << min_x_ << ", " << min_y_ << ") to (" 
         << max_x_ << ", " << max_y_ << ")" << endl;
    cout << "Calculated Image Dimensions: " << imageWidth << "x" << imageHeight_ << " pixels." << endl;
}