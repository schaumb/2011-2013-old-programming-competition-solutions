#include "stdafx.h"
#include "Client.h"


// sample

const char *DirectionName[]={ "north", "east", "south", "west" };

class MYCLIENT : public CLIENT
{
public:
	Map map;
	Route route;

	MYCLIENT();
protected:
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse);
	virtual std::string GetClientName() { return std::string("Calmare"); }
};

MYCLIENT::MYCLIENT()
{
}

std::string MYCLIENT::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
	
	if ( map.updateMap(ServerResponse) || route.empty() ) {
		route = getRouteToNextGem(map);
	}
	Direction direction = Direction::Stay;

	if ( !route.empty() ) {
		direction = route.front();
		route.pop_front();
	}

	return DirectionToString(direction);
}


CLIENT *CreateClient()
{
	return new MYCLIENT();
}
