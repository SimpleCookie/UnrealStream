// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealStream.h"
#include "NetworkServer.h"
#include <ctime>
#include <iostream>


NetworkServer::NetworkServer()/*:acceptor(io_service, tcp::endpoint(tcp::v4(), port)), s(io_service)*/ {
}


NetworkServer::~NetworkServer() {
}

void NetworkServer::sendMsg(std::string &message)
{
	try {
		boost::asio::io_service io_service;

		// A ip::tcp::acceptor object needs to be created to listen for new connections. 
		// It is initialised to listen on TCP port 8080, for IP version 4.
		tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port);
		tcp::acceptor acceptor(io_service, endpoint);
		/*
		tcp::iostream stream;
		acceptor.accept(*stream.rdbuf());
		stream << message;
		*/
		
		tcp::socket s(io_service);
		acceptor.accept(s);
		size_t request_length = message.length();
		boost::asio::write(s, boost::asio::buffer(message, request_length));
		
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << std::endl;
	}
}

std::string NetworkServer::getHeader(int contentLength)
{
	std::string h =
		"HTTP/1.1 200 OK\n"
		"Content-Length: " + std::to_string(contentLength) + "\n"
		"Content-Type: image/jpg;\n"
		"Connection: close\n"
		"\n";
	return h;
}

