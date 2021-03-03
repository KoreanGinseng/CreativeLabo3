#pragma once

#include	<string>
#include	<msclr/gcroot.h>
#include	<msclr/marshal_cppstd.h>
#include	"../../Framework/SharedInterface.h"

namespace Sample
{
	public ref class SharedWrapper
	{
	private:
		/** C++アクター */
		ISharedActor*		actor;
	public:
		SharedWrapper (ISharedActor* act)
		: actor(act)
		{
		}

		/**
		 * @brief		posxへのAccessor
		 * @param[in]	x	X位置
		 */
		void X(const float x) { actor->X(x); }
		/**
		 * @brief		posxへのAccessor
		 * @return		X位置
		 */
		const float X() { return actor->X(); }

		/**
		 * @brief		posyへのAccessor
		 * @param[in]	y	Y位置
		 */
		void Y(const float y) { actor->Y(y); }
		/**
		 * @brief		posxへのAccessor
		 * @return		Y位置
		 */
		const float Y() { return actor->Y(); }

		/**
		 * @brief		poszへのAccessor
		 * @param[in]	z	Z位置
		 */
		void Z(const float z) { actor->Z(z); }
		/**
		 * @brief		poszへのAccessor
		 * @return		Z位置
		 */
		const float Z() { return actor->Z(); }
	};
}
