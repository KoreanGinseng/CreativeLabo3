#pragma once

#include		"NodeExecutor.h"

namespace Behaviour {

	/** ノード実行用クラス */
	template < typename T >
	class ConditionalNodeExecutor : public NodeExecutor< T >
	{
	public:
		ConditionalNodeExecutor(const NodePtr< T >& node)
			: NodeExecutor< T >(node) {
		}
		virtual ~ConditionalNodeExecutor() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		Succeeded	実行の成功
		 *				Failed		実行の失敗
		 */
		virtual Result Exec(T& actor) {
			//実行開始
			_state = State::Running;
			if (_node->Exec(actor))
			{
				_state = State::Completed;
				return Result::Sucess;
			}
			_state = State::Incompleted;
			return Result::Failure;
		}
	};
}