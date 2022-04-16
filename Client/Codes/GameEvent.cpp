#include "stdafx.h"
#include "..\Headers\GameEvent.h"

_USING(Client)

CGameEvent::CGameEvent()
: m_bIsUsed(false)
{
}



void CGameEvent::Free()
{

}
Engine::_bool Client::CGameEvent::Get_CompareTag(const _tchar* pEventTag)
{
	if (m_wstrEventTag == pEventTag)
		return true;
	return false;
}
