#pragma once

#include		"Executor/ConditionalNodeExecutor.h"
#include		"ParameterNode.h"

namespace Behaviour {

	/**
	 * @brief		条件判定用ノード
	 *				ノードに設定された条件を実行してSucess/Failureを返す
	 */
	template < typename T >
	class ConditionalNodeBase : public Node< T >
	{
	public:
		/**
		 * @brief		内部で実行可能な判定
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
		/** 判定子 */
		Operator						_operator;
		/** 比較用パラメーター */
		ParameterNodePtr< T, V >		_param1;
		/** 比較用パラメーター */
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
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		true		実行の成功
		 *				false		実行の失敗
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