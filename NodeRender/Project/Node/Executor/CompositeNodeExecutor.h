#pragma once

#include		"NodeExecutor.h"
#include		"../CompositeNode.h"

namespace Behaviour {

	/**
	 * @brief		複数の子供を持つノードの実行用クラス
	 */
	template < typename T >
	class CompositeNodeExecutor : public NodeExecutor< T >
	{
	public:
		CompositeNodeExecutor(const CompositeNodePtr< T >& node)
			: NodeExecutor< T >(node) {
			for (auto& ptr : node->GetChildren())
			{
				auto add = ptr->CreateExecutor();
				add->SetParent(weak_from_this());
				_children.push_back(add);
			}
		}
		virtual ~CompositeNodeExecutor() = default;

		/**
		 * @brief		ノードの実行状態を全てリセットして
		 *				状態をInactiveに設定する
		 */
		virtual void Reset() {
			NodeExecutor::Reset();
			for (auto& ptr : _children)
			{
				ptr->Reset();
			}
		}
	};

}