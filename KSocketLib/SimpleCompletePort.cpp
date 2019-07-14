#include "SimpleCompletePort.h"
#include <functional>


SimpleCompletePort::SimpleCompletePort()
{
	portHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}


SimpleCompletePort::~SimpleCompletePort()
{
}

void SimpleCompletePort::InitWorker(int workercount)
{
	if (!workercount)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		workercount = info.dwNumberOfProcessors * 2;
	}
	for (size_t i = 0; i < workercount; i++)
	{
		HANDLE threadHandle;
		threadHandle = CreateThread(NULL, 0, &this->workThreadFunction, portHandle, 0, NULL);
		CloseHandle(threadHandle);
	}
}

template<class T>
void SimpleCompletePort::BindToPort(HANDLE handle, T tDataObject)
{
	CreateIoCompletionPort(handle, portHandle, &tDataObject, 0);
}

DWORD SimpleCompletePort::workThreadFunction(LPVOID lpParam)
{
	while (true)
	{
		DWORD byteCount;
		PULONG_PTR ptr;
		LPOVERLAPPED *over = NULL;
		if (GetQueuedCompletionStatus(HANDLE(lpParam), &byteCount, ptr, over, INFINITE))
		{

		}
	}
}
