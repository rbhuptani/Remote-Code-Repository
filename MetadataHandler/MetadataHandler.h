#ifndef MDHANDLER
#define MDHANDLER

/////////////////////////////////////////////////////////////////////
//  MetadataHandler.h - This package creates, add and read metadata//
//						fils from server repository.			   //			
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a MetadataHandler class.  It will be used to handle Metadata
related functionality on server

Public Interface:
=================
MetadataHandler	:	Constructor which sets XMLDocument with passed XML and if there is none, it will create one
addFileMD		:	add file tag with all the children ino XML Document and save the file
getAllDepFiles	:	get all dependencies of file by reading XML of all of its descedents

Build Process:
==============
Required Packages
- XMLDocument, XMLParser, XMLElements, FileMgr, FileSystem
HTTPMessage, MetadataHandler
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/

#include <iostream>
#include <string>
#include<unordered_set>
#include"..//HTTPMessage/HTTPMessage.h"
#include "..//XmlDocument/XmlDocument.h"
#include "..//XmlParser/XmlParser.h"

using namespace XmlProcessing;
using sPtr = std::shared_ptr <AbstractXmlElement>;
class MetadataHandler {
public:
	//detailed description of all function in .cpp file
	MetadataHandler(std::string path, std::string xmlFileName);
	void addFileMD(HTTPMessage msg);
	static std::unordered_set<std::string> getAllDepFiles(std::string filePath);
	static void getAllDepFilesHelper(std::string filePath, std::unordered_set<std::string> &allFiles);
	static std::string getTagValue(XmlDocument *doc,std::string tag);
private:
	std::string _path;
	std::string _xmlFileName;
	std::string _xmlFilePath;
	XmlDocument *_xDoc;
};

#endif