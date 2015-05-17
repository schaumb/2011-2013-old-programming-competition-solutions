#include <map>

template<int (*f)(int)>
int cached_values(int x)
{
  static std::map< int, int > cache;
  std::map< int, int >::iterator y = cache.find(x);
  if( y != cache.end() ) return y->second;
  return cache[x] = f(x);
}

int Fibonacci( int n )
{
  if( n < 2 ) return n;
  return cached_values< Fibonacci >( n-1 ) 
       + cached_values< Fibonacci >( n-2 );
}

#include <iostream>

int main() {
	std::cout << Fibonacci(10) << std::endl;
}

