#include "Client.h"
#include <iostream>
#include <string>
#include "../Proto.h"

struct People
{
	char name[20];
	unsigned short age;
	char gender[10];
};


void add(PBYTE pPayload, UINT64 size)
{
	int *pNum = (int*)pPayload;
	std::cout << *pNum << std::endl;
}

void changeToWomen(PBYTE pPayload, UINT64 size)
{
	People* pPeople = (People*)pPayload;
	printf("man has changed to woman,her name is:%s,age is:%hu,gender is:%s\n",
		pPeople->name,
		pPeople->age,
		pPeople->gender
	);
}

void sayHello(PBYTE pPayload, UINT64 size)
{
	std::string str = (char*)pPayload;
	std::cout << str << std::endl;
}

int main()
{
	int a = 10;
	People man{ "john",20,"man" };

	Client client;
	client.init(shmName, MAX_SHM_SIZE, evtName);

	client.send(addEvt, &a, sizeof(a), add);
	client.send(changeToWomenEvt, &man, sizeof(man), changeToWomen);
	client.send(sayHelloEvt, nullptr, 0, sayHello);

	system("pause");
	return 0;
}
