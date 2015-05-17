#include "object.hpp"
#include "helper.hpp"

v moveable::nextStep(GlobalState& s, bool forced){
	if(badCount % 5 == 0 ){prevPoss.clear();badCount++;}
	if(v(x,y)==route.front()){//Ha elértük a célt
		route.pop();
		real_route.push(v(x,y));
		prevPoss.clear();
		if(route.empty())return v(x,y);
	}
	auto vec = v(route.front().X ,route.front().Y); //irány vektor
	//double len = sqrt(pow(vec.X,2)+pow(vec.Y,2));
	v opt ; //; v( rint(vec.X/len), rint(vec.Y/len));
	
	
	int min = distance(v(x,y),vec);
	v min_opt = v(1,0);
	for(int i=0; i<6; ++i){
		if(distance(vec,v(x,y)+min_opt)<min){
			opt=min_opt;
			min = distance(vec,v(x,y)+opt);
		}
		min_opt = turn_left(min_opt);
	}
	v next = opt + v(x,y);
	
	
	if(!isPlaned && isFree(next,s,forced)){// ha eddig kerültünk megnézzük kell-e még, ha nem folytatjuk az utunk
		isPlaned=true;
		real_route.push(v(x,y));
		tr = [](v in){return in;};
		addToHistory(v(x,y));
		return next;
	}
	//A kis eltérés nem eltérés, próbáljuk átlóssan
	next = tr(opt)+v(x,y); // transzformációt használunk
	if(isFree(next,s,forced)){LOG1("ISFREE");}
	else if(isFree(v(x,y)+turn_left(tr(opt)),s,forced) ){//vigyázat kerülés közben is igaz lesz
		next = v(x,y)+turn_left(tr(opt));
	}
	else if(isFree(v(x,y)+turn_right(tr(opt)),s,forced) ){//vigyázat kerülés közben is igaz lesz
		next=v(x,y)+turn_right(tr(opt));
	}
	else{
		if(isPlaned){//TODO ha megint fordulni kell
			if(rand()>0.5){
				tr = [](v in){return turn_left(in);};
			}
			else{
				tr = [](v in){return turn_right(in);};
			}
			isPlaned = false;
		}
		int k;
		for(k=0; k<4 && !(isFree(next,s,forced)); ++k){//kipróbáljuk mind a 4 irányt
			next = (opt = tr(opt))+v(x,y);
		}
		if(k == 4 && route.size()>=2){// ha nem tudunk mozogni akkor leveszünk egyet a sorból ha van még benne cucc
			route.pop();
			return nextStep(s);
		}
		else if(k==4){// ez lehet túl merész TODO forced option
			next = v(x,y)+(opt = turn_left(opt));
			for(k = 0; k<4 && !(isFree(next,s,forced) ); ++k){//kipróbáljuk mind a 4 irányt
				next =(opt = tr(opt))+v(x,y);
			}
			if(k==4){
				LOG1("(anyilas)NEM TUDTUNK LÉPNI");
				badCount++;
				return v(x,y);
			}
		}
	}
	addToHistory(v(x,y));
	LOG3("LEP ",next.X, next.Y);
	return next;
}

void moveable::addToHistory(v x){
	prevPoss.push_back(x);
	if(prevPoss.size()>5)prevPoss.pop_front();//size of history
}

bool moveable::isFree(v pos,const GlobalState& st, bool isAttack) const{ // ellenörzi az összes objektumra nem-e vagyunk a tiltott sugarában
	
	if(!isAttack){
		FOREACHMAP(
			if(distance(i->second.Position(),pos)<R(i->second))return false;
		)
		for(auto it = prevPoss.begin();it!=prevPoss.end();++it){
			if(*it == pos) {
				return false;
			}
		}
	}
	else{
		FOREACHMAP(
			if(distance(i->second.Position(),pos)<1)return false;
		)
		if(prevPoss.back() == pos) {
			return false;
		}
	}
	if(isAttack)LOG1("BOSS mode");
	if(isAttack && st.boss!=0 && distance(st.boss->Position(),pos)<2){//A bosshoz támadáskor 3 közel megyünk
		LOG1("Closer to boss");
		return false;
	}
	else if(!isAttack && st.boss!=0 && distance(st.boss->Position(),pos)<R(*st.boss)){
		return false;
	}
	if(distance(st.base.Position(),pos)<R(st.base))return false;
	return true;
}
