#pragma once

#include		"CompositeNodeExecutor.h"

namespace Behaviour {

	/** �m�[�h���s�p�N���X */
	template < typename T >
	class SequencerNodeExecutor : public CompositeNodeExecutor< T >
	{
	public:
		SequencerNodeExecutor(const CompositeNodePtr< T >& node)
			: CompositeNodeExecutor< T >(node) {
		}
		virtual ~SequencerNodeExecutor() = default;

		/**
		 * @brief		�m�[�h�̎��s����
		 * @param[in]	actor		���s�A�N�^�[
		 * @return		Succeeded	���s�̐���
		 *				Failed		���s�̎��s
		 */
		Result Exec(T& actor) override {
			//���s�ς�
			if (_state == State::Completed)
			{
				return Result::Sucess;
			}
			else if (_state == State::Incompleted)
			{
				return Result::Failure;
			}
			//���s�J�n
			_state = State::Running;
			for (auto& ptr : _children)
			{
				Result re = ptr->Exec(actor);
				if (re == Result::Failure)
				{
					_state = State::Incompleted;
					return Result::Failure;
				}
				else if (re == Result::None)
				{
					return Result::None;
				}
			}
			_state = State::Completed;
			return Result::Sucess;
		}
	};
}