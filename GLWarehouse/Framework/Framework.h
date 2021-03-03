#pragma once

#include	"FrameTimer.h"
#include	"InputManager.h"
#include	"SceneManager.h"
#include	"Sound/AudioManager.h"
#include	"ResourceManager.h"

namespace Sample {

	class Framework
	{
	protected:
		GLFWwindow*		window_;
		int				width_;
		int				height_;
	public:
		Framework();
		virtual ~Framework();

		bool Create(int w, int h, const char* title);

		virtual void Run();

		virtual void Initialize();

		virtual void Update();

		virtual void Render();
	};

}