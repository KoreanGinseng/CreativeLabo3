#pragma once

#include		"CompositeNode.h"
#include		"Executor/SequencerNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		シーケンス実行用ノード
	 *				順番にノードを実行しどれか一つでもFailureが返ってきた時点で終了する
	 */
	template < typename T >
	class SequencerNode : public CompositeNode< T >
	{
	protected:
	public:
		SequencerNode()
			: CompositeNode< T >("Sequencer") {
		}
		virtual ~SequencerNode() = default;

		virtual NodeExecutorPtr< T > CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			auto temp = std::dynamic_pointer_cast<CompositeNode< T >>(ptr);
			return std::make_shared<SequencerNodeExecutor< T >>(temp);
		}
	};
}