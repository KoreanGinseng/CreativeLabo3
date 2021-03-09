#pragma once

#include		"INodeExecutor.h"
#include		"../Node.h"

namespace Behaviour {

	/** ノード実行用クラス */
	template < typename T >
	class NodeExecutor : public INodeExecutor< T >
	{
	protected:
		/** 状態 */
		State						_state;
		/** 対応ノード */
		NodePtr< T >				_node;
		/** 親ノード */
		NodeExecutorWeakPtr< T >	_parent;
		/** 子ノード */
		NodeExecutorList< T >		_children;
	public:
		NodeExecutor(const NodePtr< T >& node)
		: _state(State::Inactive) 
		, _node(node)
		, _parent()
		, _children() {
		}
		virtual ~NodeExecutor() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		Succeeded	実行の成功
		 *				Failed		実行の失敗
		 */
		virtual Result Exec(T& actor) { return Result::Failure; }

		/**
		 * @brief		ノードの実行状態を全てリセットして
		 *				状態をInactiveに設定する
		 */
		virtual void Reset() {
			_state = State::Inactive;
		}

		/**
		 * @brief		親の設定
		 * @param[in]	ptr			親アクター
		 */
		void SetParent(NodeExecutorWeakPtr< T >& ptr) override {
			_parent = ptr;
		}
	};
}