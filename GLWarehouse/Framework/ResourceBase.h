#pragma once

#include	"Common.h"

namespace Sample {

	class ResourceBase
	{
	protected:
		std::string		Name_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		ResourceBase() :
		Name_() {
		}
		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~ResourceBase() {
		}

		const std::string& Name() const noexcept { return Name_; }
	};

}