/////////////////////////////////////////////////////////////////////
//  Server.cpp -   Responses to requests received from any client  //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////
#include "Server.h"
#include "..//MetadataHandler/MetadataHandler.h"
#include <unordered_set>

using SH = Utilities::StringHelper;

// process request received by recevier queue
void Server::processRequest(SocketConnecter & si, std::string requestMsg)
{
	HTTPMessage rcvdHMsg;
	rcvdHMsg = HTTPMessage::parseStringMessage(requestMsg);
	if (rcvdHMsg.command == ACK_UPLOAD)
	{
		if (connectSocket(si, rcvdHMsg.toAddr, rcvdHMsg.toPort))
			serverSender.senderFactory(si,rcvdHMsg, UPLOAD_RESPONSE);
	}
	else if ((rcvdHMsg.command == RESPONSE_ALL_FILES))
	{
		if (connectSocket(si, rcvdHMsg.toAddr, rcvdHMsg.toPort))
			serverSender.senderFactory(si, rcvdHMsg, GETALLFILES_RESPONSE);
	}
	else if (rcvdHMsg.command == DOWNLOAD)
	{
		if (connectSocket(si, rcvdHMsg.toAddr, rcvdHMsg.toPort))
		{
			std::string path = FileSystem::Path::getPath(rcvdHMsg.fileName);
			if (_access(path.c_str(), 0) != 0) rcvdHMsg.fileName = "." + rcvdHMsg.fileName;
			serverSender.senderFactory(si, rcvdHMsg, DOWNLOAD_RESPONSE);
			if (rcvdHMsg.body != "false") {
				std::unordered_set<std::string> depFileList = MetadataHandler::getAllDepFiles(rcvdHMsg.fileName);
				for (auto fn : depFileList) {
					rcvdHMsg.fileName = fn;
					serverSender.senderFactory(si, rcvdHMsg, DOWNLOAD_RESPONSE);
				}
			}
		}
	}
}

// starts sender and process message came form receiver queue
void Server::startSender()
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (true)
		{
			auto requestMsg = rcvrQueue.deQ();
			HTTPMessage msg = HTTPMessage::parseStringMessage(requestMsg);
			// need to make it for N clients , check if all clients ended
			if (msg.command == END_SERVER) {  
				std::cout << std::endl << " Server Reciver Quiting"<<std::endl;
				break;
			}				
			processRequest(si, requestMsg);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << " Exception caught in startSender : " << ex.what() << std::endl;
	}
}

// start receiver and starts listening
void Server::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(_port, Socket::IP6);
		sl.start(serverReceiver);
		std::cout.flush();
		std::cin.get();
		HTTPMessage msg;
		msg.command = END_SERVER;
		msg.toAddr = "localhost";
		msg.toPort = 8085;
		msg.body = "End Server";
		msg.author = "Ronak";
		msg.contentLen = msg.body.size();
		msg.fileName = "NA";
		msg.fromPort = getPort();
		msg.fromAddr = getAddress();
		rcvrQueue.enQ(HTTPMessage::createHTTPMessage(msg));
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << " Exception caught in startReciver : " <<  ex.what() <<std::endl;
	}
}

// Parse command line arguments to set server address and port
bool Server::ParseCommandLineArgs(int argc,char** argv)
{
	if (argc > 1) {
		std::vector<std::string> address = SH::split(argv[1], ':');
		if (address.size() == 2) {
			_address = address[0];
			_port = stoi(address[1]);
			return true;
		}
	}
	return false;
}

// connect to socket
bool Server::connectSocket(SocketConnecter & si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port))
	{
		std::cout << std::endl << " Server's sender waiting to connect localhost:"<<port << std::endl;
		::Sleep(100);
	}
	return true;
}

// enqueue messages to recevier queue
void Server::enQueue(std::string msg)
{
	rcvrQueue.enQ(msg);
}

// dequeue messages from recevier queue
std::string Server::deQueue()
{
	return rcvrQueue.deQ();
}

#ifdef TEST_SERVER

int main(int argc,char **argv) {	
	Server server;
	server.ParseCommandLineArgs(argc, argv);
	std::string ct = "Server " + server.getAddress() + ":" + std::to_string(server.getPort());
	SetConsoleTitle(ct.c_str());
	SH::title(" Server Started");
	std::cout << std::endl;
	std::cout << std::endl;
	std::thread receiverThread(&Server::startReceiver, &server);
	std::thread senderThread(&Server::startSender, &server);
	receiverThread.join();
	senderThread.join();
	std::cout << "Server End";
	return 0;
}

#endif

