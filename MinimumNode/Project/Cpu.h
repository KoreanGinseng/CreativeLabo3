#pragma once

#include	"Character.h"

class CCpu : public CCharacter {
private:
	float						m_CpuTime;
	CCharacter*					m_pTarget;

	//ƒm[ƒhŠî‘b
	class Node {
	public:
		virtual void Exec(CCpu* target) = 0;
	};
	//“®ìƒm[ƒh
	class ActionNode : public Node {
	public:
		std::function<void(CCpu*)>	m_Task;
		void Exec(CCpu* target) override {
			m_Task(target);
		}
	};
	//”»’èƒm[ƒh
	class ConditionNode : public Node {
	public:
		std::function<bool(CCpu*)>	m_ConditionTask;
		Node* Yes;
		Node* No;
		void Exec(CCpu* target) override {
			if (m_ConditionTask(target))
			{
				if (Yes) { Yes->Exec(target); }
			}
			else
			{
				if (No) { No->Exec(target); }
			}
		}
	};
	Node* root;
public:
	CCpu();
	virtual ~CCpu();
	virtual void Initialize();
	virtual void Update(CCharacter& chara,LPMeshContainer pMesh);

	bool CpuDistance();
	bool CpuIsEnemyAttack();
	void CpuGuard();
	void CpuAttack();
	void CpuMove();
};