#pragma once

#include		"NodeExecutor.h"
#include		"../SimplexNode.h"

namespace Behaviour {

	/**
	 * @brief		�P��̎q�������m�[�h�̎��s�p�N���X
	 */
	template < typename T >
	class SimplexNodeExecutor : public NodeExecutor< T >
	{
	public:
		SimplexNodeExecutor(const SimplexNodePtr< T >& node)
			: NodeExecutor< T >(node) {
			auto& ptr = node->GetChild();
			auto add = ptr->CreateExecutor();
			add->SetParent(weak_from_this());
			_children.push_back(add);
		}
		virtual ~SimplexNodeExecutor() = default;

		/**
		 * @brief		�m�[�h�̎��s��Ԃ�S�ă��Z�b�g����
		 *				��Ԃ�Inactive�ɐݒ肷��
		 */
		virtual void Reset() {
			NodeExecutor::Reset();
			for (auto& ptr : _children)
			{
				Reset();
			}
		}
	};

}