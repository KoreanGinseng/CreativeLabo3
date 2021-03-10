#pragma once

#include		"ConditionalNode.h"

namespace Behaviour {

	/**
	 * @brief		�P��̎q�������m�[�h
	 */
	template < typename T >
	class SimplexNode : public ConditionalNodeBase< T >
	{
	protected:
		/** �q�m�[�h */
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