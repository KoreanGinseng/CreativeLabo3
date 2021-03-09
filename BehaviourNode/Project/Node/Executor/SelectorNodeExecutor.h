#pragma once

#include		"CompositeNodeExecutor.h"

namespace Behaviour {

	/** ノード実行用クラス */
	template < typename T >
	class SelectorNodeExecutor : public CompositeNodeExecutor< T >
	{
	public:
		SelectorNodeExecutor(const CompositeNodePtr< T >& node)
			: CompositeNodeExecutor< T >(node) {
		}
		virtual ~SelectorNodeExecutor() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		Succeeded	実行の成功
		 *				Failed		実行の失敗
		 */
		Result Exec(T& actor) override {
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
			for (auto& ptr : _children)
			{
				Result re = ptr->Exec(actor);
				if (re == Result::Sucess)
				{
					_state = State::Completed;
					return Result::Sucess;
				}
				else if (re == Result::None)
				{
					return Result::None;
				}
			}
			_state = State::Incompleted;
			return Result::Failure;
		}
	};
}