#include "Mesh.h"


Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
	, m_bClone(false)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	: CComponent(rhs)
	, m_bClone(true)
{

}



void Engine::CMesh::Free(void)
{
	Engine::CComponent::Free();
}

