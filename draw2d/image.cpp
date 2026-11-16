#include <draw2d/image.hpp>

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>

#include <stb_image.h>

#include <draw2d/surface.hpp>
#include <support/error.hpp>

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"{}\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

// Task 1.4
void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// TODO: your implementation goes here
	/*you will need toimplement helper functions in draw2d image.inl
	should blit inpiut image of type IMageRGBA to positino specified by aposition
	position specifies where image center is placed
	input pixels with an alpha value 9componentof color_srgb_alpha color struct less than 128 shou;d be discarded
	consider efficiency in your implementation and dont make duynamic allocations
	if correct, find earth after flying a bit to right (should be off srreen initially
	you shoulnt change protoytpe of blit masked function. mustnot chane imagergba or load imaghe)*/


	/*
	loop through image pixels
	call get pixel
	if a > 128, set pixel
	if not continue
	make sure your image is aware of clipping*/
	//guessing aPosition is position on surface
	int image_width = aImage.get_width();
	int image_height = aImage.get_height();


	//want pixels of image that are on screen
	//OK SO IMAGE CAN BE OUT OF BONDS ON LEFT RIGHT UP AND DOWN
	Vec2i start_position; //the top left coordinate on surface for which the image will be placed
	start_position.x = static_cast<int>(aPosition.x - (image_width/2.f));
	start_position.y = static_cast<int>(aPosition.y - (image_height/2.f)); 


	int surface_width = aSurface.get_width();
	int surface_height = aSurface.get_height();

	//Vec2f surface;

	Vec2i min;
	min.x = std::max( 0, -start_position.x );
	min.y = std::max( 0, -start_position.y ); //find bounding box

	Vec2i max;
	
	max.x = std::min( image_width,  (surface_width  - start_position.x));
	max.y = std::min( image_height, (surface_height - start_position.y) );


	if( min.x >= max.x || min.y >= max.y ){
		return;
		//return if infeasible e
	}
	//too slow apparently so need to change it
	Vec2i actual;

	for( int y = min.y; y < max.y; y++ ){
		actual.y =  start_position.y + y;
		for( int x = min.x; x < max.x; x++ )
		{
			ColorU8_sRGB_Alpha pixel = aImage.get_pixel( x, y );
			actual.x =  start_position.x + x;
			if( pixel.a >= 128 )
			{
				ColorU8_sRGB pixel_final;
				pixel_final.r = pixel.r;
				pixel_final.g = pixel.g;
				pixel_final.b = pixel.b;
				//std::cout<<"surfaceX:" <<surface.x <<",surfaceY" << surface.y << std::endl;
				aSurface.set_pixel_srgb(actual.x , actual.y, pixel_final);
			}
		}
	}


	// //i think linear stuff comes top left to bottom right

}

// Task 2.1
void blit_masked_subregion( Surface& aSurface, ImageRGBA const& aImage, Vec2i aSourceOrigin, Vec2i aSourceSize, Vec2f aPosition )
{
	// TODO: your implementation goes here
	//assets/goddard8x8.png.
	//8x8 grid weach orientation
	//top left = 0]
	//increasing angle with each image (64 images)
	//bottom right is 354.375 
	int image_width = aSourceSize.x;
	int image_height = aSourceSize.y;
	//should be little size now


	Vec2i start_position; //the top left coordinate on surface for which the image will be placed
	start_position.x = static_cast<int>(aPosition.x - (image_width/2.f)); //make sure its int cause float can mess
	start_position.y = static_cast<int>(aPosition.y - (image_height/2.f)); 
	//should be start of each individual sample

	int surface_width = aSurface.get_width();
	int surface_height = aSurface.get_height();
	

	//Vec2f surface;

	//need min and max for real box
	Vec2i min;
	min.x = std::max( 0, -start_position.x ); //make sure types match. like int to int
	min.y = std::max( 0, -start_position.y ); //find bounding box

	Vec2i max;
	max.x = std::min( image_width,  (surface_width  - start_position.x));
	max.y = std::min( image_height, (surface_height - start_position.y) );

	//same as in original blit.

	if( min.x >= max.x || min.y >= max.y ){
		return;
		//return if boundaries dont make sense
	}

	Vec2i actual;

	//i think linear stuff comes top left to bottom right
	for (int y = min.y; y < max.y; y++){
		//surface.y = static_cast<std::int32_t>(std::floor(start_position.y + y));
		actual.y = start_position.y + y;

		for (int x = min.x; x < max.x; x++){
			//x and y will go through every pixel in image on screen
			//surface.x = static_cast<std::int32_t>(std::floor(start_position.x + x));
			actual.x = start_position.x + x;
			
			//check still in
			
			//lets grab that pixel now :P
			
			ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x + aSourceOrigin.x, y + aSourceOrigin.y);
			
			//alpha ask check
			//std::cout<<"surfaceX:" <<surface.x <<",surfaceY" << surface.y << std::endl;
			if (pixel.a >= 128){
				if (actual.x < 0 || actual.y <0 || actual.x >= surface_width || actual.y >= surface_height ){
					continue;
				}
				
				ColorU8_sRGB pixel_final;
				pixel_final.r = pixel.r;
				pixel_final.g = pixel.g;
				pixel_final.b = pixel.b;
				//std::cout<<"surfaceX:" <<surface.x <<",surfaceY" << surface.y << std::endl;
				aSurface.set_pixel_srgb(actual.x, actual.y, pixel_final);
			}
		}
	}
}

// Task 2.2
void blit_blend_subregion( Surface& aSurface, ImageRGBA const& aImage, Vec2i aSourceOrigin, Vec2i aSourceSize, Vec2f aPosition )
{
	// TODO: your implementation goes here
	//one image with 64 pictures of ship. full 360. 360/64 = 5.625 angle
	//change blit to cut out and draw one recyamgular tile from larger grid while inoring tyransparent background pixels
	//load sprite sheet and write drawing function that takes ships current angle, figures out which of 64 grid tiles match that angle and draws it at ships coordinates
	int image_width = aSourceSize.x;
	int image_height = aSourceSize.y;
	//should be little size now


	Vec2i start_position; //the top left coordinate on surface for which the image will be placed
	start_position.x = static_cast<int>(aPosition.x - (image_width/2.f));
	start_position.y = static_cast<int>(aPosition.y - (image_height/2.f)); 
	//should be start of each individual sample

	int surface_width = aSurface.get_width();
	int surface_height = aSurface.get_height();

	//Vec2f surface;

	Vec2i min;
	min.x = std::max( 0, -start_position.x );
	min.y = std::max( 0, -start_position.y ); //find bounding box

	Vec2i max;
	max.x = std::min( image_width,  (surface_width  - start_position.x));
	max.y = std::min( image_height, (surface_height - start_position.y) );

	if( min.x >= max.x || min.y >= max.y ){
		return;
		//return if boundaries dont make sense
	}
	Vec2i actual;
	//i think linear stuff comes top left to bottom right
	for (int y = min.y; y < max.y; y++){
		//surface.y = static_cast<std::int32_t>(std::floor(start_position.y + y));
		actual.y = start_position.y + y;

		for (int x = min.x; x < max.x; x++){
			//x and y will go through every pixel in image on screen
			//surface.x = static_cast<std::int32_t>(std::floor(start_position.x + x));
			actual.x = start_position.x + x;
			//check still in
			
			//lets grab that pixel now :P
			// if ((y + aSourceOrigin.y)< 0 || (x + aSourceOrigin.x) < 0 || (y + aSourceOrigin.y) >=  static_cast<int>(aImage.get_height()) || (x + aSourceOrigin.x) >=  static_cast<int>(aImage.get_width()) ){
			// 	continue;
			// }
			ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x + aSourceOrigin.x, y + aSourceOrigin.y);
			
			//alpha ask check
			//std::cout<<"surfaceX:" <<surface.x <<",surfaceY" << surface.y << std::endl;

			// if (surface.x < 0 || surface.y < 0 || surface.x >= surface_width || surface.y >= surface_height ){
			// 	continue;
			// }
			
			ColorU8_sRGB pixel_final;
			pixel_final.r = pixel.r;
			pixel_final.g = pixel.g;
			pixel_final.b = pixel.b;
			ColorF linear_pixel = linear_from_srgb(pixel_final);
			float alpha = pixel.a/255.f;

			ColorU8_sRGB bgPixel = aSurface.get_pixel_srgb(actual.x, actual.y);
			ColorF linear_bgpixel = linear_from_srgb(bgPixel);

			ColorF out;
			out.r =  alpha * linear_pixel.r + (1.0 - alpha) * linear_bgpixel.r;
			out.g =  alpha * linear_pixel.g + (1.0 - alpha) * linear_bgpixel.g;
			out.b =  alpha * linear_pixel.b + (1.0 - alpha) * linear_bgpixel.b;
			//std::cout<<"surfaceX:" <<surface.x <<",surfaceY" << surface.y << std::endl;
			ColorU8_sRGB new_out = linear_to_srgb(out);
			aSurface.set_pixel_srgb(actual.x, actual.y, new_out);
			
		}
	}
}


namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
