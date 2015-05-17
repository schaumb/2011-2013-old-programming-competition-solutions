#include <iostream>
#include <iomanip>
#include "Circle.hpp"
#include "Installation.hpp"
#include "util.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>


#include <map>



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
	er::Installation solution(ss.str());
	solution.arrangeAntennas(inventory);
	
	std::cout<<"Created island with area: "<<solution.islandArea()<<std::endl;
	std::cout<<"Theoretical coverage: "<<solution.theoreticalCoverage() * 100<< '%' <<std::endl;
	std::cout<<"Effective coverage: "<<solution.currentCoverage() * 100<< '%'<<std::endl;
	std::cout<<"Tower locations: "<<std::endl;
	solution.printLocations();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	return 0;
}