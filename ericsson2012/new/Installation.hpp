#ifndef INSTALLATION_HPP
#define INSTALLATION_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include "Circle.hpp"
#include "Util.hpp"
#include <utility>
//#define DEV__
#ifdef DEV__
#include "create_svg.hpp"
#endif 

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/multi/geometries/multi_polygon.hpp>

namespace er{




using namespace boost::geometry;
typedef model::point<double, 2, cs::cartesian> Point;
typedef model::polygon<Point> Polygon;
typedef std::pair<Polygon, unsigned> Antenna;

class Installation{
private:
	Installation(const Installation& rhs); // singleton
public:
	Polygon boundaries;
	std::vector<Antenna> antennas;
	Installation() : boundaries() {
		boost::geometry::read_wkt(
			"POLYGON((250.0 114.0, 300.0 200.0, 250.0 286.0, 150.0 286.0, 200.0, 200.0, 150.0 114.0))",
			boundaries);
	};
	Installation(const std::string& wkt) : boundaries() {
		read_wkt(wkt, boundaries);
		correct(boundaries);
	}
	template<unsigned T>
	void addAntenna(const er::Circle<T>& c){ // no I fucking can't get rid of the fucking <>s.
		Polygon tmp;
		read_wkt(c.wkt(), tmp);
		correct(tmp);
		antennas.push_back(std::make_pair(tmp, c.r));
	}
	void arrangeAntennas(const std::map<unsigned, unsigned>& inventory){
		
		antennas = genPoints(boundaries, inventory);
		/*
		std::vector<Point> pnts = generateRoute(boundaries).toSortedPoints();
		Point c;
		centroid(boundaries, c);
		pnts.insert(pnts.begin(), c);

		unsigned i=0;
		bool b=false;
		std::for_each(inventory.rbegin(), inventory.rend(), [&i, this, &pnts](const std::pair<unsigned, unsigned> a){
			unsigned r=a.first;
			unsigned pcs=a.second;
			
			for(unsigned j=0;j<pcs && i<pnts.size(); ++j){
				addAntenna(Circle<>(pnts[i].get<0>(), pnts[i].get<1>(), r));
				++i;					
			}
		});
		
		*/
	}
	double islandArea() const {
		return area(boundaries);
	}
	double theoreticalCoverage() const {
		double sum=0.0;
		std::for_each(antennas.begin(), antennas.end(), [&sum](const Antenna& a){
			sum+=area(a.first);
		});
		return sum/islandArea();
	}
	double currentlyCoveredArea() const {
		model::multi_polygon<Polygon> coveredArea;
		std::for_each(antennas.begin(), antennas.end(), [this, &coveredArea](const Antenna a){
			model::multi_polygon<Polygon> tmp;
			union_(coveredArea, a.first, tmp);
			assign(coveredArea, tmp);
		});
		model::multi_polygon<Polygon> ret;
		intersection(coveredArea, boundaries, ret);
		return area(ret);
	}
	double currentCoverage() const {
		return currentlyCoveredArea() / islandArea();
	}
	void printLocations() const {
		std::cout<<std::fixed;
		std::cout<<antennas.size()<<std::endl;
		std::for_each(antennas.begin(), antennas.end(), [](const Antenna& a){
			Point p;
			centroid(a.first, p);
			std::cout<<p.get<0>()<<" "<< p.get<1>()<<" "<< a.second<<std::endl;
		});
	}
#ifdef DEV__
	void makeSVG() const {
		model::multi_polygon<Polygon> antennas2;
		std::for_each(antennas.begin(), antennas.end(), [&antennas2](const Antenna a){
			antennas2.push_back(a.first);
		});
		er::create_svg("out.svg", boundaries, antennas2);
	}
#endif

};

} // namespace er
#endif // INSTALLATION_HPP