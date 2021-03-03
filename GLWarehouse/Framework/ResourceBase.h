#pragma once

#include	"Common.h"

namespace Sample {

	class ResourceBase
	{
	protected:
		std::string		Name_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		ResourceBase() :
		Name_() {
		}
		/**
		 * @brief		デストラクタ
		 */
		virtual ~ResourceBase() {
		}

		const std::string& Name() const noexcept { return Name_; }
	};

}