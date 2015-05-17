#include "object.hpp"


void commander::Attack(strategy_protocol::CommandsMessage& cmds,const std::string& what,const std::string& id){
		++counter;
		LOG4("ATTACK " ,id,"->",what);
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::CommandsMessage::ATTACK);
		auto attack = command->mutable_attackcommand();
		attack-> set_withwhat(id);
		attack-> set_what(what);
	}
	
void commander::Move(strategy_protocol::CommandsMessage& cmds,int x, int y, const std::string& id){
		++counter;
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::CommandsMessage::MOVE);
		auto move = command->mutable_movecommand();
		move->set_withwhat(id);
		auto pos = move->mutable_toposition();
		pos->set_x(x);
		pos->set_y(y);
		LOG4("move:: ",x,' ',y);
	}
	
void commander::TrainSoldier(strategy_protocol::CommandsMessage& cmds,const  std::string& id){
		++counter;
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::CommandsMessage::TRAIN);
		auto train = command->mutable_traincommand();
		train->set_withwhat(id);
		train->set_what(strategy_protocol::CommandsMessage::SOLDIER);
	}
void commander::TrainWorker(strategy_protocol::CommandsMessage& cmds,const  std::string& id){
		++counter;
		auto command = cmds.add_commands();
		command->set_commandtype(strategy_protocol::CommandsMessage::TRAIN);
		auto train = command->mutable_traincommand();
		train->set_withwhat(id);
		train->set_what(strategy_protocol::CommandsMessage::WORKER);
	}

int commander::counter;
