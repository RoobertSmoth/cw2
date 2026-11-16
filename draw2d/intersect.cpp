#include <draw2d/intersect.hpp>

#include <algorithm>


bool intersect_circle_tri( Vec2f aCircleCenter, float aRadius, Vec2f aP0, Vec2f aP1, Vec2f aP2 ) noexcept
{	
	//lecture notes say:
	/*AABB: Axis aligned bounding box is simple
	circle triangle intersecion nice hit volumes
	closest point test
	- find closest point on each edge to circle center
		q1 ON EDGE ab
		q2 on edge bd
		q3 on edge da
	compute the squared distances from c to q1,q2,q3
	select min square distance d^2 min
	finally test if d^2 min <= r^2
	special case:
	- circle lie entirely inside triangle without intersecyoi any of edges
		- this case, all three distances may be greter than circle's radius
		- edge distance tests alone are insufficient
		
	slution: check whether circle centre lies inside triangle using a point in triangle test
		- half plane test*/

	//step 1 we gotta check if there is a circle in the center which doesnt help for the other intersection

	//gonna reuse the three point quick calc thing from draw
	Vec2f points[3] = {aP0, aP1, aP2};
	float points_cross_product[3];

	for (int pix = 0; pix < 3; pix++){
		Vec2f A = points[(pix + 1) % 3];
		Vec2f B = points[(pix + 2) % 3];
		points_cross_product[pix] = ((B.x - A.x)*(aCircleCenter.y - A.y)) - ((B.y - A.y)*(aCircleCenter.x - A.x));
	}

	//must have same sign
	if (((points_cross_product[0] >= 0 ) && (points_cross_product[1] >= 0 ) && (points_cross_product[2] >= 0 ))||((points_cross_product[0] <= 0 ) && (points_cross_product[1] <= 0 ) && (points_cross_product[2] <= 0 ))){
		return true;
		//collision
	}
	//else may be colliding with walls or not colliding
	//closest point time
	float t[3];
	Vec2f q[3];
	Vec2f dif[3];
	float dif_square[3];
	//mathy parameter to help find closest point
	for (int pix = 0; pix < 3; pix++){
		Vec2f U = points[(pix + 1) % 3];
		Vec2f V = points[(pix + 2) % 3];
		//incoming massive equation
		t[pix] = std::clamp((((aCircleCenter.x-U.x)*(V.x-U.x))+((aCircleCenter.y-U.y)*(V.y-U.y)))/(((V.x-U.x)*(V.x-U.x))+((V.y-U.y)*(V.y-U.y))),0.f,1.f);
		q[pix].x = U.x + t[pix] * (V.x - U.x);
		q[pix].y = U.y + t[pix] * (V.y - U.y);
		dif[pix].x = aCircleCenter.x - q[pix].x; 
		dif[pix].y = aCircleCenter.y - q[pix].y; 
		dif_square[pix] = (dif[pix].x * dif[pix].x) +   (dif[pix].y * dif[pix].y);
	}

	//now need top find d^2 min
	float d_min = std::min({dif_square[0],dif_square[1],dif_square[2]});
	if (d_min <= (aRadius*aRadius)){
		return true;
	}

	return false; // TODO: implement me
}

