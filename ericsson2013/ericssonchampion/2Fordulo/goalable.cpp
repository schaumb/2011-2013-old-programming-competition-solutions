#include "object.hpp"

using namespace std;

#define NEWROUTING 10

std::queue<v> goalable::getRout(){
	if(++query_counter % NEWROUTING != 0 && query_counter > 3){
		queue<v> q(thebest);
		return q;
	}
	else{
		queue<v> qu;
		qu.push(v(x,y));
		return qu;
	}
}
std::queue<v> goalable::getReversRout(){
	if(query_counter > 3){
		deque<v> d(thebest.rbegin(),thebest.rend());
		return queue<v>(d);
	}
	else{
		queue<v> qu;
		qu.push(thebest.front());
		return qu;
	}
}

void goalable::addHint(queue<v>& q){//mindig előbb mint a getReverseRout
	//if(g.front()!=st.base.Position()){return;}// ha nem jó hejről indultunk
	deque<v> deq;
	int d=0;
	v i;
	while(!q.empty()){
        i = q.front();
        deq.push_back(i);
        q.pop();
        if(!q.empty())
            d+=distance(i,q.front());
	}
	//Debug
		LOG2("(anyilas)HINT QUEUE",id);
		for(auto it = deq.begin();it!=deq.end();++it){
			LOG3(it->X,":",it->Y);
		}
		LOG1("END HINT QUEUE");
		//End debug
	if(theBestD > d){
		theBestD = d;
		thebest = deq;
		LOG1("KISEBB");
	}
}
