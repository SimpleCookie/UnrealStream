// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>
#include "AllowWindowsPlatformTypes.h"
#include <boost/asio.hpp>
#include "HideWindowsPlatformTypes.h"
using boost::asio::ip::tcp;

/**
 * 
 */
class UNREALSTREAM_API NetworkServer 
{
public:
	NetworkServer();
	~NetworkServer();
	void sendMsg(std::string &message);
	std::string getHeader(int contentLength);
private:
	enum { max_length = 1024 };
	unsigned short port = 8080;
	std::string msg;
	//FRunnableThread* Thread;
	//boost::asio::io_service io_service;
	//tcp::acceptor acceptor;
	//tcp::socket s;


};
