#include <iostream>
#include <cctype>
#include "ray.h"

vec3 color(const ray& r) {
	// Linear blending based on y coordinate

	// Make it an unit vector so -1.0 < y < 1.0
	vec3 unit_direction = unit_vector(r.direction());
	// Scale y to t such that 0.0 < t < 1.0
	float t = 0.5 * (unit_direction.y() + 1.0);
	// Linear blend(lerp) where top is blue(0.5,0.7,1.0) and bottom is white
	// Formula: blend_color = (1-t)*white + t*blue
	return (1.0-t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {

	int nx = 1280;
	int ny = 720;
	if (argc > 2) {
		nx = atoi(argv[1]);
		ny = atoi(argv[2]);
	}
	
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	// World coords of lower left corner
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	// Horizontal distance from origin
	vec3 horizontal(4.0, 0.0, 0.0);
	// Vertical distance from origin
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}