#pragma once

#include	"Character.h"

class CCpu : public CCharacter {
private:
	float						m_CpuTime;

	std::function<bool()>       m_Condition[2];
	std::function<void()>       m_Task[3];

	CCharacter*                 m_pTarget = nullptr;

	class Node {
	public:
		virtual void Exec(CCpu* target) = 0;
	};

	class ActionNode : public Node {
	public:
		std::function<void(CCpu*)> m_Task;
		virtual void Exec(CCpu* target) override {
			m_Task(target);
		}
	};

	class ConditionNode : public Node {
	public:
		std::function<bool(CCpu*)> m_Condition;
		std::shared_ptr<Node> Yes;
		std::shared_ptr<Node> No;
		virtual void Exec(CCpu* target) override {
			if (m_Condition(target)) {
				Yes->Exec(target);
			}
			else {
				No->Exec(target);
			}
		}
	};

	std::shared_ptr<Node> root;

public:
	CCpu();
	virtual ~CCpu();
	virtual void Initialize();
	virtual void Update(CCharacter& chara,LPMeshContainer pMesh);

	bool AI_Distance();
	bool AI_IsEnemyAttack();

	void AI_Guard();
	void AI_Attack();
	void AI_Move();
};