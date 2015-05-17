
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

typedef std::vector<std::vector<double> > matrix;

void print(matrix m) {
	for ( int i = 0; i < 3; ++i ) {
		for ( int j = 0; j < 3; ++j ) {
			std::cout << m[i][j] << '\t';
		}
		std::cout << ( std::endl);
	}
}

matrix operator*(const matrix& m1, const matrix& m2) {
	matrix m = m1;
	const double& a = m1[0][0];
	const double& b = m1[0][1];
	const double& c = m1[0][2];

	const double& d = m1[1][0];
	const double& e = m1[1][1];
	const double& f = m1[1][2];

	const double& g = m1[2][0];
	const double& h = m1[2][1];
	const double& i = m1[2][2];

	const double& A = m2[0][0];
	const double& B = m2[0][1];
	const double& C = m2[0][2];

	const double& D = m2[1][0];
	const double& E = m2[1][1];
	const double& F = m2[1][2];

	const double& G = m2[2][0];
	const double& H = m2[2][1];
	const double& I = m2[2][2];


	m[0][0] = a*A + b*D + c*G;
	m[0][1] = a*B + b*E + c*H;
	m[0][2] = a*C + b*F + c*I;
  
	m[1][0] = d*A + e*D + f*G;
	m[1][1] = d*B + e*E + f*H;
	m[1][2] = d*C + e*F + f*I;
 
	m[2][0] = g*A + h*D + i*G;
	m[2][1] = g*B + h*E + i*H;
	m[2][2] = g*C + h*F + i*I;

	return m;
}

matrix inv(const matrix& n) {



	const double& a = n[0][0];
	const double& b = n[0][1];
	const double& c = n[0][2];

	const double& d = n[1][0];
	const double& e = n[1][1];
	const double& f = n[1][2];

	const double& g = n[2][0];
	const double& h = n[2][1];
	const double& i = n[2][2];
	if (a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g == 0) {
		throw 3;
	}
	matrix m = n;

	m[0][0] = (e*i - f*h)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
	m[0][1] = -(b*i - c*h)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
m[0][2] = (b*f - c*e)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
  
m[1][0] = -(d*i - f*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
m[1][1] = (a*i - c*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
m[1][2] = -(a*f - c*d)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
 
 m[2][0] = (d*h - e*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
 m[2][1] = -(a*h - b*g)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
 m[2][2] = (a*e - b*d)/(a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g);
		return m;
}


std::vector<double> fszzorzas(const matrix& m, const std::vector<double>& v) {
	std::vector<double> res(3);
	res[0] = v[0] * m[0][0] + m[0][1] * v[1] + m[0][2] * v[2];
	res[1] = v[1] * m[1][0] + m[1][1] * v[1] + m[1][2] * v[2];
	res[2] = v[2] * m[2][0] + m[2][1] * v[1] + m[2][2] * v[2];

	return res;
}


int main() {
	
	
	std::ifstream in("i.in");
	int tcs;
	in >> tcs;

	while ( tcs-- ) {
		matrix K(3, std::vector<double>(3, 0));
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ ) {
				in >> K[i][j];
			}
		}

		int bytes;
		in >> bytes;

		

		try {
			matrix invK = inv(K);


			if ( bytes % 3 != 0 ) {
				throw 6;
			}

			for ( int i = 0; i < bytes/3; ++i ) {
				std::vector<double> c(3);
				in >> c[0] >> c[1] >> c[2];
				std::vector<double> charsD = fszzorzas(invK, c);

				for ( int i = 0; i < 3; i++ ) {
					std::cout << int(charsD[0]) % 256 << std::endl;
				}

				std::vector<char> chars(3);
				chars[0] = int(charsD[0]) % 256;
				chars[1] = int(charsD[1]) % 256;
				chars[2] = int(charsD[2]) % 256;

				std::cout << chars[0] << chars[1] << chars[2];

			}

			std::cout << std::endl;
		} catch (int i) {
			std::cout << "cannot decrypt" << i << std::endl;
		}


	}

	std::cin.get();
}

