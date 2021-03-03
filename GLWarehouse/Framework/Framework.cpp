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
	//GLFW�̏�����
	glfwSetErrorCallback(ErrorCallback);
	if (glfwInit() != GL_TRUE)
	{
		return false;
	}
	//�E�C���h�E�̐���
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

	//GLEW�̏�����
	if (glewInit() != GLEW_OK)
	{
		ERROR_LOG("GLEW�̏������Ɏ��s���܂���");
		return false;
	}

	FrameTimer::Instance().Initialize();
	AudioManager::Instance().Initialize();
	InputManager::Instance().Initialize(window_);
	RenderManager::Instance().Initialize(window_);

	//�������I��
	INFO_LOG("Framework�̏������I��...");
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

	//�V�[���I��
	INFO_LOG("Framework�̏I��...");
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