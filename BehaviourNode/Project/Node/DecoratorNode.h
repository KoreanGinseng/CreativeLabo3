#pragma once

#include		"SimplexNode.h"
#include		"Executor/DecoratorNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		条件判定用ノード
	 *				ノードに設定された条件を実行してSucess/Failureを返す
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
		/** 判定子 */
		Operator						_operator;
		/** 比較用パラメーター */
		ParameterNodePtr< T, V >		_param1;
		/** 比較用パラメーター */
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
			case Operator::Greater:		//>
				return _param1->GetValue(actor) > _param2->GetValue(actor);
			case Operator::GreaterEqual:	//>=
				return _param1->GetValue(actor) >= _param2->GetValue(actor);
			}
			return false;
		}
	};
}