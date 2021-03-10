#include "ExecView.h"

void ExecView::Init(const NodePtr<BehaviourActorPtr>& root, Vector2 pos) {
	//“K“–‚É“®ìƒm[ƒh‚ð¶¬‚·‚é
	if (_exec == nullptr) {
		_exec = root->CreateExecutor();
		SetViewData(_exec, pos);
	}
	else {
		_exec->Reset();
	}
}

void ExecView::Exec(BehaviourActorPtr & actor) {
	_exec->Exec(actor);
}

void ExecView::Update() {
	Vector2 mp;
	Vector2 mm;
	g_pInput->GetMousePos(mp);
	g_pInput->GetMouseMove(mm.x, mm.y);
	int rect_cnt = _view.size();
	for (auto& itr = std::rbegin(_view); itr != std::rend(_view); ++itr) {
		if (itr->_rect.CollisionPoint(mp) && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
			itr->_rect.Translation(mm);
			break;
		}
	}
}

void ExecView::Render() {
	for (const auto& itr : _view) {
		for (const auto& itr2 : itr._lines.second) {
			Vector2 start = _view[itr._lines.first]._rect.GetCenter();
			Vector2 end = _view[itr2]._rect.GetCenter();
			CGraphicsUtilities::RenderLine(start, end, MOF_COLOR_WHITE);
		}
	}
	int rect_cnt = _view.size();
	for (const auto& itr : _view) {
		MofU32 color = MOF_COLOR_CWHITE;
		if (itr._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Running) {
			color = MOF_COLOR_YELLOW;
		}
		else if (itr._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Completed) {
			color = MOF_COLOR_GREEN;
		}
		else if (itr._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Incompleted) {
			color = MOF_COLOR_RED;
		}
		CGraphicsUtilities::RenderFillRect(itr._rect, color);
		CGraphicsUtilities::RenderString((Mof::Rectangle)itr._rect, itr._name.c_str());
	}
}

void ExecView::SetViewData(const NodeExecutorPtr<BehaviourActorPtr>& exec, Vector2 pos) {
	int i = _view.size();
	_view.push_back(view_data());
	_view[i]._rect = (CRectangle(pos.x, pos.y, pos.x + 100, pos.y + 50));
	_view[i]._name = (exec->GetNode()->GetName());
	_view[i]._lines.first = i;
	_view[i]._exec = exec;
	auto& child = exec->GetChild();
	for (const auto& itr : child) {
		int last = _view.size();
		SetViewData(itr, pos);
		_view[i]._lines.second.push_back(last);
	}
}