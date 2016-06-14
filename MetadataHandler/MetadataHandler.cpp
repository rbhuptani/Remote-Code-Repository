///////////////////////////////////////////////////////////////////////
//  MetadataHandler.cpp - This package creates, add and read metadata//
//						fils from server repository.				 //			
//  ver 1.0															 //
//  Language:      Visual C++ 2008, SP1                              //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1          //
//  Application:   Remote Code Repository, Project 4                 //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse          //
//                 University, rmbhupta@syr.edu                      //
/////////////////////////////////////////////////////////////////////
#include "MetadataHandler.h"
#include "..//FileMgr/FileMgr.h"
#include "..//FileMgr/FileSystem.h"
#include "..//Utilities/Utilities.h"
#include <io.h>
using SH = Utilities::StringHelper;

// Constructor which sets XMLDocument instance 
MetadataHandler::MetadataHandler(std::string path, std::string xmlFileName)
{
	_path = path;
	_xmlFileName = xmlFileName;
	_xmlFilePath = _path + _xmlFileName;
	std::ifstream xmlFile(_xmlFilePath);
	if (!xmlFile.is_open()) {
		std::ofstream newXmlFile;
		newXmlFile.open(_xmlFilePath);
		newXmlFile << "<?xml version = \"1.0\" encoding = \"utf-8\" ? >";
		newXmlFile << "<folder name=\"" << xmlFileName << "\"> </folder>";
		newXmlFile.close();
	}
	XmlParser _parser(_xmlFilePath);
	_xDoc = _parser.buildDocument();
}

// add file tag by reading HTTPMessage into XML file
void MetadataHandler::addFileMD(HTTPMessage msg)
{
	std::string filePath = FileSystem::Path::getFullFileSpec(msg.fileName);
	std::string fileName = FileSystem::Path::getName(filePath);
	std::vector<std::string> depList;
	depList = msg.body != "NA" ? SH::split(msg.body, '^') : depList;
	sPtr fileTag = makeTaggedElement("file");
	sPtr nameTag = makeTaggedElement("name");
	sPtr checkinTag = makeTaggedElement("open");
	nameTag->addChild(makeTextElement(fileName));
	checkinTag->addChild(makeTextElement(msg.open));
	sPtr authorTag = makeTaggedElement("author");
	authorTag->addChild(makeTextElement(msg.author));
	sPtr depTag = makeTaggedElement("dependencies");
	for (auto dep : depList) {
		sPtr depFileTag = makeTaggedElement("depFile");
		depFileTag->addChild(makeTextElement(dep));
		depTag->addChild(depFileTag);
	}
	fileTag->addChild(nameTag);
	fileTag->addChild(checkinTag);
	fileTag->addChild(authorTag);
	fileTag->addChild(depTag);
	sPtr root = _xDoc->xmlRoot();
	root->addChild(fileTag);
	_xDoc = new XmlDocument(makeDocElement(root));
	std::ofstream XmlFile;
	XmlFile.open(_xmlFilePath);
	XmlFile.clear();
	XmlFile << _xDoc->toString();
	XmlFile.close();
}

// get all dependencies of file specified in parameter
std::unordered_set<std::string> MetadataHandler::getAllDepFiles(std::string filePath)
{
	
	std::unordered_set<std::string> allFiles;
	getAllDepFilesHelper(filePath, allFiles);
	return allFiles;
}

// helper function for above function 
void MetadataHandler::getAllDepFilesHelper(std::string filePath, std::unordered_set<std::string>& allFiles)
{
	filePath = FileSystem::Path::getFullFileSpec(filePath);
	std::string path = FileSystem::Path::getPath(filePath);
	if (_access(path.c_str(), 0) != 0) return;// directory not found
	std::string xmlFilePath = path + SH::getFolderName(path) + ".xml";
	std::ifstream xmlFile(xmlFilePath);
	if (!xmlFile.is_open())return;  //xml file not found
	XmlParser _parser(xmlFilePath);
	XmlDocument *xDoc = _parser.buildDocument();
	std::vector<sPtr> files = xDoc->elements("folder").select();
	for (auto file : files) {
		XmlDocument tempXDoc(makeDocElement(file));
		std::string name = getTagValue(&tempXDoc, "name");
		if (name == FileSystem::Path::getName(filePath))
		{
			std::vector<sPtr> dependencies = tempXDoc.element("dependencies").descendents().select();
			if (dependencies.size() > 0) {
				for (size_t i = 0; i < dependencies.size();i++)
				{
					if (dependencies[i]->tag() == "")
					{
						std::string depFile = SH::trim(dependencies[i]->value());
						if (allFiles.count(depFile) != 1) {
							std::ifstream tfile(depFile);
							if (!tfile.is_open()) depFile = "." + depFile;
							depFile = FileSystem::Path::getFullFileSpec(depFile);
							MetadataHandler::getAllDepFilesHelper(depFile, allFiles);
							allFiles.insert(depFile);
						}
					}	
				}
			}
		}
	}
}

// get specific tag value of a document
std::string MetadataHandler::getTagValue(XmlDocument * doc, std::string tag)
{
	std::string value;
	std::vector<sPtr> elements = doc->elements(tag).select();
	if (elements.size() > 0)
	{
		for (auto elem : elements)
		{
			if (elem->tag() != "")
				std::cout << "\n  found: " << elem->tag();
			else
				value = elem->value();
		}
	}
	else
	{
		std::cout << "\n  couldn't find child elements";
	}
	return SH::trim(value);
}

#ifdef TEST_METADATAHANDLER

int main()
{
	MetadataHandler md("./Test/", "Test.xml");
	HTTPMessage msg;
	msg.toAddr = "localhost";
	msg.toPort = 8085;
	msg.author = "Ronak";
	msg.contentLen = msg.body.size();
	msg.fileName = "test_send_server.txt";
	msg.command = REQUEST_DOWNLOAD;
	msg.fromPort = 8080;
	msg.fromAddr = "localhost";
	msg.body = "Test.cpp^Test.h";
	//md.addFileMD(msg);
	/*std::cout << std::endl;
	std::string file = FileSystem::Path::getFullFileSpec(msg.fileName);
	std::cout << file << std::endl;
	std::string path = FileSystem::Path::getPath(file);
	std::cout << path << std::endl;
	std::cout << SH::getFolderName(path);*/
	md.getAllDepFiles("./Test/Test.txt");
	return 0;
}

#endif