#pragma once

namespace d2d
{
	class ImageLoader
	{
	public:
		static unsigned int loadTexture(void* data);

	private:
		static unsigned char* loadPNG(void* data, int& width, int& height, int& format);

	}; // ImageLoader
}