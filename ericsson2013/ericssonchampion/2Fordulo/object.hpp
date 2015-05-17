#include <iostream>

#ifndef _H_OBJECT
#define _H_OBJECT

#include "WorldStatusUpdate.pb.h"
#include "Commands.pb.h"
#include "ProtobufUtil.hh"
#include "helper.hpp"
#include <unordered_map>
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
	virtual void AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds)=0;
	
	static void reset(){counter = 0;}
	static void AddDummyCommand(strategy_protocol::CommandsMessage& cmds){// nem csinálunk semmit
		if(counter == 0)TrainWorker(cmds,"");
	}
	
protected:
	static void TrainWorker(strategy_protocol::CommandsMessage& cmds,const  std::string& id);
	void TrainSoldier(strategy_protocol::CommandsMessage& cmds,const  std::string& id);
	void Move(strategy_protocol::CommandsMessage& cmds,int x, int y, const std::string& id);
	void Attack(strategy_protocol::CommandsMessage& cmds,const std::string& what,const std::string& id);
	private:
	static int counter;
};

class object {
public:
	object(){id = "undefined";}
	object(const strategy_protocol::WorldStatusUpdateMessage_CommonProperties& cp){
		id = cp.id();
		x = cp.position().x();
		y = cp.position().y();
	}
	void setCommon(const ::strategy_protocol::WorldStatusUpdateMessage_CommonProperties& cp){
		id = cp.id();
		x = cp.position().x();
		y = cp.position().y();
	}
	virtual void setRound(int r){round = r;}
	virtual int getRound(){return round;}
	
	v Position()const{return v(x,y);}
	virtual int RadiusW()const{return 1;}
	virtual int RadiusS()const{return 1;}
    std::string Id()const{return id;}
    
    virtual void Guard(object*){};
    virtual void UnGuard(){};
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
	dynamic_object(const strategy_protocol::WorldStatusUpdateMessage_CommonProperties& cp):object(cp){};
	int Health() const {return health;}
protected:
	int health;
};

class moveable : public dynamic_object{
public:
	moveable(){
		tr = [](v in){return in;};
	}
	moveable(const strategy_protocol::WorldStatusUpdateMessage_CommonProperties& cp):dynamic_object(cp){
		tr = [](v in){return in;};
	}
	virtual int R(const object& o) const = 0;
	std::queue<v> RealRoute(){return real_route;}
	void SetRealRoute(std::queue<v> r) {route=r;}
protected:
	std::queue<v> route;
	v nextStep(GlobalState& s,bool forced = false);
	bool isPlaned; //az út a tervezett szerint halad-e
	std::queue<v> real_route;
	bool isFree(v pos,const GlobalState& s, bool forced) const;
	std::function<v(v)> tr;
	//v prevPos;
	void addToHistory(v x);
	std::deque<v> prevPoss;
	int badCount = 1;
};

class goalable:public dynamic_object{
public:
	goalable(){}
	goalable(const strategy_protocol::WorldStatusUpdateMessage_CommonProperties& cp):dynamic_object(cp){}
	std::queue<v> getRout();
	std::queue<v> getReversRout();
	void addHint(std::queue<v>& q);
	int BaseDist()const{return theBestD;}
protected:
	int query_counter = 0;
	std::deque<v> thebest;
	int theBestD = INT_MAX;
};

class Mine : public goalable{
public:
	Mine(){}
	Mine(const strategy_protocol::WorldStatusUpdateMessage_Mine& m):goalable(m.common()),capacity(m.capacity()){};
	Mine& operator=(const strategy_protocol::WorldStatusUpdateMessage_Mine& m){
		setCommon(m.common());
		capacity= m.capacity();
		return *this;
	}
	int Capacity() const {return capacity;}
	int RadiusW() const override{return 0;}
	int RadiusS() const override{return 3;}
    void AddWorker(){++worker_counter;}
    void DieWorker(){--worker_counter;}
    int CanAddWorker()const{return theBestD/2 >= worker_counter;}
private:
	int capacity;
    int worker_counter = 0;
};

class Worker:public moveable,public commander{
public:
	enum state_type{SEARCH_MINE, WORKING};
	Worker(){}
	Worker(const strategy_protocol::WorldStatusUpdateMessage_Worker& w):moveable(w.common()),resource(w.resource()){
        health=w.health();
        state=SEARCH_MINE;
        guarded=false;
        buddy=NULL;
        mine=NULL;
    }
    ~Worker();
	Worker& operator=(const strategy_protocol::WorldStatusUpdateMessage_Worker& w){
		setCommon(w.common());
		health=w.health();
		resource=w.resource();
		return *this;
	}
	int Resource() const {return resource;}
	int R(const object& o) const {
		int r =o.RadiusW();
		int d = distance(v(x,y),o.Position());
		return std::min(d+1,r);}
    int RadiusW() const override{return 1;}
	int RadiusS() const override{return 2;}
    state_type State(){return state;}
    void State(state_type new_state){state=new_state;}
    void Guard(object* b){
        guarded=true;
        buddy=b;
    }
    void UnGuard();
    bool IsGuarded(){return guarded;}
    object* Buddy(){return buddy;}
    void AddMine(Mine* m){
        mine=m;
        m->AddWorker();
    } 
    void RemoveMine(){mine=NULL;}
    void AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds);
private:
	int resource;
    int iteration=0;
    v direction_vector;
    void Iterate(GlobalState& st,strategy_protocol::CommandsMessage& cmds);
    state_type state=SEARCH_MINE;
    object* buddy=0;
    Mine* mine=0;
    bool guarded=false;
	void Move(strategy_protocol::CommandsMessage& cmds,int toX, int toY){x=toX;y=toY;commander::Move(cmds,toX,toY,id);x=toX;y=toY;}
	void Attack(strategy_protocol::CommandsMessage& cmds,const std::string& what){commander::Attack(cmds,what,id);}
};

class Soldier:public moveable,public commander{
public:
	enum state_type{FOLLOWING, SEARCH_BOSS,WAITING_BOSS, ATTACK_BOSS};
	Soldier(){}
	Soldier(const strategy_protocol::WorldStatusUpdateMessage_Soldier& w):moveable(w.common()){
        health=w.health();
    }
    ~Soldier(){
       if(buddy!=NULL)
           buddy->UnGuard();
    }
	Soldier& operator=(const strategy_protocol::WorldStatusUpdateMessage_Soldier& w){
		setCommon(w.common());
		health= w.health();
		return *this;
	}
	int R(const object& o)const{
		int r = o.RadiusS();
		int d = distance(v(x,y),o.Position());
		return std::min(d+1,r);
	}
	int RadiusW() const override{return 1;}
	int RadiusS() const override{return 1;}
    state_type State(){return state;}
    void State(state_type new_state){state=new_state;}
    void UnGuard(){
        if(buddy!=NULL)
            buddy->UnGuard();
        buddy=NULL;
    }
    void Guard(object* w){
		LOG3("Guard",id,w->Id());
        buddy=w;
        state=FOLLOWING;
        buddy->Guard(this);
    }
    void AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds);
    bool IsLocked()const{return is_locked;}
    void Lock(){is_locked=true;}
    void UnLock(){is_locked=false;}
private:
    bool is_locked=false;
    state_type state = FOLLOWING;
	void Move(strategy_protocol::CommandsMessage& cmds,int toX, int toY){x=toX;y=toY;commander::Move(cmds,toX,toY,id);x=toX;y=toY;}
	void Attack(strategy_protocol::CommandsMessage& cmds,const std::string& what){commander::Attack(cmds,what,id);}
    object* buddy = 0;
};

class Boss:public goalable{
public:
	Boss(const strategy_protocol::WorldStatusUpdateMessage_Base& b):goalable(b.common()){health=b.health();}
	Boss& operator=(const strategy_protocol::WorldStatusUpdateMessage_Base& w){
		setCommon(w.common());
		health= w.health();
		return *this;
	}

	int RadiusW() const override{return 1;}
	int RadiusS() const override{return 1;}
private:

};

//Ez nem biztos, hogy kell
class Minion:public dynamic_object{
public:
	Minion(){}
	Minion(const strategy_protocol::WorldStatusUpdateMessage_Worker& m):dynamic_object(m.common()),radius(3){health=m.health();}
	Minion(const strategy_protocol::WorldStatusUpdateMessage_Soldier& m):dynamic_object(m.common()),radius(3){health=m.health();}
	Minion& operator=(const strategy_protocol::WorldStatusUpdateMessage_Worker& w){
		radius = 3;
		setCommon(w.common());
		health= w.health();
		return *this;
	}
	Minion& operator=(const strategy_protocol::WorldStatusUpdateMessage_Soldier&& w){
		radius = 4;
		setCommon(w.common());
		health= w.health();
		return *this;
	}
	int RadiusW() const override{return radius;}
	int RadiusS() const override{return radius-1;}
private:

	int radius;
};

class Base:public dynamic_object,public commander{
public:
	Base(){}
	Base(const strategy_protocol::WorldStatusUpdateMessage_Base& b):dynamic_object(b.common()){health=b.health();}
	Base& operator=(const strategy_protocol::WorldStatusUpdateMessage_Base& b){
		setCommon(b.common());
		health= b.health();
		return *this;
	}
	int RadiusW() const override{return 1;}
	int RadiusS() const override{return 1;}
	void AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds);
	
	bool IsGuarded() {return guarded;}
	void Guard(object* b) override {
        guarded=true;
        buddy=b;
    }
    void UnGuard() override;

private:
	object* buddy=0;
	bool guarded;
	void TrainWorker(strategy_protocol::CommandsMessage& cmds){commander::TrainWorker(cmds,id);}
	void TrainSoldier(strategy_protocol::CommandsMessage& cmds){commander::TrainSoldier(cmds,id);}
};

enum strategy_state{QUIESCENCE,ATTACK,BOSS,ATTACK_THE_BOSS};



struct GlobalState{
	strategy_state state;
	Boss* boss;
	int resource;
	Base base;


	int myteam;
	std::map<std::string,Worker> workers;
	std::map<std::string,Soldier> soldiers;
	std::map<std::string,Mine> mines;
	std::unordered_map<std::string,Minion> minions;
	std::unordered_map<std::string,object> obstacles;
	GlobalState():boss(0){}

	//TODO more additional statistical information

};




#endif

