
#include <iostream>
#include <vector>

void A( int* lil, int ili )
{
  int l, il;
  int i,ii, li;

  for( i = 0; i < ili; ++i )
  {
    l = lil[i];
    li = i;
    for( ii = i+1; ii < ili; ii++ )
    {
      if( l > lil[ii] ) { l = lil[ii]; li = ii; }
    }
    if( li != i )
    {
      il = lil[i];
      lil[i] = l;
      lil[li] = il;
    }
  }
}

void B( int* nmn, int mn )
{
  bool mm = true;
  int nn;
  int nm;

  int mnm = 0;
  while( true )
  {
    mm = true;
    for( nn = 0; nn < mn-1-mnm; ++nn )
    {
      if( nmn[nn] > nmn[nn+1] )
      {
        nm = nmn[nn];
        nmn[nn] = nmn[nn+1];
        nmn[nn+1] = nm;
        mm = false;
      }
    }
    if( mm ) break;
    ++mnm;
  }
}

void C(int* vv, int u, int vu)
{
  int uv = u, w = vu;
  int vw;
  int wv = vv[ abs((u + vu) / 2) ];

  while( uv <= w )
  {
    while( vv[uv] < wv ) uv++;
    while( vv[w] > wv ) w--;
    
    if( uv <= w )
    {
      vw = vv[uv];
      vv[uv++] = vv[w];
      vv[w--] = vw;
    }
  }

  if( u < w ) C( vv, u, w );
  if( uv < vu ) C( vv, uv, vu );
}


int main() {

	std::vector<int> v = {5, 2, 3, 5, 5, 9, -2, 30};
	std::cout << "O: "; for ( auto x : v ) { std::cout << x << ", "; } std::cout << std::endl;
	{ auto v2 = v; A( &v2[0], v.size() );
	std::cout << "A: "; for ( auto x : v2 ) { std::cout << x << ", "; } std::cout << std::endl; }
	{ auto v2 = v; B( &v2[0], v.size() );
	std::cout << "B: "; for ( auto x : v2 ) { std::cout << x << ", "; } std::cout << std::endl; }
	{ auto v2 = v; C( &v2[0], 0, v.size()-1 );
	std::cout << "C: "; for ( auto x : v2 ) { std::cout << x << ", "; } std::cout << std::endl; }
}

