//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

namespace xd {

size_t getPixelFormatSize(const PixelFormat format)
{
	switch(format) {
		case ALPHA: return 1;
		case LUMINANCE: return 1;
		case LUMINANCE_ALPHA: return 2;
		case RGB: return 3;
		case RGBA: return 4;
	}
	return 0;
}

Pixmap::Pixmap(const PixelFormat format) :
	m_width(0),
	m_height(0),
	m_data(0),
	m_format(format)
{
}

Pixmap::Pixmap(const uint width, const uint height, const uchar *data, const PixelFormat format) :
	m_width(width),
	m_height(height),
	m_format(format)
{
	// Copy pixels
	m_pixelSize = getPixelFormatSize(m_format);
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width * height * m_pixelSize];
		memcpy(m_data, data, width * height * m_pixelSize);
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const uint width, const uint height, const PixelFormat format) :
	m_width(width),
	m_height(height),
	m_format(format)
{
	// Create empty pixmap
	m_pixelSize = getPixelFormatSize(m_format);
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width * height * m_pixelSize];
		memset(m_data, 0, width * height * m_pixelSize);
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const Pixmap &other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_format = other.m_format;
	m_pixelSize = getPixelFormatSize(m_format);
	if(other.m_data)
	{
		m_data = new uchar[m_width * m_height * m_pixelSize];
		memcpy(m_data, other.m_data, m_width*m_height*m_pixelSize);
	}
	else
	{
		m_data = 0;
	}
}

/*Pixmap &Pixmap::operator=(Pixmap &other)
{
	swap(m_data, other.m_data);
	swap(m_width, other.m_width);
	swap(m_height, other.m_height);
	return *this;
}*/

Pixmap::~Pixmap()
{
	delete[] m_data;
}

const uchar *Pixmap::getData() const
{
	return m_data;
}

uint Pixmap::getWidth() const
{
	return m_width;
}

uint Pixmap::getHeight() const
{
	return m_height;
}

PixelFormat Pixmap::getFormat() const
{
	return m_format;
}

Color Pixmap::getColor(const uint x, const uint y) const
{
	Color color;
	if(x < m_width && y < m_height)
	{
		memcpy(&color, m_data + (x + y*m_width) * m_pixelSize, 4);
	}
	else
	{
		//warn("");
	}
	return color;
}

void Pixmap::setColor(const uint x, const uint y, const Color &color)
{
	if(x < m_width && y < m_height)
	{
		memcpy(m_data + (x + y*m_width) * m_pixelSize, &color, m_pixelSize);
	}
	else
	{
		//warn("");
	}
}

void Pixmap::fill(const Color &color)
{
	for(uint y = 0; y < m_height; ++y)
	{
		for(uint x = 0; x < m_width; ++x)
		{
			memcpy(m_data + (x + y*m_width) * m_pixelSize, &color, m_pixelSize);
		}
	}
}

void Pixmap::clear()
{
	fill(Color(0));
}

void Pixmap::exportToFile(const string &path) const
{
	// TODO: REFACTORING
	//XAssetManager::SavePixmap(path, (uchar*)m_data, m_width, m_height);
}

}