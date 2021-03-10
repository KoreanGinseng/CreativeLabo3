#pragma once

#include		"Executor/ConditionalNodeExecutor.h"
#include		"ParameterNode.h"

namespace Behaviour {

	/**
	 * @brief		��������p�m�[�h
	 *				�m�[�h�ɐݒ肳�ꂽ���������s����Sucess/Failure��Ԃ�
	 */
	template < typename T >
	class ConditionalNodeBase : public Node< T >
	{
	public:
		/**
		 * @brief		�����Ŏ��s�\�Ȕ���
		 */
		enum class Operator {
			Equal,			//!<==
			NotEqual,		//!<!=
			Less,			//!<<
			LessEqual,		//!<<=
			Greater,		//!<>
			GreaterEqual,	//!<>=
		};
	public:
		ConditionalNodeBase()
			: Node< T >("Conditional") {
		}
		ConditionalNodeBase(const std::string& name)
			: Node< T >(name) {
		}
		virtual ~ConditionalNodeBase() = default;

		virtual NodeExecutorPtr< T > CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			return std::make_shared<ConditionalNodeExecutor< T >>(ptr);
		}
	};

	template < typename T, typename V >
	class ConditionalNode : public ConditionalNodeBase< T >
	{
	protected:
		/** ����q */
		Operator						_operator;
		/** ��r�p�p�����[�^�[ */
		ParameterNodePtr< T, V >		_param1;
		/** ��r�p�p�����[�^�[ */
		ParameterNodePtr< T, V >		_param2;
	public:
		ConditionalNode(Operator op, ParameterNodePtr< T, V > p1, ParameterNodePtr< T, V > p2)
			: ConditionalNodeBase< T >()
			, _operator(op)
			, _param1(p1)
			, _param2(p2) {
		}
		virtual ~ConditionalNode() = default;

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
			case Operator::Greater:			//>
				return _param1->GetValue(actor) > _param2->GetValue(actor);
			case Operator::GreaterEqual:	//>=
				return _param1->GetValue(actor) >= _param2->GetValue(actor);
			}
			return false;
		}
	};
}