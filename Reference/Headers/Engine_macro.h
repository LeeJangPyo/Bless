#pragma once

#define			_MSG_BOX(MESSAGE) MessageBox(0, MESSAGE, TEXT("System Message"), MB_OK);
#define			_TAGMSG_BOX(_message,_tag)	MessageBoxW(NULL, _message, _tag, MB_OK)

#define NO_COPY(CLASSNAME)									\
	private:												\
	CLASSNAME(const CLASSNAME&);							\
	CLASSNAME& operator = (const CLASSNAME&);				

#define _DECLARE_SINGLETON(CLASSNAME)						\
	NO_COPY(CLASSNAME)										\
	private:												\
	static CLASSNAME*	m_pInstance;						\
	public:													\
	static CLASSNAME*	GetInstance( void );				\
	static unsigned long DestroyInstance( void );					

#define _IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
		if(NULL == m_pInstance) {							\
			m_pInstance = new CLASSNAME;					\
		}													\
		return m_pInstance;									\
	}														\
	unsigned long CLASSNAME::DestroyInstance( void ) {		\
		unsigned long dwRefCnt = 0;							\
		if(NULL != m_pInstance)	{							\
			dwRefCnt = m_pInstance->Release();				\
			if(0 == dwRefCnt) m_pInstance = NULL;			\
		}													\
		return dwRefCnt;									\
	}


#ifdef ENGINE_EXPORTS
#define _ENGINE_DLL _declspec(dllexport)
#else
#define _ENGINE_DLL _declspec(dllimport)
#endif

#define _BEGIN(NAMESPACE) namespace NAMESPACE {

#define _END }

#define _USING(NAMESPACE) using namespace NAMESPACE;

