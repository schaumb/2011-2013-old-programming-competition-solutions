#include <cmath>


#ifndef HELPER__HPP

#define HELPER__HPP

#define X first
#define Y second
typedef std::pair<int,int> v;
inline v operator+(v a,v b){return v(a.X+b.X,a.Y+b.Y);}

inline int distance(v a, v b){
	return std::max( std::max( abs( a.X - b.X ) , abs( a.Y - b.Y ) ) ,  abs( ( a.X + a.Y ) - ( b.X + b.Y ) ) );
}

inline v turn_left(v x){
	static const v dirs[] = {v(0,-1), v(-1,0), v(-1,1), v(0,1), v(1,0), v(1,-1)}; //óramutatóval ellentétes sorrendben
	int i;
	for(i=0;i<6 && dirs[i]!= x ; ++i){}
	return dirs[(i+1)%6];
}

inline v turn_right(v x){
	static const v dirs[] = {v(0,-1), v(-1,0), v(-1,1), v(0,1), v(1,0), v(1,-1)}; //óramutatóval ellentétes sorrendben
	int i;
	for(i=0;i<6 && dirs[i]!= x ; ++i){}
	return dirs[(i-1)%6];
}

#define FOREACHMAP(b) for(auto i = st.mines.begin(); i != st.mines.end();++i){ b }\
	for(auto i = st.workers.begin(); i != st.workers.end();++i){b}\
	for(auto i = st.soldiers.begin(); i != st.soldiers.end();++i){b}\
	for(auto i = st.minions.begin(); i != st.minions.end();++i){b}\
	for(auto i = st.obstacles.begin(); i != st.obstacles.end();++i){b}

#define FOREACHMOVEABLE(b)	for(auto i = st.workers.begin(); i != st.workers.end();++i){b}\
	for(auto i = st.soldiers.begin(); i != st.soldiers.end();++i){b}


//#define DEBUG_T
//#define DEBUG_LOG

#ifdef DEBUG_LOG
	#define LOG1(v1) std::cerr<<v1<<std::endl;
	#define LOG2(v1,v2) std::cerr<<v1<<v2<<std::endl;
	#define LOG3(v1,v2,v3) std::cerr<<v1<<v2<<v3<<std::endl;
	#define LOG4(v1,v2,v3,v4) std::cerr<<v1<<v2<<v3<<v4<<std::endl;
#else
	#define LOG1(v1)
	#define LOG2(v1,v2)
	#define LOG3(v1,v2,v3)
	#define LOG4(v1,v2,v3,v4)
#endif



#ifdef DEBUG_T 

#define PRINTTABLE(st) std::string tab[50];\
	for(int l=0; l<50 ; ++l ){\
		tab[l] = std::string(50,'.');\
		for(int i=l%2; i < 50;i=+2){\
			tab[l][i]=' ';\
		}\
	}\
	for(auto i = st.mines.begin(); i != st.mines.end();++i){\
		int x = i->second.Position().X*2;\
		int y = i->second.Position().Y*2;\
		x = y%2 ==0? x-1: x;\
		if(y<50&&x<50)tab[x][y]='M';\
	}\
	for(auto i = st.workers.begin(); i != st.workers.end();++i){\
		int x = i->second.Position().X*2;\
		int y = i->second.Position().Y*2;\
		x = y%2 ==0? x-1: x;\
		if(y<50&&x<50)tab[x][y]=i->second.State()==Worker::WORKING?'W':'3';\
	}\
	for(auto i = st.soldiers.begin(); i != st.soldiers.end();++i){\
		int x = i->second.Position().X*2;\
		int y = i->second.Position().Y*2;\
		x = y%2 ==0? x-1: x;\
		char c = 'F';\
		switch (i->second.State()){\
			case Soldier::SEARCH_BOSS:{\
				c = 'S';\
				break;\
			}\
			case Soldier::WAITING_BOSS:{\
				c = 'a';\
				break;\
			}\
			case Soldier::ATTACK_BOSS:{\
				c = 'A';\
				break;\
			}\
			case Soldier::FOLLOWING:{\
                c = 'F';\
				break;\
			}\
		}\
		if(y<100&&x<100)tab[x][y]=c;\
	}\
	for(auto i = st.minions.begin(); i != st.minions.end();++i){\
		int x = i->second.Position().X;\
		int y = i->second.Position().Y;\
		x = y%2 ==0? x-1: x;\
		if(y<100&&x<100)tab[x][y]='E';\
	}\
	for(auto i = st.obstacles.begin(); i != st.obstacles.end();++i){\
		int x = i->second.Position().X;\
		int y = i->second.Position().Y;\
		x = y%2 ==0? x-1: x;\
		if(y<100&&x<100)tab[x][y]='O';\
	}\
	if(st.boss!=0){\
		int x=st.boss->Position().X;\
		int y = st.boss->Position().Y;\
		x = y%2 ==0? x-1: x;\
		if(y<100&&x<100)tab[x][y]='B';\
	}\
	int x=st.base.Position().X;\
	int y=st.base.Position().Y;\
	x = y%2 ==0? x-1: x;\
	if(y<100&&x<100)tab[x][y]='b';\
	for(int l=0; l<25 ; ++l ){\
		std::cerr<<tab[l]<<std::endl;\
	}
	
#else
	#define PRINTTABLE(st)
#endif





#endif 


