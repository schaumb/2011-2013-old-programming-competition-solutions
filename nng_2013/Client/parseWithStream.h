

#ifndef PARSEWITHSTREAM_HPP_
#define PARSEWITHSTREAM_HPP_

#include "stdafx.h"

template<class T1>
bool parseWithStream(const std::string& str, T1& t1) {
	std::stringstream ss(str);
	return (ss >> t1) && std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

template<class T1, class T2>
bool parseWithStream(const std::string& str, T1& t1, T2& t2) {
	std::stringstream ss(str);
	return (ss >> t1 >> t2) && std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

template<class T1, class T2, class T3>
bool parseWithStream(const std::string& str, T1& t1, T2& t2, T3& t3) {
	std::stringstream ss(str);
	return (ss >> t1 >> t2 >> t3) && std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

template<class T1, class T2, class T3, class T4>
bool parseWithStream(const std::string& str, T1& t1, T2& t2, T3& t3, T4& t4) {
	std::stringstream ss(str);
	return (ss >> t1 >> t2 >> t3 >> t4) && std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

template<class T1, class T2, class T3, class T4, class T5>
bool parseWithStream(const std::string& str, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) {
	std::stringstream ss(str);
	return (ss >> t1 >> t2 >> t3 >> t4 >> t5) && std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

#if 0

template<class... Ts>
bool parseWithStream(const std::string& str, Ts&...);

template<class... Ts>
bool parseWithStream(std::istream& ss, Ts&...);

namespace parse_impl {

inline
bool parseWithStream(std::istream& ss) {
    //Check if the remaning stream only contains blanks
    return std::istream_iterator<std::string>(ss) == std::istream_iterator<std::string>();
}

template<class Head, class... Ts>
bool parseWithStream(std::istream& ss, Head& head, Ts&... tail) {
    return ss >> head && parseWithStream( ss, tail... );
}

} //namespace parse_impl

template<class... Ts>
bool parseWithStream(const std::string& str, Ts&... tail) {
    std::stringstream ss(str);
    return parse_impl::parseWithStream( ss, tail... );
}

template<class... Ts>
bool parseWithStream(std::istream& ss, Ts&... tail) {
    return parse_impl::parseWithStream( ss, tail... );
}

#endif

#endif /* PARSEWITHSTREAM_HPP_ */
