#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <iostream>
#include <functional>

//每次传递数据的包头
struct Header
{
	char type[20];  //调用的功能类型
	UINT64  size;   //数据大小
};

class Server
{
	typedef std::function<void(PBYTE pPayload, UINT64& size)> _FUNC;  //可以注册的事件类型
	typedef std::tuple<std::string, HANDLE, _FUNC> _NODE;  //每个事件节点

	struct Event
	{
		std::string type;
		_FUNC func;
	};

public:
	Server();
	Server(const Server&) = delete;
	~Server() { CloseHandle(m_hMap); }

	void createShm(const std::string& mapName, size_t size);
	void appendRcvEvent(const std::string& eventName, const _FUNC& func);

	void eventLoop(const std::string& eventName);

private:

	HANDLE m_hMap;
	size_t m_mapSize;
	std::string m_eventName;
	std::map<std::string, _FUNC> m_handlers;
};