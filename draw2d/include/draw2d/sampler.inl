#include <draw2d/image.hpp>

inline
Sampler2D::Sampler2D( ImageRGBA const& aImage )
	: mSrcImage( &aImage )
{}

inline
ColorF_Alpha Sampler2D::sample( Vec2f aImageCoord ) const
{	
	Vec2f shifted_aImageCoord;
	shifted_aImageCoord.x = aImageCoord.x - 0.5f;
	shifted_aImageCoord.y = aImageCoord.y -0.5f;
	int x0 = static_cast<int>(std::floor(shifted_aImageCoord.x));
	int y0 = static_cast<int>(std::floor(shifted_aImageCoord.y));

	float fx = shifted_aImageCoord.x - x0;
	float fy = shifted_aImageCoord.y - y0;

	int x1 = x0 + 1;
	int y1 = y0 + 1;

	int max_x = static_cast<int>(mSrcImage->get_width()) - 1;
	int max_y = static_cast<int>(mSrcImage->get_height()) - 1;

	int cx0 = std::clamp(x0, 0, max_x);
	int cx1 = std::clamp(x1, 0, max_x);
	int cy0 = std::clamp(y0, 0, max_y);
	int cy1 = std::clamp(y1, 0, max_y);

	//corners

	ColorU8_sRGB_Alpha bot_left = mSrcImage->get_pixel(cx0,cy0);
	ColorU8_sRGB_Alpha bot_right= mSrcImage->get_pixel(cx1, cy0);
	ColorU8_sRGB_Alpha top_left = mSrcImage->get_pixel(cx0, cy1);
	ColorU8_sRGB_Alpha top_right = mSrcImage->get_pixel(cx1, cy1);

	//calculatuiobnss
	ColorF bot_left_lin = linear_from_srgb(ColorU8_sRGB{bot_left.r, bot_left.g, bot_left.b});
	float bot_left_alph = bot_left.a/255.f;
	ColorF bot_right_lin = linear_from_srgb(ColorU8_sRGB{bot_right.r, bot_right.g, bot_right.b});
	float bot_right_alph = bot_right.a/255.f;
	ColorF top_left_lin = linear_from_srgb(ColorU8_sRGB{top_left.r, top_left.g, top_left.b});
	float top_left_alph = top_left.a/255.f;
	ColorF top_right_lin = linear_from_srgb(ColorU8_sRGB{top_right.r, top_right.g, top_right.b});
	float top_right_alph = top_right.a/255.f;


	//use the equation
	ColorF_Alpha bottom;
	bottom.r = (1.f - fx) * bot_left_lin.r + fx * bot_right_lin.r;
	bottom.g = (1.f - fx) * bot_left_lin.g + fx * bot_right_lin.g;
	bottom.b = (1.f - fx) * bot_left_lin.b + fx * bot_right_lin.b;
	bottom.a = (1.f - fx) * bot_left_alph + fx * bot_right_alph;

	ColorF_Alpha top;
	top.r = (1.f - fx) * top_left_lin.r + fx * top_right_lin.r;
	top.g = (1.f - fx) * top_left_lin.g + fx * top_right_lin.g;
	top.b = (1.f - fx) * top_left_lin.b + fx * top_right_lin.b;
	top.a = (1.f - fx) * top_left_alph + fx * top_right_alph;

	ColorF_Alpha out;
	out.r = (1.f - fy) * bottom.r + fy * top.r;
	out.g = (1.f - fy) * bottom.g + fy * top.g;
	out.b = (1.f - fy) * bottom.b + fy * top.b;
	out.a = (1.f - fy) * bottom.a + fy * top.a;
	

	return out; // TODO: implement me, ive implemenyted
}
