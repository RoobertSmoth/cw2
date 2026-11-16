/* Inline functions need to have a definition that is visible to the compiler
 * whenever the function is used. They could be defined in the header that
 * declares the function. However, to keep the definitions and declarations
 * somewhat apart, it is a common practice to move them to an "inline" file
 * such as this one (*.inl extension). This file is then #include:ed at the end
 * of the header, to ensure that whoever includes the header also automatically
 * includes the inline file.
 *
 * Inlining allows us to avoid any overheads related to call (when building in
 * "release mode" / with optimizations enabled). This makes functions like
 * set_pixel_srgb() zero overhead abstractions.
 *
 */
inline
void Surface::set_pixel_srgb( Index aX, Index aY, ColorU8_sRGB const& aColor )
{
	assert( aX < mWidth && aY < mHeight ); // IMPORTANT! This line must remain the first line in this function!

	// TODO: your implementation goes here
	//0,0 is bottom left, remember 
	//like G1 exercise

	Index i = get_linear_index( aX, aY);
	i = i*4;
	//hello

	mSurface[i+0] = aColor.r;
	mSurface[i+1] = aColor.g;
	mSurface[i+2] = aColor.b;
	mSurface[i+3] = 0; //rgbX <- can be 0
}

inline 
auto Surface::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto Surface::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
Rect2F Surface::clip_area() const noexcept
{
	return Rect2F{ 0.f, 0.f, float(mWidth), float(mHeight) };
}

inline
auto Surface::get_linear_index( Index aX, Index aY ) const noexcept -> Index
{	
	//FORMULA: LI = (aY x width) + aX
	return (aY * mWidth) + aX; // TODO: IMPLEMENTED
}


inline
ColorU8_sRGB Surface::get_pixel_srgb( Index aX, Index aY ) const noexcept
{
	assert( aX < mWidth && aY < mHeight ); // IMPORTANT! This line must remain the first line in this function!
	Index i = get_linear_index( aX, aY);
	i *= 4;
	ColorU8_sRGB pixel;

	pixel.r = mSurface[i+0];
	pixel.g = mSurface[i+1]; 
	pixel.b= mSurface[i+2];

	return {pixel};

	//return {0, 0, 0}; // TODO: remove this line when you implement this function.
}


