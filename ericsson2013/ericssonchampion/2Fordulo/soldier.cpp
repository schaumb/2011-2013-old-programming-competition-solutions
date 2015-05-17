#include "object.hpp"


using namespace std;

bool RouteIsFree(const GlobalState& st, v pos1, v pos2){
	auto vec = v(pos1.X - pos2.X,pos1.Y - pos2.Y); //irány vektor
	double len = sqrt(pow(vec.X,2)+pow(vec.Y,2));
	v dest = v( rint(vec.X/len), rint(vec.Y/len));
	for(v p = pos1;distance(p,pos2)>=3;p = p + dest){
		FOREACHMAP(
			if(distance(i->second.Position(),p)==0)return false;
		)
	}
	return true;
}

void Soldier::AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds){
	//belső állapotok: FOLLOWING,WAITING_BOSS, ATTACK_BOSS, SEARCH_BOSS
	
	//nézzük meg, hogy van-e valaki, akit támadhatunk
	
	vector<string> mins;
	vector<string> closer_mins;
	for(auto i = st.minions.begin();i != st.minions.end();++i){
		int d = distance(v(x,y),i->second.Position());
		if(d<=3){
			mins.push_back(i->first);
		}
		else if(d<=6){
			closer_mins.push_back(i->first);
		}
	}
	if((state != ATTACK_BOSS || rand()%4 == 0 ) && mins.size()>0){
		Attack(cmds,mins[rand()%mins.size()]);
		return;
	}
	
	
	//mozogjunk a stateeknek megfelelően
	switch (st.state) {
		case ATTACK:{
					//A katona csend esetén is agresszív
				}
		case QUIESCENCE:{
			if(closer_mins.size()!=0){
				route = queue<v>();
				route.push(st.minions[closer_mins[random()%closer_mins.size()]].Position());
				v n = nextStep(st);
				Move(cmds,n.X,n.Y);
				return;
			}
            break;
		}
		case BOSS:{//ha megvan a boss a követők bizonyos százaléka elmegy a bosshoz, a többiek biztosan
			if(state == FOLLOWING && rand()%60 == 0){
				if(buddy!=nullptr)buddy->UnGuard();
				route = queue<v>();
				state = WAITING_BOSS;
			}
			if(state == SEARCH_BOSS){
				route = queue<v>(); //waiting boss állapot előtt mindig 0zni kell a queuet
				state = WAITING_BOSS;
			}
			break;
		}
		case ATTACK_THE_BOSS:{//ha boss támadás van, akkor azok a katonák akik előtt szabad az út elindulnak.
			
			if(distance(st.boss->Position(),v(x,y))<=7 /* && RouteIsFree(st,v(x,y),st.boss->Position())*/){
				state = ATTACK_BOSS;
			}
			break;
		}
	}
	switch(state){
		case FOLLOWING:{//ha követünk
			if(buddy == 0){//ha még nincs kit
				//keresünk valakit
				auto i = st.workers.begin();
				for(; i != st.workers.end()&&i->second.IsGuarded();++i){LOG2("(anyilas)Ő nem az ",i->second.Id());}
				if(i != st.workers.end()){
					Guard(&st.workers[i->first]); //ha találunk dolgozót, követjük
					LOG2("(anyilas)FRIEND",i->first);
				}
				else{
					if(!st.base.IsGuarded()){
						Guard(&st.base);
						LOG2("(anyilas)FRIEND",i->first);
					}
					else{
						for(int x=0;x<10;++x){
							route.push(v(x,y)+v(rand()%12,rand()%12));
						}
							if(st.state>=BOSS)state=WAITING_BOSS;
							else state=SEARCH_BOSS; //ha nem akkor keresünk
							return;
					}
				}
			}
			if(!route.empty())route=queue<v>();
			route.push(buddy->Position()); //tegyük be a sorba a dolgozót
			//LOG3(id,"->>KÖVET ->>",buddy->Id());
			v n = nextStep(st); //kérjük le a következő lépést
			Move(cmds,n.X,n.Y);
            break;
		}
		case SEARCH_BOSS:{//TODO jól
			if(route.size()<=1){
				state = FOLLOWING;
			}
			v n = nextStep(st);
			Move(cmds,n.X,n.Y);
			break;
		}
		case WAITING_BOSS:{
			
			//use moveable
			if(route.empty()){//útvonal beállítása
				if(distance(v(x,y),st.base.Position())<2){//ha közel vagyunk a bázishoz, akkor használhatjuk a boss heurisztikáját
					route = st.boss->getRout();
					real_route.push(st.base.Position());
				}
				else{// ha nem akkor kerülő algoritmus használatával el kell jutni a boss-hoz
					route = queue<v>();
					route.push(st.boss->Position());
				}
			}
			
			v n = nextStep(st);//következő lépés
			// ha megérkeztünk addHint
			if(real_route.front() == st.base.Position() && distance(v(x,y),st.boss->Position())>7 && distance(n,st.boss->Position())<=7){
				real_route.push(st.boss->Position());
				if(real_route.front()==st.base.Position())st.boss->addHint(real_route);
				real_route = queue<v>();
			}
			
			// következő lépés
			UnLock();
			if(v(x,y)!=n&&((distance(v(x,y),st.boss->Position())>7)||(distance(v(x,y),st.boss->Position())<=7 && distance(n,st.boss->Position())<=7))){
				Move(cmds,n.X,n.Y);
			}
			else if (v(x,y)==n){
				Lock();
			}
			break;
		}
		case ATTACK_BOSS:{
			//use attack algorithm
			v pos1 = v(x,y);
			v pos2 = st.boss->Position();
			route = queue<v>();
			route.push(st.boss->Position());
			v n = nextStep(st,true);
			int distancex = distance(pos1,pos2);
			if(distancex>3 || (distancex==3 && distance(n,pos2) == 2)){
				/*
				auto vec = v(pos2.X - pos1.X,pos2.Y - pos1.Y); //irány vektor
				double len = sqrt(pow(vec.X,2)+pow(vec.Y,2));
				v dest = v( rint(vec.X/len), rint(vec.Y/len));
				v n = pos1+dest;
				*/
				Move(cmds,n.X,n.Y);
			}
			else{
				Attack(cmds,st.boss->Id());
			}
		}
	}
}



