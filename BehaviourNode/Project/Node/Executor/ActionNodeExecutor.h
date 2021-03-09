#pragma once

#include		"NodeExecutor.h"

namespace Behaviour {

	/** ノード実行用クラス */
	template < typename T >
	class ActionNodeExecutor : public NodeExecutor< T >
	{
	public:
		ActionNodeExecutor(const NodePtr< T >& node)
			: NodeExecutor< T >(node) {
		}
		virtual ~ActionNodeExecutor() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		Succeeded	実行の成功
		 *				Failed		実行の失敗
		 */
		virtual Result Exec(T& actor) {
			//実行済み
			if (_state == State::Completed)
			{
				return Result::Sucess;
			}
			else if (_state == State::Incompleted)
			{
				return Result::Failure;
			}
			//実行開始
			_state = State::Running;
			if (_node->Exec(actor))
			{
				_state = State::Completed;
				return Result::Sucess;
			}
			return Result::None;
		}
	};
}