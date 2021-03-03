#pragma once

#define _CRT_SECURE_NO_WARNINGS

//C++�W��
#include	<iostream>
#include	<iomanip>
#include	<string>
#include	<random>
#include	<vector>
#include	<list>
#include	<map>
#include	<unordered_map>
#include	<thread>
#include	<mutex>
#include	<memory>
#include	<fstream>
#include	<sstream>
#include	<cmath>
#include	<algorithm>
#include	<functional>
#include	<utility>

//Socket�֘A
#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#include	<winsock2.h>
#pragma comment (lib, "ws2_32.lib")

//OpenGL�֘A
#pragma comment (lib, "Library/lib32/GL/OpenGL32.lib")
#pragma comment (lib, "Library/lib32/GLEW/glew32.lib")
#pragma comment (lib, "Library/lib32/GLUT/glut32.lib")
#pragma comment (lib, "Library/lib32/GLFW/glfw3.lib")

#include	"../Library/include/GLEW/glew.h"
#include	"../Library/include/GLFW/glfw3.h"
#include	"../Library/include/GLM/glm.hpp"
#include	"../Library/include/GLM/mat4x4.hpp"
#include	"../Library/include/GLM/gtc/matrix_transform.hpp"

//PNG�֘A
#ifdef _DEBUG
#pragma comment (lib, "Library/lib32/libpng/libpng16d.lib")
#else
#pragma comment (lib, "Library/lib32/libpng/libpng16.lib")
#endif // DEBUG

#include	"../Library/include/libpng/png.h"

//OpenAL�֘A
#pragma comment (lib, "Library/lib32/AL/OpenAL32.lib")

#include	"../Library/include/AL/al.h"
#include	"../Library/include/AL/alc.h"

#include	<windows.h>
#include	<ks.h>
#include	<ksmedia.h>

//Json�֘A
#include	"../Library/include/nlohmann/json.hpp"

//CS�֘A
#pragma comment (lib, "Library/lib32/LibCli/LibCli.lib")

#include	"../Library/LibCli/CSScript.h"

//SAFEDELETE
#define		SAFE_DELETE(x)		{if(x){ delete x; x = nullptr;} }
#define		SAFE_DELETE_ARRAY(x){if(x){ delete[] x; x = nullptr;} }

//���O�֘A
#if _DEBUG
//Release���[�h�ł̏o�͂��K�v�ȏꍇ�͕ʓr�v���W�F�N�g�̐ݒ�Œǉ����Ă�������
#define		OUTPUTLOG
#endif

#ifdef OUTPUTLOG
#define		INFO_LOG(log)			std::cout << log << std::endl;
#define		ERROR_LOG(log)			std::cerr << log << std::endl;
#define		GL_ERROR_RETURN( estr, ret )	\
	{\
		const GLenum error = glGetError();\
		if (error != GL_NO_ERROR)\
		{\
			ERROR_LOG(estr)\
			return ret;\
		}\
	}
#else
#define		INFO_LOG(log)
#define		ERROR_LOG(log)
#define		GL_ERROR_RETURN( func_name )
#endif

namespace Sample {

	/** Enum�̐����ϊ� */
	template <typename Enumeration>
	auto EnumAsNumber(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}
};