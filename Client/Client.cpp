#include "Client.h"


Client::Client():m_hMap(NULL),m_mapSize(0)
{
}

void Client::init(const std::string& mapName, size_t size, const std::string& eventName)
{
	// 首先试图打开一个命名的内存映射文件对象  
	m_hMap = ::OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		0,                         //如这个函数返回的句柄能由当前进程启动的新进程继承，则这个参数为TRUE。
		mapName.c_str()
	);
	if (NULL == m_hMap)// 打开失败
	{
		throw;
	}
	m_mapSize = size;

	m_EventName = eventName;

}


void Client::send(const std::string& eventType, LPVOID pdata, size_t dataSize, const _EVENT& callback)
{
	Header header;

	LPVOID pBuffer = ::MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	memset(pBuffer, 0, m_mapSize);

	//设置数据头
	strncpy_s(header.type, eventType.length() + 1, eventType.c_str(), 20);
	header.size = dataSize;

	//将数据复制到共享内存(数据头+有效数据)
	memmove((PBYTE)pBuffer, &header, sizeof(header));

	PBYTE pPayload = (PBYTE)pBuffer + sizeof(header);
	memmove(pPayload, pdata, dataSize);

	std::string sndEventName = m_EventName + "ctos";
	HANDLE hSendEvent = OpenEvent(EVENT_ALL_ACCESS, NULL, sndEventName.c_str());
	if (hSendEvent == nullptr)
	{
		throw;
	}
	SetEvent(hSendEvent);

	//开始监听数据返回事件
	std::string rcvEventName = m_EventName + "stoc";
	HANDLE hRcvEvent = CreateEvent(NULL, FALSE, FALSE, rcvEventName.c_str());
	if (hRcvEvent == nullptr)
	{
		//错误处理
	}
	std::cout << "等待数据返回中..." << std::endl;
	WaitForSingleObject(hRcvEvent, INFINITE);  //收到信号，自动重置

	//数据返回以后：
	 //解包
	memset(&header, 0, sizeof(header));
	memmove(&header, (PBYTE)pBuffer, sizeof(header));
	pPayload = (PBYTE)pBuffer + sizeof(header);

	callback(pPayload, header.size);
	CloseHandle(hRcvEvent);
}
