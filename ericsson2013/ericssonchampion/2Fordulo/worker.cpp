#include "object.hpp"

#define MAX_RESOURCE 10

void Worker::AddToCommands(GlobalState& st,strategy_protocol::CommandsMessage& cmds){
    std::string mine_id = "";
    v next_step;
    if(mine!=0 && distance(Position(), mine->Position()) <=1 && mine->Capacity() <= 0)
        State(SEARCH_MINE);

    for(auto i = st.minions.begin(); i != st.minions.end(); ++i){
        if(distance(i->second.Position(), Position()) < 3 && Resource() == 0){
            Attack(cmds, i->second.Id());
            return;
        }
    }
    
    
    switch(state)
    {
        case WORKING:
            if(Position() == mine->Position() && Resource() ==  MAX_RESOURCE){ //Kérdezés
                //route=mine->getReversRout();
                route=std::queue<v>();
                route.push(st.base.Position()); //ez és az előző!!!
                real_route.push(mine->Position());
                mine->addHint(real_route);
                next_step = nextStep(st);
                Move(cmds, next_step.X, next_step.Y);
                break;
            }
            if(distance(Position(), st.base.Position()) == 1 && Resource() == 0){
                //route=mine->getRout(); 
                route=std::queue<v>();
                route.push(mine->Position()); //ez és az előző!!!
                //if(route.size() > 1){
                //    route.pop();
                //}
                real_route=std::queue<v>();
                real_route.push(st.base.Position());
                next_step = nextStep(st);
                Move(cmds, next_step.X, next_step.Y);
                break;
            }
            else{//Útközben
                next_step = nextStep(st);
                Move(cmds, next_step.X, next_step.Y);
                break;
            }

        case SEARCH_MINE:
			switch(st.state)
			{
				case BOSS:
				case ATTACK_THE_BOSS:
				{
					route = std::queue<v>();
					route.push(st.boss->Position());
					next_step=nextStep(st);
					/*if(next_step == v(x,y))
					{
						std::cerr << "WHY??:(\n";
					}*/
					Move(cmds, next_step.X, next_step.Y);
					
					return;
				}
			}
			mine = 0;
            if(!st.mines.empty()){
                for(auto i = st.mines.begin(); i != st.mines.end();++i){
                    if(i->second.CanAddWorker() && i->second.Capacity() > 0){
                        mine_id=i->first;
                        State(WORKING);
                        iteration=0;
                        break;
                    }
                }
                if(mine_id!=""){
                    AddMine(&st.mines[mine_id]);
                    //route=mine->getRout();
                    route=std::queue<v>();
                    route.push(mine->Position()); //ez és az előző!!!
                    real_route=std::queue<v>();
                    real_route.push(st.base.Position());
                }
            }
            else{
                if(route.empty())
                    Iterate(st, cmds);
            }
            next_step=nextStep(st);
            Move(cmds, next_step.X, next_step.Y);
            break; 
    }
}

void Worker::Iterate(GlobalState &st, strategy_protocol::CommandsMessage& cmds){
    v next_step;
    direction_vector=v(0,-1);
    if(iteration%6 == 0) {
        ++iteration;
        direction_vector = v(direction_vector.X*iteration, direction_vector.Y*iteration);
        route.push(Position() + direction_vector);
    }
    else {
        route.push(Position() + turn_right(direction_vector));
    }
}

Worker::~Worker()
{
{
        if(mine!=NULL)
            mine->DieWorker();
        if(buddy!=NULL)
            buddy->UnGuard();
    }
}

void Worker::UnGuard()
{
    if(guarded){
        guarded=false;
        if(buddy!=NULL){
            buddy->UnGuard();
            buddy=NULL;
        }
    }
}
