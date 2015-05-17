#include "object.hpp"
#include "WorldStatusUpdate.pb.h"
#include "Commands.pb.h"
#include "ProtobufUtil.hh"



using namespace strategy_protocol;
using namespace protoutil;
using namespace std;

int step = 1;

void calcCMDS(const WorldStatusUpdateMessage& w, CommandsMessage& cmds, GlobalState &st){
	//Először frissítjük az objektumokat
	
	LOG1("begin read");
	//resource
	if(w.has_resource()){
		st.resource = w.resource();
	}
	
	if(w.has_resource()){
		st.myteam = w.myteam();
	}
	
	for(int i=0; i<w.bases_size(); ++i){
		if(w.bases(i).team()==st.myteam){
			st.base = w.bases(i);
		}
		else if (st.boss == NULL){
			st.boss = new Boss(w.bases(i));
		}
	}
	
	//workers
	for(int i=0; i<w.workers_size(); ++i){
		if(w.workers(i).team()==st.myteam){
			st.workers[w.workers(i).common().id()] = w.workers(i);
			st.workers[w.workers(i).common().id()].setRound(step);
		}
		else{
			st.minions[w.workers(i).common().id()] = w.workers(i);
			st.minions[w.workers(i).common().id()].setRound(step);
		}
	}
	
	//soldiers
	
	for(int i=0; i<w.soldiers_size(); ++i){
		if(w.soldiers(i).team()==st.myteam){
			st.soldiers[w.soldiers(i).common().id()] = w.soldiers(i);
			st.soldiers[w.soldiers(i).common().id()].setRound(step);
		}
		else{
			st.minions[w.soldiers(i).common().id()] = w.soldiers(i);
			st.minions[w.soldiers(i).common().id()].setRound(step);
		}
	}
	
	//mines
	
	for(int i=0; i<w.mines_size(); ++i){
		st.mines[w.mines(i).common().id()] = w.mines(i);
		st.mines[w.mines(i).common().id()].setRound(step);
	}
	
	LOG1("end read");
	LOG1("start update");
	
	//Ha van idő TODO ne keljen
	for(int i=0; i<w.obstacles_size(); ++i){
		st.obstacles[w.obstacles(i).common().id()] = w.obstacles(i).common();
	}
	//IDE jöhet a state globális módosításai
	int mySoldierCountNearBoss = 0;
	bool allSoldierNearIsLocked = true;
	if( st.boss!=0 )
	{
		for(const auto& it : st.soldiers){
			int dist = distance(it.second.Position(),st.boss->Position());
			if(dist < 9) 
			{
				allSoldierNearIsLocked&= it.second.IsLocked();
				++mySoldierCountNearBoss;
			}
		}
	}
	
    if(st.boss != NULL && st.state < BOSS)
	{
        st.state=BOSS;
	}
	else if(st.state == QUIESCENCE && st.minions.size()*0.75 > st.soldiers.size()){
		st.state = ATTACK;
	}
	else if(st.state == ATTACK && st.minions.size()*0.5 < st.soldiers.size()){
		st.state = QUIESCENCE;
	}
	else if(  st.state == BOSS &&( mySoldierCountNearBoss > 2 || ( allSoldierNearIsLocked && mySoldierCountNearBoss > 0 )  ))
	{
		st.state = ATTACK_THE_BOSS;
	}
	else if ( st.state == ATTACK_THE_BOSS  && mySoldierCountNearBoss == 0)
	{
		st.state = BOSS;
	}

	PRINTTABLE(st)

	//Minden comander objektumot megkérünk, hogy fűzzön hozzá parancsot
	st.base.AddToCommands(st,cmds);
	
	for(auto i = st.mines.begin(); i != st.mines.end(); ){
		if(i->second.Capacity() == 0){
			LOG2("EMPTY ",i->second.Id());
			st.mines.erase(i++);
		}
		else{
			++i;
		}
	}
	
	for(auto i = st.workers.begin(); i != st.workers.end(); ){
		if(i->second.getRound() == step){
			i->second.AddToCommands(st,cmds);
			++i;
		}
		else{
			LOG2("DIED ",i->second.Id());
			st.workers.erase(i++);
		}
	}
	
	for(auto i = st.soldiers.begin(); i != st.soldiers.end(); ){
		if(i->second.getRound() == step){
			i->second.AddToCommands(st,cmds);
			++i;
		}
		else{
			LOG2("DIED ",i->second.Id());
			st.soldiers.erase(i++);
		}
	}
	LOG1("end updating")
}

int main(){
	
	
	GlobalState st;
	st.state = QUIESCENCE;
	WorldStatusUpdateMessage world;
	//Elő Olvasunk, hogy a bázist beállítsuk
	readProtoFromStream(world, cin);
	st.base = world.bases(0);
	
	CommandsMessage commands;
	//Parancsok előállítása
	calcCMDS(world, commands, st);
	//Elküldjük a parancsokat
	writeProtoOnStream(commands, cout);
	while (cin.good()) {

		//Olvasunk
		readProtoFromStream(world, cin);

		//cerr << "Tick: " << step << endl;
		//cerr << "Proto content with DebugString" << endl;
		//cerr << world.DebugString() << endl;
		commander::reset();
		CommandsMessage commands;
		commander::AddDummyCommand(commands);
		//kiszámítjuk a parancsokat
		calcCMDS(world, commands, st);
		//Elküldjük a parancsokat
		writeProtoOnStream(commands, cout);

		++step;
		
		//PRINTTABLE(st)
		
	}
	return 0;
}
