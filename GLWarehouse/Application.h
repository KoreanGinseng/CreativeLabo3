#pragma once

#include	"Framework/Framework.h"

namespace Sample {

	class Application : public Framework {
	protected:
	public:
		Application();
		virtual ~Application();

		void Initialize() override;

		void Update() override;

		void Render() override;
	};
}