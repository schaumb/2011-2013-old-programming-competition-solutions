#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include <string>
#include <sstream>
#include <boost\math\constants\constants.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
namespace er{
typedef boost::geometry::model::polygon
	<
		boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> 
	> Polygon;
template<unsigned int n=256>
class Circle {
	const double twopibyn;
public:
	double x, y;
	unsigned r;
	Circle(double x, double y, unsigned r) : x(x), y(y), r(r), twopibyn(boost::math::constants::pi<double>()*2/n) { };
	std::string wkt() const {
		
		std::stringstream ss;
		ss<<std::fixed;
		ss<<"POLYGON((";
		ss<<x+r<<" "<<y;
		for(std::size_t i=1; i<n-1; ++i){
			ss	<<", "<< r* std::cos(twopibyn*i) + x 
				<<" " << r*	std::sin(twopibyn*i) + y;
		}
		ss<<"))";
		return ss.str();
	}
	operator Polygon() const {
		Polygon tmp;
		boost::geometry::read_wkt(wkt(), tmp);
		return tmp;
	}
		

};
} // namespace er
#endif // CIRCLE_HPP