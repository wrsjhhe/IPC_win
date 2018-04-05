#include "Server.h"
#include <iostream>
#include "../Proto.h"

struct People
{
	char name[20];
	unsigned short age;
	char gender[10];
};

void add(PBYTE pBuffer, UINT64& size)
{
	int *pNum = (int*)pBuffer;
	(*pNum)++;
	//Sleep(3000);
}

void changeToWomen(PBYTE pBuffer, UINT64& size)
{
	People* pPeople = (People*)pBuffer;

	People people{ "lily" ,19,"woman" };
	memset(pPeople, 0, sizeof(People));
	memmove(pPeople, &people, sizeof(People));

}

void sayHello(PBYTE pBuffer, UINT64& size)
{
	char hello[] = "hello,my name is john";
	size = strlen(hello) + 1;
	memset(pBuffer, 0, size);
	memmove(pBuffer, hello, size);
}

int main()
{
	Server server;
	server.createShm(shmName, MAX_SHM_SIZE);

	server.appendRcvEvent(addEvt, add);
	server.appendRcvEvent(changeToWomenEvt, changeToWomen);
	server.appendRcvEvent(sayHelloEvt, sayHello);

	server.eventLoop(evtName);

	system("pause");
	return 0;
}