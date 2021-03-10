#pragma once

#include	"Actor.h"
#include	"Node/ActionNode.h"
#include	"Node/DecoratorNode.h"
#include	"Node/SelectorNode.h"
#include	"Node/SequencerNode.h"

using namespace Behaviour;

class ExecView {
private:

	using Lines = std::pair<int, std::vector<int>>;
	
	struct view_data {
		CRectangle				            _rect;
		std::string							_name;
		Lines			                    _lines;
		NodeExecutorPtr<BehaviourActorPtr>  _exec;
		view_data() {}
	};
	
	std::vector<view_data> _view;

	NodeExecutorPtr< BehaviourActorPtr > _exec;

public:

	void Init(const NodePtr< BehaviourActorPtr >& root, Vector2 pos = Vector2());

	void Exec(BehaviourActorPtr& actor);

	void Update();

	void Render();

private:

	void SetViewData(const NodeExecutorPtr<BehaviourActorPtr>& exec, Vector2 pos);
};

