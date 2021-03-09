#pragma once

#include		"NodeExecutor.h"

namespace Behaviour {

	/** �m�[�h���s�p�N���X */
	template < typename T >
	class ConditionalNodeExecutor : public NodeExecutor< T >
	{
	public:
		ConditionalNodeExecutor(const NodePtr< T >& node)
			: NodeExecutor< T >(node) {
		}
		virtual ~ConditionalNodeExecutor() = default;

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
				_state = State::Completed;
				return Result::Sucess;
			}
			_state = State::Incompleted;
			return Result::Failure;
		}
	};
}