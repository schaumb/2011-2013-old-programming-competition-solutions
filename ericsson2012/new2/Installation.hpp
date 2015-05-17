#ifndef INSTALLATION_HPP
#define INSTALLATION_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include "Circle.hpp"
#include "Util.hpp"
#include <utility>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/math/constants/constants.hpp>
#define DEV__
#ifdef DEV__
#include "create_svg.hpp"
#endif 
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
    
    Installation(const std::string& wkt) : boundaries() {
        read_wkt(wkt, boundaries);
        correct(boundaries);
        // calculate average distance between points.
    }
    template<unsigned T>
    void addAntenna(const er::Circle<T>& c){ // no I fucking can't get rid of the fucking <>s.
        Polygon tmp;
        read_wkt(c.wkt(), tmp);
        correct(tmp);
        antennas.push_back(std::make_pair(tmp, c.r));
    }
    void arrangeAntennas(const std::map<unsigned, unsigned>& inventory){
		
		Point c;
		centroid(boundaries, c);
		 std::vector<Point> pnts;
		double terulet = area(boundaries);
		ComplRoute myRoutes;
        //std::vector<Point> pnts = generateRoute(boundaries).toSortedPoints();
        double ker = perimeter(boundaries);
#ifdef DEV__
		std::cout << 4 * boost::math::constants::pi<double>() << " * " << terulet << " - " << ker*ker << "\n";
		std::cout << (4 * boost::math::constants::pi<double>() * terulet - ker*ker) << '\n';
#endif
		if(  std::abs((4 * boost::math::constants::pi<double>() * terulet - ker*ker) < 1 ) )
		{
			er::Circle<64> circ(c.get<0>(), c.get<1>(),  ker / ( 2 * boost::math::constants::pi<double>() ) );
			boundaries.clear();
			boundaries = circ;
			correct(boundaries);
#ifdef DEV__
			std::cout << "HIER";
#endif
		}
		if (num_points(boundaries) < 1000)
		{
			myRoutes = generateRoute(boundaries);
			pnts = myRoutes.toSortPoints();
		}
        unsigned long long sum_d = 0;
		std::vector<unsigned> radi;
        std::for_each(inventory.begin(), inventory.end(), [&radi](const std::pair<unsigned, unsigned> a){
            unsigned r=a.first;
            unsigned pcs=a.second;
			for(unsigned i=0;i<pcs; ++i)
				radi.push_back(r);
        });
		unsigned i = 0;
		std::sort(radi.rbegin(), radi.rend());
	
		
			
		std::cout << radi.size() << std::endl;

#ifdef DEV__
			addAntenna(Circle<>(c.get<0>(),c.get<1>(),radi[radi.size()-1]));
#endif		
		int centerrad = radi[radi.size()-1];
			std::cout << c.get<0>() <<' ' << c.get<1>() << ' ' << radi[radi.size()-1] << '\n';
			radi.pop_back();

		for(i = 0; radi.size() && i < pnts.size(); ++i )
		{
#ifdef DEV__
			addAntenna(Circle<>(pnts[i].get<0>(),pnts[i].get<1>(),radi[0]));
#endif
            std::cout<<pnts[i].get<0>()<<' '<< pnts[i].get<1>()<<' '<< radi[0]<< '\n';
			radi.erase(radi.begin());
		}
		
		auto it = boundaries.outer().begin();
		for(i = 0; radi.size() && it != boundaries.outer().end(); ++i , ++it)
		{
#ifdef DEV__
			addAntenna(Circle<>(it->get<0>(),it->get<1>(),radi[i]));
#endif
			std::cout << it->get<0>() + radi[radi.size()-1] * (c.get<0>() - it->get<0>()) / std::abs (distance (c,*it) ) <<' ' << it->get<1>() + radi[radi.size()-1] * (c.get<1>() - it->get<1>()) / std::abs (distance (c,*it) ) << ' ' << radi[radi.size()-1] << '\n';
			radi.pop_back();
		}
		
		double twopibyn(2*boost::math::constants::pi<double>()/radi.size());
		for(i = 0; i < radi.size(); ++i)
		{
#ifdef DEV__	
			addAntenna(Circle<>(c.get<0>(),c.get<1>(),radi[i]));
#endif
			
			std::cout << c.get<0>() + (centerrad + radi[i] )* std::cos(twopibyn*i) <<' ' << c.get<1>() + (centerrad + radi[i] )* std::sin(twopibyn*i) << ' ' << radi[i] << '\n';
		}
        
//		printLocations();
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
#ifdef DEV__
		model::multi_polygon<Polygon> ant;		
#endif

		std::for_each(antennas.begin(), antennas.end(), [
#ifdef DEV__
&ant
#endif
		](const Antenna& a){
            Point p;
            centroid(a.first, p);
         //   std::cout<<p.get<0>()<<" "<< p.get<1>()<<" "<< a.second<<"\n";
#ifdef DEV__
			ant.push_back(a.first);
#endif
        });
		std::cout<<std::endl;

		#ifdef DEV__
		create_svg("out.svg", boundaries, ant);
#endif
    }


};

} // namespace er
#endif // INSTALLATION_HPP
