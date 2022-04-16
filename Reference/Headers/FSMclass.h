#ifndef FSMClass_h__
#define FSMClass_h__


#include "Base.h"

_BEGIN(Engine)
class CFSMstate;

typedef map< int, CFSMstate*, less<int> > State_Map;
typedef State_Map::value_type SM_VT;

class _ENGINE_DLL CFSMclass : public CBase
{
public:
	explicit CFSMclass();
	virtual ~CFSMclass() = default;

public:
	// return the current state ID
	int GetCurrentState() { return m_iCurrentState; }
	// set current state
	void SetCurrentState(int iStateID) { m_iCurrentState = iStateID; }

	CFSMstate *GetState(int iStateID);	// return the FSMstate object pointer
	void AddState(CFSMstate *pState);	// add a FSMstate object pointer to the map
	void DeleteState(int iStateID);	// delete a FSMstate object pointer from the map

	int StateTransition(int iInput);	// perform a state transition based on input and current state

private:
	HRESULT	Ready_FSMclass(_int iStateID);

private:
	State_Map m_mapState;		// map containing all states of this FSM
	int m_iCurrentState;	// the m_iStateID of the current state
public:
	static CFSMclass*	Create(_int iStateID);

protected:
	virtual void Free() override;

};

_END
#endif // FSMClass_h__