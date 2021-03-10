#pragma once

#include		"SimplexNodeExecutor.h"

namespace Behaviour {

	/** �m�[�h���s�p�N���X */
	template < typename T >
	class DecoratorNodeExecutor : public SimplexNodeExecutor< T >
	{
	public:
		DecoratorNodeExecutor(const SimplexNodePtr< T >& node)
			: SimplexNodeExecutor< T >(node) {
		}
		virtual ~DecoratorNodeExecutor() = default;

		/**
		 * @brief		�m�[�h�̎��s����
		 * @param[in]	actor		���s�A�N�^�[
		 * @return		Succeeded	���s�̐���
		 *				Failed		���s�̎��s
		 */
		virtual Result Exec(T& actor) {
			//���s�J�n
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