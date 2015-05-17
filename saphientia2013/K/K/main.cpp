
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>


struct student {
	int id;
	int country;
	std::vector<int> friends;

	int crit1() const {
		return std::set<int>(friends.begin(), friends.end()).size();
	}
	int crit2() const {
		return std::count_if(friends.begin(), friends.end(), [&](int i) { return i != country; });
	}
	int crit3() const {
		return friends.size();
	}

	bool operator<(const student& o) const {
		if ( crit1() != o.crit1() ) {
			return crit1() < o.crit1();
		}
		if ( crit2() != o.crit2() ) {
			return crit2() < o.crit2();
		}
		if ( crit3() != o.crit3() ) {
			return crit3() < o.crit3();
		}
		return false;
	}
	
};

int main() {
	std::ifstream in("k.in");
	int n, m;
	in >> n >> m;

	std::vector<student> students(n);
	for ( unsigned i = 0; i < n; ++i ) {
		students[i].id = i+1;
		in >> students[i].country;
	}

	int i, j;
	while ( in >> i >> j ) {
		--i;--j;
		students[i].friends.push_back(students[j].country);
		students[j].friends.push_back(students[i].country);
	}

	std::sort(students.begin(), students.end());
	std::reverse(students.begin(), students.end());

	std::cout << students.front().id << std::endl;
	for ( int i = 1; i < students.size(); ++i ) {
		if ( students[i] < students.front() ) {
			break;
		}
		std::cout << students[i].id << std::endl;
	}

}

#if 0
struct student {
	
	int friendCount() const { return friends.size(); }
	int numberOfDiffCountry() const {
		std::set<int> cs;
		//std::cout << "====" << std::endl;
		for ( int i = 0; i < friends.size(); ++i ) {
			//std::cout << friends[i]->country << std::endl;
			cs.insert(friends[i]->country);
		}
		//std::cout << "----" << std::endl;
		return cs.size();
	}
	int diffFriendCount() const {
		int c = 0;
		for ( int i = 0; i < friends.size(); ++i ) {
			if ( country != friends[i]->country ) {
				++c;
			}
		}

		return c;
	}
	int id;
	int country;

	std::vector<student*> friends;

	bool operator<(const student& other) const {
		if ( numberOfDiffCountry() != other.numberOfDiffCountry() ) {
			return numberOfDiffCountry() < other.numberOfDiffCountry();
		}
		if ( diffFriendCount() != other.diffFriendCount() ) {
			return diffFriendCount() < other.diffFriendCount();
		}
		if ( friendCount() != other.friendCount() ) {
			return friendCount() < other.friendCount();
		}
	}
};

int main() {

	std::ifstream in("k.in");
	int c;
	in >> c;
	
	std::vector<student> students(c);
	for ( unsigned i = 0; i < c; ++i ) {
		students[i] = student();
		students[i].id = i+1;


		
		in >> students[i].country;
		std::cout << students[i].country << std::endl;
	}

	int i, j;
	while ( in >> i >> j ) {
		std::cout << i << ", " << j << std::endl;
		students[i-1].friends.push_back( &students[j-1] );
		students[j-1].friends.push_back( &students[i-1] );
	}

	for ( unsigned i = 0; i < c; ++i ) {
		std::cout << students[i].id << " : " << students[i].numberOfDiffCountry() << "; " << students[i].diffFriendCount() << "; " << students[i].friendCount() << "\n";
	}

	std::cout << std::max_element(students.begin(), students.end()) -> id << std::endl;
	std::cin.get();
	std::cin.get();

	std::cin.get();
	std::cin.get();
}


#endif