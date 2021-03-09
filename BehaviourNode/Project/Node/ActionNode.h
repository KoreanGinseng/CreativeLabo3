#pragma once

#include		"Executor/ActionNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		処理実行ノード
	 *				継承して処理を作成する
	 */
	template < typename T >
	class ActionNodeBase : public Node< T >
	{
	protected:
	public:
		ActionNodeBase()
			: Node< T >("Action") {
		}
		virtual ~ActionNodeBase() = default;

		virtual NodeExecutorPtr< T > CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			return std::make_shared<ActionNodeExecutor< T >>(ptr);
		}
	};

	/**
	 * @brief		アクターの関数実行用ノード
	 */
	template < typename T >
	class FunctionNode : public ActionNodeBase< T >
	{
	protected:
		/** 実行処理 */
		using OnExecFunction = std::function< bool (T&) >;
		/** 実行処理 */
		OnExecFunction			OnExec;
	public:
		FunctionNode(OnExecFunction exec)
			: ActionNodeBase< T >()
			, OnExec(exec) {
		}
		virtual ~FunctionNode() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		true		実行の成功
		 *				false		実行の失敗
		 */
		virtual bool Exec(T& actor) {
			return OnExec(actor);
		}
	};
}