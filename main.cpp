#include <iostream>
#include <cctype>
#include <cfloat>

#include "drand48.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray &r, hitable *world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		// Make it an unit vector so -1.0 < y < 1.0
		vec3 unit_direction = unit_vector(r.direction());
		// Scale y to t such that 0.0 < t < 1.0
		float t = 0.5 * (unit_direction.y() + 1.0);
		// Linear blending(lerp) based on y coordinate
		// Here top is blue(0.5,0.7,1.0) and bottom is white
		// Formula: blend_color = (1-t)*white + t*blue
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene(int n)
{
	hitable **list = new hitable *[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{ //diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
				}
				else if (choose_mat < 0.95)
				{ //metal
					list[i++] = new sphere(center, 0.2,
										   new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
				}
				else
				{ //glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main(int argc, char *argv[])
{
	int nx = 200;
	int ny = 100;
	int ns = 500;
	if (argc > 2)
	{
		nx = atoi(argv[1]);
		ny = atoi(argv[2]);
		ns = atoi(argv[3]);
	}

	std::cout << "P3\n"
			  << nx << " " << ny << "\n255\n";

	// TODO: Build config file for objects to be added to scene.

	hitable *world = random_scene(500);

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0; //(lookfrom-lookat).length();
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}