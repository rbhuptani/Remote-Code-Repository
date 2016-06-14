#include "Client.h"

/////////////////////////////////////////////////////////////////////
//  Client.cpp - Sends message to Server and Receives messages     //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////

using SH = Utilities::StringHelper;

// This function starts sender using Sockets class and dequeue messages from sender queue and process them
void Client::startSender()
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (true)
		{
			auto request = sndrQueue.deQ();
			if (request.command == END_CLIENT_RECEIVER) {
				std::cout << std::endl << " Client Sender Quiting" << std::endl;
				break;
			}				
			processRequest(si, request);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << " Exception caught in startSender : " << ex.what() << std::endl;
	}
}


// This function starts receiver
void Client::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(_port, Socket::IP6);
		sl.start(clientReceiver);
		std::cout << std::endl<< " Press key to exit: "<<std::endl;
		std::cout.flush();
		std::cin.get();
		HTTPMessage msg;
		msg.command = END_CLIENT_RECEIVER;
		msg.toAddr = _server_address;
		msg.toPort = _server_port;
		msg.body = "End Client";
		msg.author = "Ronak";
		msg.contentLen = msg.body.size();
		msg.fileName = "NA";
		msg.fromPort = _port;
		msg.fromAddr = _address;
		postMessage(msg);
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << " Exception caught in startReciver : " << ex.what() << std::endl;
	}
}

//This function connects to socket with specified addresss and port
bool Client::connectSocket(SocketConnecter & si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port))
	{
		std::cout << std::endl << " Client is waiting to connect to localhost:"<< port << std::endl;
		::Sleep(100);
	}
	return true;
}

//This function will enqueue message in sender queue
void Client::postMessage(const HTTPMessage & msg)
{
	sndrQueue.enQ(msg);
}

//This function will process message and send with appropriate command 
void Client::processRequest(SocketConnecter & si, HTTPMessage request)
{
	connectSocket(si, request.toAddr, request.toPort);
	request.fromPort = _port;
	request.fromAddr = _address;
	if (request.command == UPLOAD_REQUEST)
		clientSender.senderFactory(si, request, UPLOAD_REQUEST);
	else if (request.command == GETALLFILES_REQUEST)
		clientSender.senderFactory(si, request, GETALLFILES_REQUEST);
	else if (request.command == DOWNLOAD_REQUEST)
		clientSender.senderFactory(si, request, DOWNLOAD_REQUEST);
	si.shutDown();
}

// This function will receive message from receiver queue
HTTPMessage Client::getMessage()
{
	HTTPMessage msg;
	std::string stringMsg = rcvrQueue.deQ();
	msg = HTTPMessage::parseStringMessage(stringMsg);
	return msg;
}

// Parse command line arguments to ser server and clients address and port
bool Client::ParseCommandLineArgs(int argc, char** argv)
{
	if (argc > 1) {
		std::vector<std::string> address = SH::split(argv[1], ':');
		if (address.size() == 2) {
			_address = address[0];
			_port = stoi(address[1]);
		}
	}
	if (argc > 2) {
		std::vector<std::string> address = SH::split(argv[2], ':');
		if (address.size() == 2) {
			_server_address = address[0];
			_server_port = stoi(address[1]);
		}
	}
	return false;
}

#ifdef TEST_CLIENT

int main(int argc, char **argv) {
	Client client;
	client.ParseCommandLineArgs(argc, argv);
	std::string ct = "Client " + client.getClientAddress() + ":" + std::to_string(client.getClientPort());
	SetConsoleTitle(ct.c_str());
	SH::Title(" Sending Upload Request Message with file : test_send.txt");
	std::cout << std::endl;
	HTTPMessage msg;
	msg.fromAddr = client.getClientAddress();
	msg.fromPort = client.getClientPort();
	msg.toAddr = client.getServerAddress();
	msg.toPort = client.getServerPort();
	msg.body = "Test.cpp^test.h";
	msg.author = "Ronak";
	msg.contentLen = msg.body.size();
	msg.fileName = client.getReceiverPath() + "test_send.txt";
	msg.command = UPLOAD_REQUEST;
	client.postMessage(msg);

	SH::Title(" Sending Get All files Request Message ");
	std::cout << std::endl;
	msg.fileName = "test_send.txt";
	msg.command = GETALLFILES_REQUEST;
	client.postMessage(msg);

	SH::Title(" Sending Download Request Message with file : ./Server_Repository/Test/Test.txt");
	std::cout << std::endl;
	msg.body = "true";
	msg.command = DOWNLOAD_REQUEST;
	msg.fileName = "./Server_Repository/Test/Test.txt";
	client.postMessage(msg);

	SH::Title(" Ending Client Receiver ");
	std::cout << std::endl;
	msg.command = END_CLIENT_RECEIVER;
	client.postMessage(msg);

	std::thread receiverThread(&Client::startReceiver, &client);
	std::thread senderThread(&Client::startSender, &client);
	receiverThread.join();
	senderThread.join();	
	return 0;
}

#endif