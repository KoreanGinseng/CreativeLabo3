#pragma once

#include		"ConditionalNode.h"

namespace Behaviour {

	/**
	 * @brief		単一の子供を持つノード
	 */
	template < typename T >
	class SimplexNode : public ConditionalNodeBase< T >
	{
	protected:
		/** 子ノード */
		NodePtr< T >		_child;
	public:
		SimplexNode(const std::string& name)
			: ConditionalNodeBase< T >(name) {
		}
		virtual ~SimplexNode() = default;

		void SetChild(const NodePtr< T >& node) { _child = node; }
		NodePtr< T >& GetChild() { return _child; }
		const NodePtr< T >& GetChild() const { return _child; }
	};
	template < typename T > using SimplexNodePtr = std::shared_ptr<SimplexNode< T >>;
}