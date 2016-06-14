#include "Receiver.h"
#include "..//Utilities/Utilities.h"
#include "..//FileMgr/FileMgr.h"
#include "..//MetadataHandler/MetadataHandler.h"
#include <vector>
#include <io.h>
#include <ctime>
using SH = Utilities::StringHelper;

// this function sets recevier path
void Receiver::setRecevierPath(std::string rcvPath)
{
	_rcvPath = rcvPath;
}

// this function overlaod operator () 
// it will process message request as the come form socket
void Receiver::operator()(Socket & si)
{
	//std::cout << "Receiver started"<<std::endl;
	std::string stringMsg;
	while (true)
	{
		stringMsg = si.recvString();
		if (stringMsg.size() == 0)
			break;
		HTTPMessage rcvMessage;
		rcvMessage = rcvMessage.parseStringMessage(stringMsg);
		if (rcvMessage.command == START_UPLOAD || rcvMessage.command == END_UPLOAD || rcvMessage.command == ACK_UPLOAD)
		{
			fileUploadHandler(si, rcvMessage);
			continue;  // go back and get another command
		}
		else if ((rcvMessage.command == REQUEST_ALL_FILES) || (rcvMessage.command == RESPONSE_ALL_FILES))
		{
			getAllFilesHandler(rcvMessage);
			continue;
		}
		else if (rcvMessage.command == START_DOWNLOAD || rcvMessage.command == END_DOWNLOAD || rcvMessage.command == REQUEST_DOWNLOAD)
		{
			fileDownloadHandler(si, rcvMessage);
		}
		else if (rcvMessage.command == END_CLIENT_RECEIVER)
		{
			_rcvQueue.enQ(stringMsg);
		}
	}
}

// this function swap address of source and destination
void Receiver::swapAddress(HTTPMessage & msg)
{
	std::string temp = msg.fromAddr;
	msg.fromAddr = msg.toAddr;
	msg.toAddr = temp;
	int tempPort = msg.fromPort;
	msg.fromPort= msg.toPort;
	msg.toPort = tempPort;
}

// this function store files comming from socket
bool Receiver::getFile(Socket & socket, HTTPMessage & msg)
{
	bool flag = false;
	std::string fn = FileSystem::Path::getName(msg.fileName); //filename with ext
	std::string filePath = _rcvPath + fn;
	if (msg.command == END_UPLOAD || msg.command == UPLOAD || msg.command == START_UPLOAD) {
		//version creator
		std::string fileName = SH::getFileName(fn); //without extension 
		std::string packagePath = _rcvPath + fileName;
		if (_access(packagePath.c_str(), 0) != 0)
			FileSystem::Directory::create(packagePath);
		std::time_t result = std::time(nullptr);
		std::string currTime = std::to_string(result);
		packagePath = packagePath + "/" + currTime;
		if (_access(packagePath.c_str(), 0) != 0)
			FileSystem::Directory::create(packagePath);
		filePath = packagePath + "/" + fn;
		MetadataHandler md(FileSystem::Path::getPath(filePath), fileName + ".xml");
		md.addFileMD(msg);
	}
	flag = fileWriter(socket, msg, filePath);
	if (flag)
		msg.fileName = FileSystem::Path::getFullFileSpec(filePath);
	return flag;
}

// this function si helper function to above function
bool Receiver::fileWriter(Socket & socket, HTTPMessage & rcvmessage, std::string filePath)
{
	bool flag = false;
	Socket::byte buffer[BUFF_SIZE];
	std::ofstream* outFile = new std::ofstream;
	outFile->open(filePath, std::ios::out | std::ios::binary);
	if (outFile->is_open())
	{
		int size = rcvmessage.contentLen;
		buffReader(socket, size, buffer);
		outFile->write(buffer, size);
		if (rcvmessage.command == END_UPLOAD || rcvmessage.command == END_DOWNLOAD)
		{
			flag = true;
			if (outFile)
			{
				outFile->close();
				outFile = nullptr;
			}
		}
		while (!flag)
		{
			std::string stringMsg = socket.recvString();
			if (stringMsg.size() == 0)
				break;
			rcvmessage = rcvmessage.parseStringMessage(stringMsg);
			size = rcvmessage.contentLen;
			buffReader(socket, size, buffer);
			outFile->write(buffer, size);
			if (rcvmessage.command == END_UPLOAD || rcvmessage.command == END_DOWNLOAD)
			{
				flag = true;
				if (outFile)
				{
					outFile->close();
					outFile = nullptr;
				}
			}
		}
	}
	else
		std::cout << std::endl << " Receiver : Not able to open " << filePath;
	return flag;
}

// this function is helper function of above function
bool Receiver::buffReader(Socket & socket_, int size, Socket::byte * buffer)
{
	bool isGood;
	if (socket_.bytesWaiting() == 0)
		return true;
	isGood = socket_.recv(size, buffer);
	if (socket_ == INVALID_SOCKET || !isGood)
		return false;
	return true;
}

// this function get all files from Server_Repository directory
void Receiver::getAllServerFiles(HTTPMessage & msg)
{
	if (msg.command == REQUEST_ALL_FILES) {
		std::cout << std::endl << " Receiver : Message received to get all files from localhost:" << msg.fromPort << std::endl;
		std::vector<std::string> allFiles = FileMgr::getFiles(_rcvPath, { "*.h" , "*.cpp" , "*.txt" });
		std::string allFilesString = "";
		for (std::string file : allFiles) {
			file = FileSystem::Path::getFullFileSpec(file);
			allFilesString += file;
			allFilesString.append(1, FILES_SEP);
		}
		std::string body = allFilesString.substr(0, allFilesString.size() - 1);
		msg.command = RESPONSE_ALL_FILES;
		if (body == "")
			body = "NA";
		msg.body = body;
		swapAddress(msg);
		std::string message = HTTPMessage::createHTTPMessage(msg);
		_rcvQueue.enQ(message);
	}
	else if (msg.command == RESPONSE_ALL_FILES) {
		//recevied all files list from server 
		std::cout << std::endl << " Receiver : Message received with all files from localhost:" << msg.fromPort << std::endl;
		std::string allFiles = msg.body;
		std::vector<std::string> allFilesList = SH::split(allFiles, '^');
		allFiles = "";
		for (std::string file : allFilesList)
			allFiles += "\t- " + file + "\n";
		std::cout << std::endl << allFiles <<std::endl;
		std::string message = HTTPMessage::createHTTPMessage(msg);
		_rcvQueue.enQ(message);
	}
}

// this is fileOpload handler and will be called when Ipload request has come
void Receiver::fileUploadHandler(Socket & _socket, HTTPMessage & rcvMessage)
{
	if ((rcvMessage.command == UPLOAD || rcvMessage.command == START_UPLOAD || rcvMessage.command == END_UPLOAD) && rcvMessage.fileName.size() > 0)
	{
		std::string fileName = rcvMessage.fileName;
		if (getFile(_socket, rcvMessage))
		{
			std::cout << std::endl << " Receiver : Message received to upload "<< "\"" << fileName <<"\"" << " from localhost:" << rcvMessage.fromPort << std::endl;
			std::string onlyFileName = FileSystem::Path::getName(fileName);
			swapAddress(rcvMessage);
			rcvMessage.command = ACK_UPLOAD;
		}
		else {
			std::cout << std::endl << " Receiver : Upload Failed" << std::endl;
			return;
		}
	}
	std::string message = HTTPMessage::createHTTPMessage(rcvMessage);
	_rcvQueue.enQ(message);
}

// this is getallfiles handler and will be called when getallfiles request has come
void Receiver::getAllFilesHandler(HTTPMessage & rcvmessage)
{
	getAllServerFiles(rcvmessage);
}
// this is download handler and it will be called when download requesr is recevied
void Receiver::fileDownloadHandler(Socket & _socket, HTTPMessage & rcvmessage)
{
	if (rcvmessage.command == REQUEST_DOWNLOAD) {
		std::cout<<std::endl<<" Receiver : File download request has been received by " + rcvmessage.fromAddr + ":" + std::to_string(rcvmessage.fromPort) << std::endl;
		rcvmessage.command = DOWNLOAD;
		swapAddress(rcvmessage);
	}
	else if (rcvmessage.command == START_DOWNLOAD || rcvmessage.command == END_DOWNLOAD && rcvmessage.fileName.size() > 0)
	{
		std::string fileName = rcvmessage.fileName;
		if (getFile(_socket, rcvmessage))
		{
			std::string onlyFileName = FileSystem::Path::getName(fileName);
			rcvmessage.command = ACK_DOWNLOAD;		
			std::cout<<std::endl<<" Receiver : File \""<< rcvmessage.fileName <<"\"downloaded from " + rcvmessage.fromAddr + ":" + std::to_string(rcvmessage.fromPort) <<std::endl;
		}
		else {
			std::cout << std::endl << " Receiver : Download Failed" << std::endl;
			return;
		}	
	}
	std::string message = HTTPMessage::createHTTPMessage(rcvmessage);
	_rcvQueue.enQ(message);
}

#ifdef TEST_RECEIVER

int main() {
	SH::title("Testing Receiver");
	std::cout << std::endl;
	try
	{
		BlockingQueue<std::string> queue;
		std::string rcvPath = "./Download/";
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		Receiver rcvr(queue, rcvPath);
		sl.start(rcvr);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << " Exception caught : " << ex.what() << std::endl;
	}
	return 0;
}

#endif