#pragma once

#include	"GameDefine.h"

class CStage {
private:
	CMeshContainer				m_Mesh;
	CDirectionalLight			m_Light;

	CMeshContainer				m_FarMesh;

	CMeshContainer				m_SkyMesh;
	CMeshContainer				m_Cloud1Mesh;
	float						m_Cloud1Rotate;
	CMeshContainer				m_Cloud2Mesh;
	float						m_Cloud2Rotate;
	CMeshContainer				m_Cloud3Mesh;
	float						m_Cloud3Rotate;

	CMeshContainer				m_CollisionMesh;
public:
	CStage();
	virtual ~CStage();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	void Release();

	LPMeshContainer GetCollisionMesh(void){ return &m_CollisionMesh; }
};