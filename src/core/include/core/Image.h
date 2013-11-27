/*
 * Image.h
 *
 *  Created on: 21 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <memory>
#include <QImage>

namespace HighFlyers
{
class Image
{
	int width;
	int height;
	int stride;
	int bpp;

	std::shared_ptr<unsigned char> data;

public:
	Image();
	Image( int width, int height, int stride, int bpp,
			unsigned char* data, bool take_ownership = true );
	Image( int width, int height, int stride, int bpp,
			std::shared_ptr<unsigned char> data );
	Image( const QImage& image );
	virtual ~Image(){}

	QImage to_qimage() const;

	int get_width() const { return width; }
	int get_height() const { return height; }
	int get_stride() const { return stride; }
	int get_bpp() const { return bpp; }
	std::shared_ptr<unsigned char> get_data() { return data; }
};
}

#endif /* IMAGE_H_ */
