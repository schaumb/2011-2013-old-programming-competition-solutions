#include <iostream>

#ifndef _H_OBJECT
#define _H_OBJECT

#include "WorldStatusUpdate.pb.h"
#include "Commands.pb.h"
#include "ProtobufUtil.hh"
#include "helper.hpp"
#include <map>
#include <math.h>
#include <stdlib.h>
#include <tuple>
#include <queue>
#include <deque>
#include <limits>

struct GlobalState;

class Soldier;

class commander{
public:
	virtual void AddToCommands(GlobalState& st,strategy_protocol::Commands& cmds)=0;
	
protected:
	void TrainWorker(strategy_protocol::Commands& cmds,const  std::string& id){
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::Commands::TRAIN);
		auto train = command->mutable_traincommand();
		train->set_withwhat(id);
		train->set_what(strategy_protocol::Commands::WORKER);
	}
	void TrainSoldier(strategy_protocol::Commands& cmds,const  std::string& id){
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::Commands::TRAIN);
		auto train = command->mutable_traincommand();
		train->set_withwhat(id);
		train->set_what(strategy_protocol::Commands::SOLDIER);
	}
	void Move(strategy_protocol::Commands& cmds,int x, int y, const std::string& id){
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::Commands::MOVE);
		auto move = command->mutable_movecommand();
		move->set_withwhat(id);
		auto pos = move->mutable_toposition();
		pos->set_x(x);
		pos->set_y(y);
		
	}
	void Attack(strategy_protocol::Commands& cmds,const std::string& what,const std::string& id){
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::Commands::ATTACK);
		auto attack = command->mutable_attackcommand();
		attack-> set_withwhat(id);
		attack-> set_what(what);
	}
};

class object {
public:
	object(){id = "undefined";}
	object(const strategy_protocol::WorldStatusUpdate_CommonProperties& cp){
		id = cp.id();
		x = cp.position().x();
		y = cp.position().y();
	}
	void setCommon(const ::strategy_protocol::WorldStatusUpdate_CommonProperties& cp){
		id = cp.id();
		x = cp.position().x();
		y = cp.position().y();
	}
	virtual void setRound(int r){round = r;}
	virtual int getRound(){return round;}
	
	v Position()const{return v(x,y);}
	virtual int RadiusW(){return 1;}
	virtual int RadiusS(){return 1;}
    std::string Id(){return id;}
protected:
	std::string id;
	int x;
	int y;
    int radius;
    int round;
};

class dynamic_object :public object{
public:
	dynamic_object(){}
	dynamic_object(const strategy_protocol::WorldStatusUpdate_CommonProperties& cp):object(cp){};
	int Health() const {return health;}
protected:
	int health;
};

class moveable : public dynamic_object{
public:
	moveable(){
		tr = [](v in){return in;};
	}
	moveable(const strategy_protocol::WorldStatusUpdate_CommonProperties& cp):dynamic_object(cp){
		tr = [](v in){return in;};
	}
	virtual int R(object o) const = 0;
	std::queue<v> RealRoute(){return real_route;}
	void SetRealRoute(std::queue<v> r) {route=r;}
protected:
	std::queue<v> route;
	v nextStep(GlobalState& s);
	bool isPlaned; //az út a tervezett szerint halad-e
	std::queue<v> real_route;
	bool isFree(v pos,const GlobalState& s) const;
	std::function<v(v)> tr;
	v prevPos;
};

class goalable:public dynamic_object{
public:
	goalable(){}
	goalable(const strategy_protocol::WorldStatusUpdate_CommonProperties& cp):dynamic_object(cp){}
	std::queue<v> getRout();
	std::queue<v> getReversRout();
	void addHint(std::queue<v>& q);
protected:
	int query_counter = 0;
	std::deque<v> thebest;
	int theBestD = INT_MAX;
};

class Mine : public goalable{
public:
	Mine(){}
	Mine(const strategy_protocol::WorldStatusUpdate_Mine& m):goalable(m.common()),capacity(m.capacity()){};
	Mine& operator=(const strategy_protocol::WorldStatusUpdate_Mine& m){
		setCommon(m.common());
		capacity= m.capacity();
		return *this;
	}
	int Capacity() const {return capacity;}
	int RadiusW(){return 0;}
	int RadiusS(){return 3;}
    void AddWorker(){++worker_counter;}
    void DieWorker(){--worker_counter;}
    int CanAddWorker()const{return theBestD/15 < worker_counter;}
private:
	int capacity;
    int worker_counter = 0;
};

class Worker:public moveable,public commander{
public:
	enum state_type{SEARCH_MINE, WORKING};
	Worker(){}
	Worker(const strategy_protocol::WorldStatusUpdate_Worker& w):moveable(w.common()),resource(w.resource()){
        health=w.health();
        state=SEARCH_MINE;
        guarded=false;
        buddy=NULL;
        mine=NULL;
    }
    ~Worker(){
        if(mine!=NULL)
            mine->DieWorker();
    }
	Worker& operator=(const strategy_protocol::WorldStatusUpdate_Worker& w){
		setCommon(w.common());
		health=w.health();
		resource=w.resource();
		return *this;
	}
	int Resource() const {return resource;}
	int R(object o) const {return o.RadiusW();}
	int RadiusW(){return 1;}
	int RadiusS(){return 2;}
    state_type State(){return state;}
    void State(state_type new_state){state=new_state;}
    void Guard(Soldier* b){
        guarded=true;
        buddy=b;
    }
    void UnGuard(){
        guarded=false;
        buddy=NULL;
    }
    bool IsGuarded(){return guarded;}
    Soldier* Buddy(){return buddy;}
    void AddMine(Mine* m){
        mine=m;
        m->AddWorker();
    } 
    void RemoveMine(){mine=NULL;}
    void AddToCommands(GlobalState& st,strategy_protocol::Commands& cmds);
private:
	int resource;
    state_type state;
    Soldier* buddy;
    Mine* mine;
    bool guarded;
	void Move(strategy_protocol::Commands& cmds,int toX, int toY){commander::Move(cmds,toX,toY,id);x=toX;y=toY;}
	void Attack(strategy_protocol::Commands& cmds,const std::string& what){commander::Attack(cmds,what,id);}
};

class Soldier:public moveable,public commander{
public:
	enum state_type{FOLLOWING,GUARD,WAITING_BOSS, ATTACK_BOSS, SEARCH_BOSS};
	Soldier(){}
	Soldier(const strategy_protocol::WorldStatusUpdate_Soldier& w):moveable(w.common()){
        health=w.health();
        state=SEARCH_BOSS;
        buddy=NULL;
    }
    ~Soldier(){
       if(buddy!=NULL)
           buddy->UnGuard();
    }
	Soldier& operator=(const strategy_protocol::WorldStatusUpdate_Soldier& w){
		setCommon(w.common());
		health= w.health();
		return *this;
	}
	int R(object o)const{return o.RadiusS();}
	int RadiusW(){return 1;}
	int RadiusS(){return 1;}
    state_type State(){return state;}
    void State(state_type new_state){state=new_state;}
    void Guard(Worker* w){
        buddy=w;
        state=FOLLOWING;
        buddy->Guard(this);
    }
    void AddToCommands(GlobalState& st,strategy_protocol::Commands& cmds);
private:
    state_type state;	
    Worker* buddy;
	void Move(strategy_protocol::Commands& cmds,int toX, int toY){commander::Move(cmds,toX,toY,id);x=toX;y=toY;}
	void Attack(strategy_protocol::Commands& cmds,const std::string& what){commander::Attack(cmds,what,id);}
};

class Boss:public goalable{
public:
	Boss(const strategy_protocol::WorldStatusUpdate_Boss& b):goalable(b.common()){health=b.health();}
	Boss& operator=(const strategy_protocol::WorldStatusUpdate_Boss& w){
		setCommon(w.common());
		health= w.health();
		return *this;
	}

	int RadiusW(){return 6;}
	int RadiusS(){return 6;}
private:

};

//Ez nem biztos, hogy kell
class Minion:public dynamic_object{
public:
	Minion(){}
	Minion(const strategy_protocol::WorldStatusUpdate_Minion& m):dynamic_object(m.common()){health=m.health();}
	Minion& operator=(const strategy_protocol::WorldStatusUpdate_Minion& w){
		setCommon(w.common());
		health= w.health();
		return *this;
	}
	int RadiusW(){return 4;}
	int RadiusS(){return 2;}
private:
};

class Base:public dynamic_object,public commander{
public:
	Base(){}
	Base(const strategy_protocol::WorldStatusUpdate_Base& b):dynamic_object(b.common()){health=b.health();}
	Base& operator=(const strategy_protocol::WorldStatusUpdate_Base& b){
		setCommon(b.common());
		health= b.health();
		return *this;
	}
	int RadiusW(){return 1;}
	int RadiusS(){return 3;}
	void AddToCommands(GlobalState& st,strategy_protocol::Commands& cmds);
private:
	void TrainWorker(strategy_protocol::Commands& cmds){commander::TrainWorker(cmds,id);}
	void TrainSoldier(strategy_protocol::Commands& cmds){commander::TrainSoldier(cmds,id);}
};

enum strategy_state{QUIESCENCE,ATTACK,BOSS,ATTACK_THE_BOSS};



struct GlobalState{
	strategy_state state;
	Boss* boss;
	int resource;
	Base base;

	std::map<std::string,Worker> workers;
	std::map<std::string,Soldier> soldiers;
	std::map<std::string,Mine> mines;
	std::map<std::string,Minion> minions;
	std::map<std::string,object> obstacles;
	GlobalState():boss(0){}

	//TODO more additional statistical information

};


#endif
