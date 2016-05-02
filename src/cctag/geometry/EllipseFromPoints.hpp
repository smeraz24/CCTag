#ifndef _CCTAG_NUMERICAL_ELLIPSEFROMPOINTS_HPP_
#define _CCTAG_NUMERICAL_ELLIPSEFROMPOINTS_HPP_

#include <cctag/geometry/Ellipse.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types_c.h>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <cstddef>
#include <cmath>
#include <vector>


namespace cctag {
namespace numerical {
namespace geometry {

Point2d<Eigen::Vector3f> extractEllipsePointAtAngle( const Ellipse & ellipse, float theta );
///@todo rename this function
void points( const Ellipse & ellipse, const std::size_t nb, std::vector< cctag::Point2d<Eigen::Vector3f> > & pts );
///@todo rename this function
void points( const Ellipse & ellipse, const std::size_t nb, const float phi1, const float phi2, std::vector< cctag::Point2d<Eigen::Vector3f> > & pts );

template <class T>
void fitEllipse(const std::vector<cctag::Point2d<T>> & points, Ellipse& e )
{
	std::vector<cv::Point2f> cvpts;
	cvpts.reserve( points.size() );
        for (const auto& pt : points)
		cvpts.push_back( cv::Point2f(pt.x(), pt.y()));
        
	const cv::RotatedRect rR = cv::fitEllipse( cv::Mat( cvpts ) );
	const float xC           = rR.center.x;
	const float yC           = rR.center.y;

	const float b = rR.size.height / 2.0;
	const float a = rR.size.width / 2.0;

	const float angle = rR.angle * boost::math::constants::pi<float>() / 180.0;

	e.setParameters( Point2d<Eigen::Vector3f>( xC, yC ), a, b, angle );
}

void ellipsePoint( const cctag::numerical::geometry::Ellipse& ellipse, float theta, Eigen::Vector3f& pt );


void computeIntermediatePoints(const Ellipse & ellipse, Point2d<Eigen::Vector3i> & pt11, Point2d<Eigen::Vector3i> & pt12, Point2d<Eigen::Vector3i> & pt21, Point2d<Eigen::Vector3i> & pt22);
void rasterizeEllipticalArc(const Ellipse & ellipse, const Point2d<Eigen::Vector3i> & pt1, const Point2d<Eigen::Vector3i> & pt2, std::vector< Point2d<Eigen::Vector3i> > & vPoint, std::size_t intersectionIndex);
void rasterizeEllipse( const Ellipse & ellipse, std::vector< Point2d<Eigen::Vector3i> > & vPoint );

/**
 * Get the pixel perimeter of an ellipse
 * @param ellipse
 * @return the perimeter in number of pixels
 */
std::size_t rasterizeEllipsePerimeter( const Ellipse & ellipse );

/**
 * @brief Compute intersections if there, between a line of equation Y = y and an ellipse.
 *
 * @param[in] ellipse
 * @param[in] y ordonate for which we compute x values of intersections
 * @return intersected points sorted in ascend order (returns x coordinates: 0, 1, or 2 points).
 */
std::vector<float> intersectEllipseWithLine( const numerical::geometry::Ellipse& ellipse, const float y, bool horizontal);

}
}
}

#endif

