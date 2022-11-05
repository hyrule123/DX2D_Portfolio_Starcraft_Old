#pragma once

#include "../EngineInfo.h"

class CThread
{
	friend class CThreadManager;

protected:
	CThread();
	virtual ~CThread() = 0;

protected:
	HANDLE	m_Thread;
	HANDLE	m_StartEvent;
	bool	m_Loop;

public:
	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}

public:
	void Suspend();
	void Resume();
	void ReStart();
	void Stop();
	void Start();

public:
	virtual bool Init();
	virtual void Run() = 0;

private:
	static unsigned int __stdcall ThreadFunction(void* Arg);
};

