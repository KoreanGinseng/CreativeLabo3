#pragma once

#include		"CompositeNode.h"
#include		"Executor/SequencerNodeExecutor.h"

namespace Behaviour {

	/**
	 * @brief		�V�[�P���X���s�p�m�[�h
	 *				���ԂɃm�[�h�����s���ǂꂩ��ł�Failure���Ԃ��Ă������_�ŏI������
	 */
	template < typename T >
	class SequencerNode : public CompositeNode< T >
	{
	protected:
	public:
		SequencerNode()
			: CompositeNode< T >("Sequencer") {
		}
		virtual ~SequencerNode() = default;

		virtual NodeExecutorPtr< T > CreateExecutor() const {
			NodePtr< T > ptr = std::const_pointer_cast<Node< T >>(shared_from_this());
			auto temp = std::dynamic_pointer_cast<CompositeNode< T >>(ptr);
			return std::make_shared<SequencerNodeExecutor< T >>(temp);
		}
	};
}