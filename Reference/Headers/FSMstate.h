#ifndef FSMstate_h__
#define FSMstate_h__


#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CFSMstate : public CBase
{
public:
	explicit CFSMstate();
	virtual ~CFSMstate() = default;

public:
	// access the state ID
	_int GetID() { return m_iStateID; }
	// add a state transition to the array
	void AddTransition(_int iInput, _int iOutputID);
	// remove a state transation from the array
	void DeleteTransition(_int iOutputID);
	// get the output state and effect a transistion
	_int GetOutput(_int iInput);
private:
	HRESULT	 Ready_State(_int iStateID, _uint uiTransitions);
private:
	_uint m_uiNumberOfTransistions;	// maximum number of states supported by this state
	_int *m_piInputs;					// input array for tranistions
	_int *m_piOutputState;				// output state array
	_int m_iStateID;						// the unique ID of this state


public:
	static CFSMstate*	Create(_int iStateID, _uint uiTransitions);
protected:
	virtual void Free() override;

};

_END
#endif // FSMstate_h__
