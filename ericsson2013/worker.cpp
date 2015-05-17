#include "object.hpp"

#define MAX_RESOURCE 10

void Worker::AddToCommands(GlobalState& st,strategy_protocol::Commands& cmds){
    std::string mine_id;
    v next_step;
    switch(state)
    {
        case WORKING:
            if(Position() == mine->Position() && Resource() < MAX_RESOURCE)
               break; //Állunk a bányán, szedjük a cuccost
            if(Position() == mine->Position() && Resource() ==  MAX_RESOURCE){
                //Kérdezünk a bányától, vagy:
                route.empty();
                route.push(st.base.Position()); //Indulunk haza
                break;
            }
            //Bányához megyünk, vagy haza
            else{ 
                next_step = nextStep(st);
                Move(cmds, next_step.X, next_step.Y);
            }
             
            break;

        case SEARCH_MINE:
            if(st.mines.size() != 0){
                for(auto i = st.mines.begin(); i != st.mines.end();++i){
                    if(i->second.CanAddWorker()){
                        mine_id=i->first;
                        State(WORKING);
                        break;
                    }
                }
                if(mine_id!=""){
                    AddMine(&st.mines[mine_id]);
                    route.push(mine->Position());
                }
                next_step=nextStep(st);
                Move(cmds, next_step.X, next_step.Y); 
            }
            else{
                   
            }
            break; 
    }
}

