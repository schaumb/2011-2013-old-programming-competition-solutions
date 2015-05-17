#include <ios>
#define f(i)(i<19?.4/i-2.78:.07*i-4.06)*i
#define N(k)k>0&k<40&j<12-f(-(k-40))?j<58+f((k))?32:35:
main(){char x[73];for(int j=0,i=1;i<98;(++j%=73)?:++i+puts(x))x[j]=N(i)N(i-29)N(i-58)0;}