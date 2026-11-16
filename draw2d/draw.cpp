#include <draw2d/draw.hpp>

#include <algorithm>

#include <cmath>
#include <cassert>

#include <draw2d/sampler.hpp>
#include <draw2d/surface.hpp>

namespace
{
	inline
	unsigned classify_( Rect2F const& aRect, Vec2f const& aPoint )
	{
		unsigned ret{};
		if( aPoint.x < aRect.xmin )
			ret |= 1;
		else if( aPoint.x > aRect.xmin + aRect.width - 1.f )
			ret |= 2;
		
		if( aPoint.y < aRect.ymin )
			ret |= 4;
		else if( aPoint.y > aRect.ymin + aRect.height - 1.f )
			ret |= 8;
		
		return ret;
	}
}

// Implement in Task 1.3
void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	// TODO: your implementation goes here
	/*draws trinagle defined by 3 vertices, vertex assigned color
	colors should be barycentric interpolated across triangle
	function must work with reasonable inputs (work with clipping)
	interpolation is in RGB and only convert to sRGB when writing color to surface
	pick any method, should be reasonably efficient, no dynamic or system calls in triangle drawing method
	use triangles-sandbox to visually experiment with triangle drawing in isolation
	should also be able to see asteroids in main program
	must not change protoyupe of draw_triangle_interp functy, must use surface set_pixel_srgb to draw pixels*/

	//bounding box method
	//step 1 drawing the bounding box. smallest rectyangle that will enclose triangle. calc area of box
	//need highext y with lowest x point, then highest x with lowest y to find diagonal pixels

	//find top

	//not doing that long winded way above again
	Vec2f minimum;
	minimum.x = std::min(aP0.x, aP1.x);
	minimum.x = std::min(minimum.x, aP2.x);
	minimum.y = std::min(aP0.y, aP1.y);
	minimum.y = std::min(minimum.y, aP2.y);

	Vec2f maximum;
	maximum.x = std::max(aP0.x, aP1.x);
	maximum.x = std::max(maximum.x, aP2.x);
	maximum.y = std::max(aP0.y, aP1.y);
	maximum.y = std::max(maximum.y, aP2.y);


	//STOP GIVING ME THESE ERRORS!!!
	minimum.x = std::max( 0.f, std::floor(minimum.x) );
	minimum.y = std::max( 0.f, std::floor(minimum.y) );
	maximum.x = std::min( static_cast<float>(aSurface.get_width() - 1), std::ceil(maximum.x) );
	maximum.y = std::min( static_cast<float>(aSurface.get_height() - 1), std::ceil(maximum.y) );


	//i feel like i did it a bit backwards from top left to bottom right ooops
	float area_ABC = ((aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP1.y - aP0.y) * (aP2.x - aP0.x)) / 2.f;
	if( std::abs(area_ABC) < 1e-6f )
		return;
		//PLS dont be 0
	//use area

	//find centroid
	Vec2f centroid;
	// centroid.x = (aP0.x + aP1.x + aP2.x)/3;
	// centroid.y = (aP0.y + aP1.y + aP2.y)/3;

	Vec2f points[] = {aP0, aP1, aP2};

	//loop through bounding box
	//cp = a C0, b c1, g c2
	for (int y = minimum.y; y <= maximum.y; y++){
		for(int x = minimum.x; x<= maximum.x; x++){
			//center + 0.5
			centroid.x = x + 0.5f;
			centroid.y = y + 0.5f;
			float alpha;
			float beta;
			float gamma;
			ColorF Cx;
			float abg_areas[3];

			for (int pix = 0; pix < 3; pix++){
				Vec2f A = points[(pix + 1) % 3];
				Vec2f B = points[(pix + 2) % 3];
				abg_areas[pix] = ((A.x - centroid.x) * (B.y - centroid.y) - (A.y - centroid.y) * (B.x - centroid.x)) / 2.f;
			}
			//fopund sub areas. need to be of same sign
			if ((abg_areas[0] >= 0.f && abg_areas[1] >= 0.f && abg_areas[2] >=0.f) || (abg_areas[0] <= 0.f && abg_areas[1] <= 0.f && abg_areas[2] <= 0.f)){
				//all postitive or all negative. good
				alpha = abg_areas[0]/area_ABC;
				beta = abg_areas[1]/area_ABC;
				gamma = abg_areas[2]/area_ABC;
				//cp = a C0, b c1, g c2
			}
			else{
				//skip pixel?
				continue;
			}
			
			//found min and max coords of sub triangle
			//found alpha, beta, gamma i thinkkkkkk
			Cx.r = alpha * aC0.r + beta * aC1.r + gamma * aC2.r;
			Cx.g = alpha * aC0.g + beta * aC1.g + gamma * aC2.g;
			Cx.b = alpha * aC0.b + beta * aC1.b + gamma * aC2.b;
			ColorU8_sRGB sCx = linear_to_srgb(Cx);


			//i feel like i did it a bit backwards from top left to bottom right ooops
			aSurface.set_pixel_srgb( x, y, sCx );
		}
	}







	
}

// Implement in Task 2.3
void draw_triangle_tex_masked( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, Sampler2D const& aSampler, Vec2f aT0, Vec2f aT1, Vec2f aT2 )
{
	// TODO: your implementation goes here

	//not doing that long winded way above again
	Vec2f minimum;
	minimum.x = std::min(aP0.x, aP1.x);
	minimum.x = std::min(minimum.x, aP2.x);
	minimum.y = std::min(aP0.y, aP1.y);
	minimum.y = std::min(minimum.y, aP2.y);

	Vec2f maximum;
	maximum.x = std::max(aP0.x, aP1.x);
	maximum.x = std::max(maximum.x, aP2.x);
	maximum.y = std::max(aP0.y, aP1.y);
	maximum.y = std::max(maximum.y, aP2.y);

	minimum.x = std::max( 0.f, std::floor(minimum.x) );
	minimum.y = std::max( 0.f, std::floor(minimum.y) );
	maximum.x = std::min( static_cast<float>(aSurface.get_width() - 1), std::ceil(maximum.x) );
	maximum.y = std::min( static_cast<float>(aSurface.get_height() - 1), std::ceil(maximum.y) );


	//i feel like i did it a bit backwards from top left to bottom right ooops
	float area_ABC = ((aP1.x-aP0.x)*(aP2.y-aP0.y) - (aP1.y-aP0.y)*(aP2.x-aP0.x))/2.f;
	if( std::abs(area_ABC) < 1e-6f ){
    	return;
	}
	//use area

	//find centroid
	Vec2f centroid;

	Vec2f points[] = {aP0, aP1, aP2};

	//loop through bounding box
	//cp = a C0, b c1, g c2
	for (int y = minimum.y; y <= maximum.y; y++){
		for(int x = minimum.x; x<= maximum.x; x++){
			//center + 0.5
			centroid.x = x + 0.5f;
			centroid.y = y + 0.5f;	//sample is in middle so add this
		
			float abg_areas[3];
			for (int pix = 0; pix < 3; pix++){
				Vec2f A = points[(pix + 1)%3];
				Vec2f B = points[(pix+2)%3];
				//goes through them awllllll
				abg_areas[pix] = ((A.x-centroid.x) * (B.y - centroid.y) - (A.y - centroid.y) * (B.x - centroid.x))/ 2.f;
			}
			//fopund sub areas. need to be of same sign
			float alpha, beta, gamma;
			if ((abg_areas[0] >= 0.f && abg_areas[1] >= 0.f && abg_areas[2] >=0.f) || (abg_areas[0] <= 0.f && abg_areas[1] <= 0.f && abg_areas[2] <=0.f)){
				//all postitive or all negative. good
				alpha = abg_areas[0]/area_ABC;
				beta = abg_areas[1]/area_ABC;
				gamma = abg_areas[2]/area_ABC;
				//cp = a C0, b c1, g c2
			}
			else{
				//skip pixel?
				continue;
			}
			
			//found min and max coords of sub triangle
			//found alpha, beta, gamma i thinkkkkkk
			Vec2f texCoord;
			texCoord.x = alpha * aT0.x + beta * aT1.x + gamma * aT2.x;
			texCoord.y = alpha * aT0.y + beta * aT1.y + gamma * aT2.y;

			//sample timee
			ColorF_Alpha sampled = aSampler.sample( texCoord );
			if( sampled.a < 0.5f ){
				continue;
			}
			ColorF Cx;
			Cx.r = sampled.r;
			Cx.g = sampled.g;
			Cx.b = sampled.b;
			ColorU8_sRGB sCx = linear_to_srgb(Cx);


			//i feel like i did it a bit backwards from top left to bottom right ooops
			aSurface.set_pixel_srgb( x, y, sCx );
		}
	}

}


// See Lecture 8
bool clip_line( Rect2F const& aTargetArea, Vec2f& aBegin, Vec2f& aEnd )
{
	// Note: If the line is clipped, update aBegin and aEnd correspondingly.
	// They are non-const references (&), meaning we can modify the values
	// passed in through them.
	//
	// Return true to indicate that the line should be drawn and false that
	// it should be discarded.

	// See https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
	auto b = classify_( aTargetArea, aBegin );
	auto e = classify_( aTargetArea, aEnd );

	while( true )
	{
		if( 0 == b && 0 == e )
			return true;
		
		if( b & e )
			return false;

		auto const next = e > b ? e : b;
		assert( next );

		float const dx = aEnd.x - aBegin.x;
		float const dy = aEnd.y - aBegin.y;

		float x, y;
		if( next & 8 )
		{
			x = aBegin.x + dx * (aTargetArea.ymin + aTargetArea.height -1.f - aBegin.y) / dy;
			y = aTargetArea.ymin + aTargetArea.height -1.f;
		}
		else if( next & 4 )
		{
			x = aBegin.x + dx * (aTargetArea.ymin - aBegin.y) / dy;
			y = aTargetArea.ymin;
		}
		else if( next & 2 )
		{
			x = aTargetArea.xmin + aTargetArea.width -1.f;
			y = aBegin.y + dy * (aTargetArea.xmin + aTargetArea.width -1.f - aBegin.x) / dx;
		}
		else // next & 1
		{
			x = aTargetArea.xmin;
			y = aBegin.y + dy * (aTargetArea.xmin - aBegin.x) / dx;
		}

		if( next == b )
		{
			aBegin.x = x;
			aBegin.y = y;
			b = classify_( aTargetArea, aBegin );
		}
		else
		{
			aEnd.x = x;
			aEnd.y = y;
			e = classify_( aTargetArea, aEnd );
		}
	}

	return true;
}

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	draw_line_solid( aSurface, aSurface.clip_area(), aBegin, aEnd, aColor );
}
void draw_line_solid( Surface& aSurface, Rect2F const& aClipArea, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	if( !clip_line( aClipArea, aBegin, aEnd ) )
		return;

	Vec2f dd = aEnd - aBegin;

	if( std::abs(dd.x) > std::abs(dd.y) )
	{
		float const ystep = dd.y / dd.x;

		if( aBegin.x > aEnd.x ) 
			std::swap( aBegin, aEnd );

		float const end = aEnd.x + 0.5f;
		for( float x = aBegin.x+0.5f, y = aBegin.y+0.5f; x <= end; x += 1.f, y += ystep )
			aSurface.set_pixel_srgb( std::uint32_t(x), std::uint32_t(y), aColor );
	}
	else
	{
		float const xstep = dd.x / dd.y;

		if( aBegin.y > aEnd.y ) 
			std::swap( aBegin, aEnd );

		float const end = aEnd.y + 0.5f;
		for( float x = aBegin.x+0.5f, y = aBegin.y+0.5f; y <= end; y += 1.f, x += xstep )
			aSurface.set_pixel_srgb( std::uint32_t(x), std::uint32_t(y), aColor );
	}
}



void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	// TODO: (Exercise G.1) your implementation goes here. Can ignore for CW2.
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	// TODO: (Exercise G.1) your implementation goes here. Can ignore for CW2.
}


