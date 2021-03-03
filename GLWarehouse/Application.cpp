#pragma once

#include	"Application.h"
#include	"Game/TitleScene.h"
#include	"Game/GameScene.h"
#include	<Windows.h>

using namespace Sample;

Application::Application()
: Framework() {
}

Application::~Application() {
}

void Application::Initialize() {
	//���\�[�X�f�B���N�g����f�ޔz�u��Ɏw��
	::SetCurrentDirectory(L"Resources");
	//���L���\�[�X
	auto pack = ResourceManager::Instance().AddPack("Default");
	pack->Shaders().Add("Sprite", "Sprite.vert", "Sprite.frag");

	//�F������L�[�̓o�^
	InputManager::Instance().AddKey("Up", GLFW_KEY_UP);
	InputManager::Instance().AddKey("Up", GLFW_KEY_W);
	InputManager::Instance().AddKey("Down", GLFW_KEY_DOWN);
	InputManager::Instance().AddKey("Down", GLFW_KEY_S);
	InputManager::Instance().AddKey("Left", GLFW_KEY_LEFT);
	InputManager::Instance().AddKey("Left", GLFW_KEY_A);
	InputManager::Instance().AddKey("Right", GLFW_KEY_RIGHT);
	InputManager::Instance().AddKey("Right", GLFW_KEY_D);
	InputManager::Instance().AddKey("Enter", GLFW_KEY_ENTER);
	InputManager::Instance().AddMouseKey("Enter", GLFW_MOUSE_BUTTON_LEFT);

	//�V�[���̓o�^
	SceneManager::Instance().CreateScene<TitleScene>();
}

void Application::Update() {
	//�Ǘ��V�[����S�čX�V
	SceneManager::Instance().Update();
}

void Application::Render() {
	//�����ݒ�Ɖ�ʃN���A
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//�Ǘ��V�[����S�ĕ`��
	SceneManager::Instance().Render();

	//��ʂɕ\��
	glfwSwapBuffers(window_);
	glfwPollEvents();
}