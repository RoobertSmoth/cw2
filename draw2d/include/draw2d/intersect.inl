#include <algorithm>


inline
bool intersect_circle_aabb( Vec2f aCircleCenter, float aRadius, Vec2f aAABBMin, Vec2f aAABBMax ) noexcept
{	
	//ship is circle, asteroiuds aabb?
	//lecture says:
	/*AABB axis aligned bounding box
	ymax, ymin _> y,max, xmax
	ymin, xmin -> ymin,xmax
	circle c with radius r
	clamp(x,a,b) = min((max(x,a),b)), a <= b
	find clsest point q
	for circle-AABB we have:
	- Qx = clamp(Cx, xmin, xmax)
	- Qy = camp(Cy, ymin, ymax)
	finally check:
		||C-Q||^2 <= r^2*/

	Vec2f Q;
	Q.x = std::clamp(aCircleCenter.x,aAABBMin.x, aAABBMax.x);
	Q.y = std::clamp(aCircleCenter.y,aAABBMin.y, aAABBMax.y);

	Vec2f Dif;
	Dif.x = abs(aCircleCenter.x - Q.x);
	Dif.y = abs(aCircleCenter.y - Q.y);

	//magnitude but ^2 cancels out so no square root
	//dif <= r^2
	//true

	Vec2f Final;
	Final.x = Dif.x  * Dif.x;
	Final.y = Dif.y  * Dif.y;

	float sum = Final.x + Final.y;
	if (sum<=(aRadius*aRadius)){
		return true;
	}

	return false; // TODO: implement me
}

