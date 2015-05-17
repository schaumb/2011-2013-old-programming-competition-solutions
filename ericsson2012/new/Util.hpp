#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED


#include <vector>
#include <algorithm>
#include <numeric>
#include "Installation.hpp"
#include <utility>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/difference.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/algorithms/convert.hpp>


namespace er{

using namespace boost::geometry;
typedef model::point<double, 2, cs::cartesian> Point;
typedef model::polygon<Point> Polygon;
typedef std::pair<Polygon, unsigned> Antenna;
typedef model::ring<Point> Ring;

std::vector<unsigned> mapToVector(const std::map<unsigned, unsigned>& m)
{
		std::vector<unsigned> res;
		
		std::for_each(m.begin(), m.end(), [&res](const std::pair<unsigned, unsigned> a){
			res.resize(res.size()+ a.second, a.first);
		});
		std::sort(res.begin(), res.end());

		return res;
}


std::vector<Antenna> genPoints(const Polygon& c, const std::map<unsigned, unsigned>& m)
{
	
	std::vector<unsigned> r_s(mapToVector(m));
	
	std::vector<Antenna> res(r_s.size());
		
	
	std::vector<std::pair<Polygon, unsigned> > areas(1);
	areas[0] = std::make_pair( c, area(c) );
			
	for(unsigned i = 0; i < r_s.size(); ++i)
	{
		Point cent;
		centroid(areas[0].first, cent);
		
		Circle<> ant(cent.get<0>(), cent.get<1>(), r_s[i]);
		Polygon tmp;
		boost::geometry::read_wkt(ant.wkt(), tmp);

		res[i] = std::make_pair(tmp, r_s[i]);
		
		Polygon diff;
		difference(areas[0].first, tmp, diff);
		// areas[0].first - tmp, majd felosztas, kitorolni elsot, felosztasokat betenni, area-szerint sorba rendezni
	}
	
	return res;
}

/*

template<typename T, class U>
struct Suly
{
    T pont;
    U min;
    U max;
    Suly(){};
    Suly& operator()(const T& p, const U& mi, const U& ma)
    {
        pont = p;
        min = mi;
        max = ma;
        return *this;
    }
    Suly(const T& p, const U& mi, const U& ma):pont(p),min(mi), max(ma){};
    bool inRange(const U& i)
    {
        return min <= i && i >= max;
    }   
    bool operator<(const Suly<T,U>& right) const
    {
        return min < right.min;
    }
    bool operator==(const Suly<T,U>& right) const
    {
        return min == right.min;
    }
};
typedef Suly< Point, double > DefSuly;
typedef std::vector< DefSuly > Route;

struct ComplRoute
{
    std::vector< Route > __r;
    void push_back(const DefSuly& e)
    {
        __r[__r.size()-1].push_back(e);
    }
    void newLine()
    {
    //    if (__r[__r.size()-1].size() > 0)
    //        __r.push_back(Route());
    }
    ComplRoute():__r(1){};
    ~ComplRoute()
    {
        for (unsigned i = 0; i < __r.size(); ++i)
        {
#ifdef DEV__
            std::cout << "New Line \n";

            for(unsigned j = 0; j < __r[i].size(); ++j)
            {
                std::cout << __r[i][j].pont.get<0>() << " " << __r[i][j].pont.get<1>() << " " << __r[i][j].min << " " << __r[i][j].max << "\n";
            }
#endif //DEV__
        }
    }
    std::vector<Point> toSortedPoints()
    {
        std::vector<Point> pnts;
        std::sort(__r[0].rbegin(), __r[0].rend());
      for(unsigned i=0;i<__r.size();++i){
       for(unsigned j=0;j<__r[i].size(); ++j){
        pnts.push_back(__r[i][j].pont);
       }
      }
	  return pnts;
  }
};

template<typename T>
struct Mylist
{
    struct Node
    {
        T val;
        Node* next;
        Node* prev;
        Node(const T& v, Node* n, Node* p):val(v),next(n),prev(p){};    
        Node(const T& v):val(v),next(nullptr),prev(nullptr){};
        ~Node(){};
    };
    
    unsigned size;
    Node* f;
    
    Mylist():f(nullptr), size(0){};
    
    void push_back(const T& e)
    {
        
        if( !f ) { f = new Node (e); f->next = f->prev = f; }
        else 
        {
            f->prev = f->prev->next = new Node (e, f, f->prev);
        }
        ++size;
    }
    
    Mylist& erase(Node* a)
    {
        if( a && a -> prev == a )
        {
            delete f;
            --size;
            f = 0;
        }
        else if ( a )
        {
            f = a -> prev -> next = a -> next;
            a -> next -> prev = a -> prev;
            
            delete a;
            --size;
        }
        return *this;
    }
    
    ~Mylist()
    {
        while (f)
        {
            erase(f);
        }
    }
};



ComplRoute generateRoute(const Polygon& pol)
{
    Mylist<Point> l;
    
    std::for_each( pol.outer().begin() , pol.outer().end()-1, [&l](const Point& c) {
        l.push_back(c);
#ifdef DEV__
        std::cout << c.get<0>()  <<" " << c.get<1>() << "\n";
#endif //DEV__
    });
    
    ComplRoute myRoute;
    if (l.size>2)
    {
        Mylist<Point>::Node *akt1(l.f->prev), *akt2(l.f);
        
        while(l.size>2)
        {
            Mylist<Point>::Node* p(akt1->prev), *n(akt2->next);

            Polygon t1;
            append(t1, akt1->val);
            append(t1, akt2->val);
            append(t1, p->val);
            append(t1, akt1->val);
#ifdef DEV__
            std::cout << "\n\nQ";

             std::cout << boost::geometry::dsv(t1) << std::endl;
#endif //DEV__                         
            Point c1;
            centroid(t1,c1);
            double r1 = 2*area(t1)/perimeter(t1);
            double R1 = std::max( std::max( distance(c1, akt1->val) , distance(c1, akt2->val ) ), distance(c1, p->val) );
#ifdef DEV__           
            std::cout << c1.get<0>() << " " << c1.get<1>() << "\n";
            std::cout << r1 << " is  = 2*" << area(t1) << "/" <<  perimeter(t1)<< "\n";
            std::cout << R1 << " is  = max{" << distance(c1, akt1->val) << "," << distance(c1, akt2->val) << "," << distance(c1, p->val) << "}\n";
#endif //DEV__
            if( p == n )
            {
                myRoute.push_back( DefSuly( c1 , r1 , R1 ) );
                l.erase(akt1);
                akt1 = p;
                    
            }
            else
            {
                Polygon t2;
                append(t2, akt1->val);
                append(t2, akt2->val);
                append(t2, n->val);
                append(t2, akt1->val);
                Point c2;
                centroid(t2,c2);
                double r2 = 2*area(t2)/perimeter(t2);
                double R2 = std::max( std::max( distance(c2, akt1->val) , distance(c2, akt2->val ) ), distance(c2, n->val) );
#ifdef DEV__
                std::cout << boost::geometry::dsv(t2) << std::endl;
                std::cout << c2.get<0>() << " " << c2.get<1>() << "\n";
                std::cout << r2 << " is  = 2*" << area(t2) << "/" <<  perimeter(t2)<< "\n";
                std::cout << R2 << " is  = max{" << distance(c2, akt1->val) << "," << distance(c2, akt2->val) << "," << distance(c2, n->val) << "}\n";
#endif //DEV__            
                //dangerous!!
                
                model::multi_polygon<Polygon> ret1;
                intersection(pol, t1, ret1);
                bool good1 = r1 > 0 && area ( ret1 ) == area( t1 );
#ifdef DEV__         
                std::cout << area ( ret1 ) << " <-- " << area( t1 ) << " - 1st \n";
#endif //DEV__
                model::multi_polygon<Polygon> ret2;
                intersection(pol, t2, ret2);
                bool good2 = r2 > 0 && area ( ret2 ) == area( t2 );
#ifdef DEV__
                std::cout << area ( ret2 ) << " <-- " << area( t2 ) << " - 2st \n";
#endif //DEV__    
                
                if ( !good1 && !good2  )
                {
                    akt1 = akt1->next;
                    akt2 = akt2->next;
                    myRoute.newLine();
                }
                else if ( !good2 || (good1 && R2-r2 > R1-r1) )
                {
                    myRoute.push_back( DefSuly( c1 , r1, R1 ) );
                    l.erase(akt1);
                    akt1 = p;
                }
                else
                {
                    myRoute.push_back( DefSuly( c2 , r2, R2 ) );
                    l.erase(akt2);
                    akt2 = n;
                }
                
            }
            
            
        }
    }
    return myRoute;
}

*/



}

#endif //UTIL_HPP_INCLUDED