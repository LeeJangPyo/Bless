#pragma once

const unsigned short g_nBackCX = 800;
const unsigned short g_nBackCY = 600;

enum SCENEID {SCENE_STATIC, SCENE_EDIT, SCENE_END};
enum CAMETATYPE { CAMERA_STATIC, CAMERA_DYANMIC, CAMERA_CUTSCENE, CAMERA_END };
extern HINSTANCE g_hInst;
extern HWND g_hWnd;

#define	VTXCNTX 256
#define	VTXCNTZ 256
#define VTXITV 1

#define COOLDOWN_END 1

const float		g_fDefault_Sacle = 0.1f;