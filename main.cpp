#include <bits/stdc++.h>

#include "DigitalTerrainModel.h"
using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <data_file_path> <image_width>" << endl;
        return EXIT_FAILURE;
    }

    string dataFilePath = argv[1];
    int imageWidth = stoi(argv[2]); 

    // Load and process the digital terrain model
    DigitalTerrainModel dtm;
    if (!dtm.loadFromFile(dataFilePath)) {
        return EXIT_FAILURE;
    }

    dtm.projectPoints(); // Project geographic coordinates to cartesian coordinates
    dtm.BoundingBox(imageWidth); // Compute the bounding box of the terrain model

    // 4. (Future steps) Delaunay triangulation, rasterization, image generation...
    
    return EXIT_SUCCESS;
}