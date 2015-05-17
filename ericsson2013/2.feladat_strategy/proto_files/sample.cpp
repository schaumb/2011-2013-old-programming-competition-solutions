#include <iostream>

#include "WorldStatusUpdate.pb.h"
#include "Commands.pb.h"
#include "ProtobufUtil.hh"

using namespace strategy_protocol;
using namespace protoutil;
using namespace std;

int main()
{

	size_t i = 0;
	while (cin.good()) {
		WorldStatusUpdate world;
		readProtoFromStream(world, cin);

		cerr << "Tick: " << i << endl;
		cerr << "Proto content with DebugString" << endl;
		cerr << world.DebugString() << endl;

		Commands commands;
		Commands::Command* command = commands.add_commands();
		command->set_commandtype(Commands::TRAIN);
		Commands::Train* train = command->mutable_traincommand();
		train->set_withwhat("Champion");
		train->set_what(Commands::WORKER);
		writeProtoOnStream(commands, cout);

		++i;
	}

	return 0;
}

