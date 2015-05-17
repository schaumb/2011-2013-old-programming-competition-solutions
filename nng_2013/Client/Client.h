class CLIENT
{
public:
	CLIENT();
	virtual ~CLIENT();
	// Initializes the connection to the given server and port
	bool Init( std::string aIPAddress );

	// Runs the client
	void Run();

	std::string DebugResponse(std::vector<std::string> &text) { return HandleServerResponse(text); }

protected:
	void SendMessage( std::string aMessage );
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse) = 0;
	virtual void ConnectionClosed();
	virtual std::string GetClientName() = 0;
	SOCKET mConnectionSocket;
};

CLIENT *CreateClient();