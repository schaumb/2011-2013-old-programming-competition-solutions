#include <iostream>
#include <iomanip>
#include "Circle.hpp"
#include "Installation.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <future>
#include <sstream>

#define DEV__
#include <map>
#ifdef DEV__
#include "create_svg.hpp"
#endif

int main(){
	//er::Circle<64> c(10, 5, 1);
	//std::cout<<c.wkt()<<std::endl;
	unsigned N, M;
	std::cin>>N>>M;
	std::map<unsigned, unsigned > inventory;
	for(unsigned i=0; i<N; ++i){
		unsigned r, pcs;
		std::cin >> r >> pcs;
		inventory[r] = pcs;
	} // anntennas acquired!
	// get boundaries, create wkt from them, then create our installation!
	std::stringstream ss;
	ss<<"POLYGON((";
	double x,y;
	std::cin>>x>>y;
	ss<<x<<" "<<y;
	for(unsigned i=1; i<M; ++i){
		// read two doubles for point.
		std::cin>>x>>y;
		ss<<", "<<x<<" "<<y;
	}
	ss<<"))"; //finished wkt for island shape.
	
//	er::Circle<1000> c(0, 0, 20);
//	er::Installation solution(c.wkt());
	er::Installation solution(ss.str());
	
	solution.arrangeAntennas(inventory);
#ifdef DEV__
	std::cout<<"Created island with area: "<<solution.islandArea()<<std::endl;
	std::cout<<"Theoretical coverage: "<<solution.theoreticalCoverage()*100<<"%"<<std::endl;
	std::cout<<"Effective coverage: "<<solution.currentCoverage()*100<<"%"<<std::endl;
	std::cout<<"Tower locations: "<<std::endl;
#endif // DEV__
//	solution.printLocations();
	return 0;
}