#pragma once
#include <windows.h>  
#include <vector>
#include <functional>
#include <iostream>
#include <tuple>

//ÿ�δ������ݵİ�ͷ
struct Header
{
	char type[20];  //���õĹ�������
	UINT64  size;   //���ݴ�С
};

class Client
{
	typedef std::function<void(PBYTE pPayload, UINT64 size)> _EVENT;  //����ע����¼�����

public:
	Client();
	Client(const Client&) = delete;

	void init(const std::string& mapName, size_t size, const std::string& eventName);

	void send(const std::string& eventType, LPVOID pdata, size_t dataSize, const _EVENT& callback);

private:

	HANDLE m_hMap;
	size_t m_mapSize;
	std::string m_EventName;
	//HANDLE m_hSendEvent;
	
};