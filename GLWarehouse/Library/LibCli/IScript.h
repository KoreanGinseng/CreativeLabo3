#pragma once

#include "SharedWrapper.h"

namespace Sample
{
	/// <summary>
	/// スクリプトのインターフェース。
	/// </summary>
	public interface class IScript
	{
	public:
		virtual bool CallStart (SharedWrapper^ sharedData);
		virtual bool CallUpdate (SharedWrapper^ sharedData);
	};
}
