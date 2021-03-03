#include "Framework.h"

using namespace Sample;

void ErrorCallback(int error, const char* desc)
{
	ERROR_LOG(desc);
}

Framework::Framework()
: window_(nullptr) 
, width_(0)
, height_(0) {
}

Framework::~Framework() {
	glfwTerminate();
}

bool Framework::Create(int w, int h, const char* title) {
	//GLFWの初期化
	glfwSetErrorCallback(ErrorCallback);
	if (glfwInit() != GL_TRUE)
	{
		return false;
	}
	//ウインドウの生成
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
	window_ = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (!window_)
	{
		return false;
	}
	width_ = w;
	height_ = h;
	glfwMakeContextCurrent(window_);

	//GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		ERROR_LOG("GLEWの初期化に失敗しました");
		return false;
	}

	FrameTimer::Instance().Initialize();
	AudioManager::Instance().Initialize();
	InputManager::Instance().Initialize(window_);
	RenderManager::Instance().Initialize(window_);

	//初期化終了
	INFO_LOG("Frameworkの初期化終了...");
	return true;
}

void Framework::Run() {
	Initialize();
	while (!glfwWindowShouldClose(window_))
	{
		FrameTimer::Instance().Update();
		InputManager::Instance().Update();

		Update();

		Render();
	}

	//シーン終了
	INFO_LOG("Frameworkの終了...");
	SceneManager::Instance().Release();
	ResourceManager::Instance().Release();
	InputManager::Instance().Release();
	AudioManager::Instance().Release();
	glfwDestroyWindow(window_);
}

void Framework::Initialize() {

}

void Framework::Update() {

}

void Framework::Render() {
}