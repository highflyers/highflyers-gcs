/*
 * Image.cpp
 *
 *  Created on: 21 lis 2013
 *      Author: Marcin Kolny
 */

#include "core/Image.h"

using namespace HighFlyers;

Image::Image()
: width( 0 ),
  height( 0 ),
  stride( 0 ),
  bpp( 0 ),
  data(nullptr)
{
}

Image::Image( int width, int height, int stride, int bpp,
		unsigned char* data, bool take_ownership )
: width( width ),
  height( height ),
  stride( stride ),
  bpp ( bpp )
{
	if (take_ownership)
		this->data = std::shared_ptr<unsigned char>( data );
	else
	{
		int size = height * stride;
		unsigned char* tmp_data = new unsigned char[height * stride];
		std::copy(data, data + size, tmp_data);
		this->data = std::shared_ptr<unsigned char>( tmp_data );
	}
}

Image::Image( int width, int height, int stride, int bpp,
		std::shared_ptr<unsigned char> data )
: width( width ),
  height( height ),
  stride( stride ),
  bpp ( bpp ),
  data( data )
{
}

Image::Image( const QImage& image )
{
	width = image.width();
	height = image.height();
	stride = image.bytesPerLine();

	switch (image.format())
	{
	case QImage::Format_Mono:
		bpp = 1;
		break;
	case QImage::Format_RGB32:
		bpp = 4;
		break;
	case QImage::Format_RGB16:
		bpp = 2;
		break;
	case QImage::Format_RGB888:
		bpp = 3;
		break;
	default:
		bpp = 0;
	}

	unsigned char* tmp_data = new unsigned char[image.byteCount()];
	std::copy( image.bits(), image.bits() + image.byteCount(), tmp_data );
	data = std::shared_ptr<unsigned char>( tmp_data );
}

QImage Image::to_qimage() const
{
	QImage::Format format;

	switch (bpp)
	{
	case 1:
		format = QImage::Format_Mono;
		break;
	case 2:
		format = QImage::Format_RGB16;
		break;
	case 3:
		format = QImage::Format_RGB888;
		break;
	case 4:
		format = QImage::Format_RGB32;
		break;
	default:
		format = QImage::Format_Invalid;
	}

	return QImage( data.get(), width, height, stride, format );
}
