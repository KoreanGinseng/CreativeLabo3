#pragma once

#include		"CompositeNode.h"
#include		"Executor/SelectorNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		セレクター実行用ノード
	 *				順番にノードを実行しどれか一つでもSucessが返ってきた時点で終了する
	 */
	template < typename T >
	class SelectorNode : public CompositeNode< T >
	{
	protected:
	public:
		SelectorNode()
			: CompositeNode< T >("Selector") {
		}
		virtual ~SelectorNode() = default;

		virtual NodeExecutorPtr<T> CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			auto temp = std::dynamic_pointer_cast<CompositeNode< T >>(ptr);
			return std::make_shared<SelectorNodeExecutor< T >>(temp);
		}
	};
}