#pragma once

namespace d2d
{
	class ImageLoader
	{
	public:
		static unsigned int loadTexture(void* data, int& width, int& height);

		static unsigned char* loadData(const char* filepath, int& width, int& height, int& format);

	private:
		static unsigned char* loadPNG(void* data, int& width, int& height, int& format);

	}; // ImageLoader
}