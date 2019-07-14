#pragma once
#include <Windows.h>
class SimpleCompletePort
{
public:
	SimpleCompletePort();
	~SimpleCompletePort();
	void InitWorker(int workercount = 0);
	template<class T>
	void BindToPort(HANDLE handle,T tDataObject);
private:
	HANDLE portHandle;
	DWORD WINAPI workThreadFunction(LPVOID lpParam);
};


