#pragma once

#include		"SimplexNode.h"
#include		"Executor/DecoratorNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		��������p�m�[�h
	 *				�m�[�h�ɐݒ肳�ꂽ���������s����Sucess/Failure��Ԃ�
	 */
	template < typename T >
	class DecoratorNodeBase : public SimplexNode< T >
	{
	public:
		DecoratorNodeBase()
			: SimplexNode< T >("Decorator") {
		}
		virtual ~DecoratorNodeBase() = default;

		virtual NodeExecutorPtr< T > CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			auto temp = std::dynamic_pointer_cast<DecoratorNodeBase< T >>(ptr);
			return std::make_shared<DecoratorNodeExecutor< T >>(temp);
		}
	};

	template < typename T, typename V >
	class DecoratorNode : public DecoratorNodeBase< T >
	{
	protected:
		/** ����q */
		Operator						_operator;
		/** ��r�p�p�����[�^�[ */
		ParameterNodePtr< T, V >		_param1;
		/** ��r�p�p�����[�^�[ */
		ParameterNodePtr< T, V >		_param2;
	public:
		DecoratorNode(Operator op, ParameterNodePtr< T, V > p1, ParameterNodePtr< T, V > p2)
			: DecoratorNodeBase< T >()
			, _operator(op)
			, _param1(p1)
			, _param2(p2) {
		}
		virtual ~DecoratorNode() = default;

		/**
		 * @brief		�m�[�h�̎��s����
		 * @param[in]	actor		���s�A�N�^�[
		 * @return		true		���s�̐���
		 *				false		���s�̎��s
		 */
		virtual bool Exec(T& actor) {
			switch (_operator)
			{
			case Operator::Equal:			//==
				return _param1->GetValue(actor) == _param2->GetValue(actor);
			case Operator::NotEqual:		//!=
				return _param1->GetValue(actor) != _param2->GetValue(actor);
			case Operator::Less:			//<
				return _param1->GetValue(actor) < _param2->GetValue(actor);
			case Operator::LessEqual:		//<=
				return _param1->GetValue(actor) <= _param2->GetValue(actor);
			case Operator::Greater:		//>
				return _param1->GetValue(actor) > _param2->GetValue(actor);
			case Operator::GreaterEqual:	//>=
				return _param1->GetValue(actor) >= _param2->GetValue(actor);
			}
			return false;
		}
	};
}