#ifndef Navigation_Line_h__
#define Navigation_Line_h__

#include "Base.h"


_BEGIN(Engine)

class _ENGINE_DLL CNavigation_Line : public CBase
{
public:
	enum POINT	{POINT_START, POINT_FINISH, POINT_END};
	//점 기준으로 왼쪽에 점이 있는지 오른쪽에 있는지 저장 변수
	enum COMPARE { COMPARE_OUT, COMPARE_IN };

private:
	explicit	CNavigation_Line(void);
	virtual		~CNavigation_Line(void);


public://Get
	_vec3&		Get_Normal(void) { return m_vNormal; }
	_vec3&		Get_Point(POINT eType) { return m_vPoint[eType]; }
public:
	HRESULT		Ready_Line(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	//Out : pSliderDir
	COMPARE		Compare(const _vec3* pEndPos);

private:
	_vec3		m_vPoint[POINT_END];
	_vec3		m_vDirection;
	_vec3		m_vNormal;


public:
	static	CNavigation_Line*	Create(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual	void	Free(void);
};


_END


#endif // Line_h__
