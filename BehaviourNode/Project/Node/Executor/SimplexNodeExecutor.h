#pragma once

#include		"NodeExecutor.h"
#include		"../SimplexNode.h"

namespace Behaviour {

	/**
	 * @brief		単一の子供を持つノードの実行用クラス
	 */
	template < typename T >
	class SimplexNodeExecutor : public NodeExecutor< T >
	{
	public:
		SimplexNodeExecutor(const SimplexNodePtr< T >& node)
			: NodeExecutor< T >(node) {
			auto& ptr = node->GetChild();
			auto add = ptr->CreateExecutor();
			add->SetParent(weak_from_this());
			_children.push_back(add);
		}
		virtual ~SimplexNodeExecutor() = default;

		/**
		 * @brief		ノードの実行状態を全てリセットして
		 *				状態をInactiveに設定する
		 */
		virtual void Reset() {
			NodeExecutor::Reset();
			for (auto& ptr : _children)
			{
				Reset();
			}
		}
	};

}