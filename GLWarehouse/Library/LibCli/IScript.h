#pragma once

#include "SharedWrapper.h"

namespace Sample
{
	/// <summary>
	/// �X�N���v�g�̃C���^�[�t�F�[�X�B
	/// </summary>
	public interface class IScript
	{
	public:
		virtual bool CallStart (SharedWrapper^ sharedData);
		virtual bool CallUpdate (SharedWrapper^ sharedData);
	};
}
