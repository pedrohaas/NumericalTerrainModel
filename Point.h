/**
 * @brief Represents a single elevation point in the terrain model.
 */
class Point {
public:
    // Geographic coordinates (WGS84)
    double latitude;
    double longitude;

    // Altitude in meters
    double altitude; 

    // Projected cartesian coordinates (e.g., Lambert93)
    double x;
    double y;

    // Color assigned to the point in the final image (e.g., RGB values)
    unsigned char r, g, b; 

    Point() : latitude(0.0), longitude(0.0), altitude(0.0), x(0.0), y(0.0), r(0), g(0), b(0) {}
};