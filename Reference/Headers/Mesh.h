#ifndef Mesh_h__
#define Mesh_h__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CMesh abstract: public CComponent
{
protected:
	explicit	CMesh(LPDIRECT3DDEVICE9	pDevice);
	explicit	CMesh(const CMesh& rhs);
	virtual		~CMesh(void) = default;

protected:
	_bool		m_bClone;

public:
	virtual		CComponent*		Clone_Component(void) = 0;
	virtual		void			Free(void);

};

_END



#endif // Mesh_h__
