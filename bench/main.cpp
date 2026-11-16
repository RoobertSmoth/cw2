#include <nanobench.h>
#include <draw2d/image.hpp>
#include <draw2d/surface.hpp>
#include <draw2d/draw.hpp>
#include <draw2d/sampler.hpp>

int main()
{
	// This is essentially the example from https://nanobench.ankerl.com/
	//float f = 1.f;
	Surface surface( 1280, 720 );

	/*
	assignment 2.5 thing:
	- compare runtime performance of different ways of drawing/blitting image
		- blit_masked_subregion
		- blit_blend)subregion
		with the quad split into two triangles and drawn with draw_triangle_tex_masked
	consider only release version.
	compare 3 variants for at least 3 different image sizes
	- 32x32
	- 64x64
	- 512x512

	use same outpt framebuffer size for all tests, ensure that drawn image first onto output framebuffer (not clipped)
	all three methods of blitting/drawing must drqaw same images to be conmparable
	basiclaly control is: same image, same framebuffer size
	include code and test images for your benchmarks in my submission

	repor
	
	*/
	//make with make config=release_x64
	//run with ./bin/bench-release-x64-gcc.exe

	ankerl::nanobench::Bench b;
	b.minEpochIterations( 3000 ); //apprently helps with eliminating warnings about errors
	// You can configure things here.
	//images
	auto image32 =  load_image( "bench/32x32.png" );
	auto image64 =  load_image( "bench/64x64.png" );
	auto image512 =  load_image( "bench/512x512.png" );
	Sampler2D sampler32( *image32 );
	Sampler2D sampler64( *image64 );
	Sampler2D sampler512( *image512 );

	//blit_masked_subregion
	b.run( "blit_masked 32x32", [&] {
		blit_masked_subregion(surface, *image32, {0,0}, {32,32},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.minEpochIterations(1); //rest back to normal since this one was the only issue

	b.run( "blit_masked 64x64", [&] {
		blit_masked_subregion(surface, *image64, {0,0}, {64,64},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.run( "blit_masked 512x512", [&] {
		blit_masked_subregion(surface, *image512, {0,0}, {512,512},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	//blit_blend_subregion

	b.run( "blit_blend 32x32", [&] {
		blit_blend_subregion(surface, *image32, {0,0}, {32,32},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.run( "blit_blend 64x64", [&] {
		blit_blend_subregion(surface, *image64, {0,0}, {64,64},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.run( "blit_blend 512x512", [&] {
		blit_blend_subregion(surface, *image512, {0,0}, {512,512},{100.f,100.f});

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	//draw_triangle_tex_masked
	//Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, Sampler2D const& aSampler, Vec2f aT0, Vec2f aT1, Vec2f aT2 

	b.run( "draw_triangle_tex 32x32", [&] {
		draw_triangle_tex_masked(surface, { 100.f, 100.f }, { 132.f, 100.f }, { 132.f, 132.f },sampler32, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f });
		draw_triangle_tex_masked( surface, { 100.f, 100.f }, { 132.f, 132.f }, { 100.f, 132.f }, sampler32, { 0.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f } );

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.run( "draw_triangle_tex 64x64", [&] {
		draw_triangle_tex_masked(surface, { 100.f, 100.f }, { 164.f, 100.f }, { 164.f, 164.f },sampler64, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f });
		draw_triangle_tex_masked( surface, { 100.f, 100.f }, { 164.f, 164.f }, { 100.f, 164.f }, sampler64, { 0.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f } );

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	b.run( "draw_triangle_tex 512x512", [&] {
		draw_triangle_tex_masked(surface, { 100.f, 100.f }, { 612.f, 100.f }, { 612.f, 612.f },sampler512, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f });
		draw_triangle_tex_masked( surface, { 100.f, 100.f }, { 612.f, 612.f }, { 100.f, 612.f }, sampler512, { 0.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f } );

		// Without the following, the result in f is never used, and therefore
		// a compiler might optimize away the computations.
		ankerl::nanobench::doNotOptimizeAway( surface );
	} );

	
	return 0;
}
