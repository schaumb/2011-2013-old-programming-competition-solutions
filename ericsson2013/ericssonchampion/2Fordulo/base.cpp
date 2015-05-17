#include "object.hpp"

#define MINRESWORKER 2.5
#define WORKERTIME 5.0

void Base::AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds){
	if(st.workers.size()+st.soldiers.size()>50){return;}//nehogy kezelhetetlenné váljon
	
	if(st.resource <20) return;
	FOREACHMOVEABLE(
		if(distance(i->second.Position(),v(x,y))==0) return;
	)
	
	static int alt = 0;
	int s = 0;
	int c = 0;
	for(auto i = st.mines.begin(); i != st.mines.end(); ++i){
		s += i->second.BaseDist()==INT_MAX?distance(v(x,y),i->second.Position()):i->second.BaseDist();
		c += i->second.CanAddWorker();
	}
	
	float workerRate = (st.workers.size()*WORKERTIME)/(s+1); //Az elején csak felső becslés!!
	
	if( workerRate > 0.75){ // Ha van elég dolgozó
		TrainSoldier(cmds);
	}

	switch (st.state) {

		case QUIESCENCE:{
			if(st.resource  > MINRESWORKER * 20){
				(++alt%2)?TrainWorker(cmds):TrainSoldier(cmds);//Init után dolgozó jön létre
			}
			else if(++alt%20==0&&st.workers.size()<3){
				TrainWorker(cmds);
			}
			break;
		}
		case ATTACK:{
			if(c==0 || st.resource  > MINRESWORKER * 50){//50 a katona ára
				TrainSoldier(cmds);
			}
			else if(c!=0 && st.resource   > MINRESWORKER * 20){
				(++alt%3==0)?TrainWorker(cmds):TrainSoldier(cmds);
			}
			
			break;
		}
		case BOSS:{
			if(c!=0 && st.workers.size() <=2 && st.resource < MINRESWORKER * 20){ // 20 a dolgozó ára
				TrainWorker(cmds);
			}
			if(c!=0 &&workerRate <0.5){
				(++alt%3 == 0)?TrainWorker(cmds):TrainSoldier(cmds);
			}
			else if(workerRate <0.7){
				(++alt%20==0)?TrainWorker(cmds):TrainSoldier(cmds);
			}
			else{
				TrainSoldier(cmds);
			}
			break;
		}
		case ATTACK_THE_BOSS:{
			if(c!=0 &&st.resource  < MINRESWORKER * 20){
				TrainWorker(cmds);
			}
			if(workerRate <0.3){
				(++alt%3)?TrainWorker(cmds):TrainSoldier(cmds);
			}
			TrainSoldier(cmds);
			break;
		}
	}
}

void Base::UnGuard()
{
    if(guarded){
        guarded=false;
        if(buddy!=NULL){
            buddy->UnGuard();
            buddy=NULL;
        }
    }
}
