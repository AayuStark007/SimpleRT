#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "dependencies/stb_image_write.h"

int main() {
	int x, y, n;
	unsigned char *data = stbi_load("tex.png", &x, &y, &n, STBI_rgb);
	stbi_write_png("foo_out.png", x, y, 3, data, 0);
}