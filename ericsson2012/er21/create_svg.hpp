#ifndef CREATE_SVG_
#define CREATE_SVG_
#include <iostream>
#include <fstream>
#include <boost/assign.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/envelope.hpp>

#include <boost/geometry/extensions/io/svg/svg_mapper.hpp>

namespace er{

template <typename Geometry1>
void create_svg(std::string const& filename, Geometry1 const& a)
{
    typedef typename boost::geometry::point_type<Geometry1>::type point_type;
    std::ofstream svg(filename.c_str());

    boost::geometry::svg_mapper<point_type> mapper(svg, 400, 400);
    mapper.add(a);

    mapper.map(a, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
}

}


#endif 