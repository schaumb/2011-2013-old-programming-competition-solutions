
#include <boost/test/unit_test.hpp>

#include "diamond.hpp"

#include <initializer_list>
#include <algorithm>

bool checkEquality(const std::string& input, std::initializer_list<int> expected) {
	std::vector<int> diamondCount = countDiamonds(input);
	return diamondCount.size() == expected.size() && std::equal(diamondCount.begin(), diamondCount.end(), expected.begin());
}

BOOST_AUTO_TEST_SUITE( diamondTest )

BOOST_AUTO_TEST_CASE( example1works ) { BOOST_CHECK( checkEquality("ii", {1} ) ); }
BOOST_AUTO_TEST_CASE( example2works ) { BOOST_CHECK( checkEquality("iiii3", {6} ) ); }
BOOST_AUTO_TEST_CASE( example3works ) { BOOST_CHECK( checkEquality("ii.ii10", {1} ) ); }
BOOST_AUTO_TEST_CASE( example4works ) { BOOST_CHECK( checkEquality("ii.ii2", {3} ) ); }
BOOST_AUTO_TEST_CASE( example5works ) { BOOST_CHECK( checkEquality("iiIii", {1, 1} ) ); }
BOOST_AUTO_TEST_CASE( example6works ) { BOOST_CHECK( checkEquality("iiiiI10", {2, 0} ) ); }
BOOST_AUTO_TEST_CASE( example7works ) { BOOST_CHECK( checkEquality("ii\nii", {2}  ) ); }
BOOST_AUTO_TEST_CASE( example8works ) { BOOST_CHECK( checkEquality("iii\niii", {3}  ) ); }
BOOST_AUTO_TEST_CASE( example9works ) { BOOST_CHECK( checkEquality("lii", {0} ) ); }
BOOST_AUTO_TEST_CASE( example10works ) { BOOST_CHECK( checkEquality("liii", {1} ) ); }
BOOST_AUTO_TEST_CASE( example11works ) { BOOST_CHECK( checkEquality("iiii10", {0} ) ); }
BOOST_AUTO_TEST_CASE( example12works ) { BOOST_CHECK( checkEquality("ii.ii", {2} ) ); }
BOOST_AUTO_TEST_CASE( example13works ) { BOOST_CHECK( checkEquality("iiliii5", {6}  ) ); }
BOOST_AUTO_TEST_CASE( example14works ) { BOOST_CHECK( checkEquality("iiii010", {20} ) ); }
BOOST_AUTO_TEST_CASE( example15works ) { BOOST_CHECK( checkEquality("il.ii", {1}  ) ); }
BOOST_AUTO_TEST_CASE( example16works ) { BOOST_CHECK( checkEquality("iii50", {1}  ) ); }

BOOST_AUTO_TEST_SUITE_END()

