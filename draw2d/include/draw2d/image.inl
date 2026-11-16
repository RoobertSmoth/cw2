inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.
	Index i = get_linear_index( aX, aY);
	i *= 4;
	ColorU8_sRGB_Alpha pixel;

	pixel.r = mData[i+0];
	pixel.g = mData[i+1]; 
	pixel.b= mData[i+2];
	pixel.a = mData[i+3]; //ALPHA VALUE	

	return {pixel}; // TODO: remove this line when you implement this
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	//return 0; // TODO: remove this line when you implement this function
	return (aY * mWidth) + aX; 
}
