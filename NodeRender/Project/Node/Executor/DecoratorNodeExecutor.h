#pragma once

#include		"SimplexNodeExecutor.h"

namespace Behaviour {

	/** ノード実行用クラス */
	template < typename T >
	class DecoratorNodeExecutor : public SimplexNodeExecutor< T >
	{
	public:
		DecoratorNodeExecutor(const SimplexNodePtr< T >& node)
			: SimplexNodeExecutor< T >(node) {
		}
		virtual ~DecoratorNodeExecutor() = default;

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
					else
					{
						_state = State::Incompleted;
						return Result::Failure;
					}
				}
			}
			_state = State::Incompleted;
			return Result::Failure;
		}
	};
}