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
		/** C++�A�N�^�[ */
		ISharedActor*		actor;
	public:
		SharedWrapper (ISharedActor* act)
		: actor(act)
		{
		}

		/**
		 * @brief		posx�ւ�Accessor
		 * @param[in]	x	X�ʒu
		 */
		void X(const float x) { actor->X(x); }
		/**
		 * @brief		posx�ւ�Accessor
		 * @return		X�ʒu
		 */
		const float X() { return actor->X(); }

		/**
		 * @brief		posy�ւ�Accessor
		 * @param[in]	y	Y�ʒu
		 */
		void Y(const float y) { actor->Y(y); }
		/**
		 * @brief		posx�ւ�Accessor
		 * @return		Y�ʒu
		 */
		const float Y() { return actor->Y(); }

		/**
		 * @brief		posz�ւ�Accessor
		 * @param[in]	z	Z�ʒu
		 */
		void Z(const float z) { actor->Z(z); }
		/**
		 * @brief		posz�ւ�Accessor
		 * @return		Z�ʒu
		 */
		const float Z() { return actor->Z(); }
	};
}
