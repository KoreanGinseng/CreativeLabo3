#include "Shader.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
Shader::Shader() 
: ResourceBase()
, ID_(0)
, matVP_(-1)
, matModel_(-1)
, texColor_(-1) {
}

/**
 * @brief		コンストラクタ
 * @param[in]	name	シェーダー(頂点シェーダー/フラグメントシェーダーで名前指定)
 */
Shader::Shader(const char* name)
: ResourceBase()
, ID_(0)
, matVP_(-1)
, matModel_(-1)
, texColor_(-1) {
	std::string tstr = name;
	int pos = tstr.find("/", 0);
	std::string vname = tstr.substr(0, pos);
	std::string fname = tstr.substr(pos + 1, tstr.length() - pos);
	Load(vname.c_str(), fname.c_str());
}
/**
 * @brief		コンストラクタ
 * @param[in]	vname	頂点シェーダー
 * @param[in]	fname	フラグメントシェーダー
 */
Shader::Shader(const char* vname, const char* fname) 
: ResourceBase()
, ID_(0)
, matVP_(-1)
, matModel_(-1)
, texColor_(-1) {
	Load(vname, fname);
}

/**
 * @brief		デストラクタ
 */
Shader::~Shader() {
	Release();
}

GLuint Shader::Compile(GLenum type, const char* name) {
	FILE* fp = fopen(name, "rb");
	if (!fp)
	{
		ERROR_LOG("シェーダー" << name << "が開けません");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	const size_t length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buf = new char[length + 1];
	fread(buf, length, 1, fp);
	buf[length] = '\0';

	//シェーダーのコンパイル
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &buf, nullptr);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint info = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info);
		if (info > 0)
		{
			char* buf = new char[info];
			glGetShaderInfoLog(shader, info, NULL, buf);
			ERROR_LOG("シェーダーのコンパイルに失敗...\n" << buf);
		}
		glDeleteShader(shader);
		delete[] buf;
		return 0;
	}
	delete[] buf;
	return shader;
}

/**
 * @brief		シェーダーの生成
 * @param[in]	vname	頂点シェーダー
 * @param[in]	fname	フラグメントシェーダー
 * @return		true	生成成功
 *				false	生成失敗
 */
bool Shader::Load(const char* vname, const char* fname){
	//シェーダーのコンパイル
	GLuint vs = Compile(GL_VERTEX_SHADER, vname);
	GLuint fs = Compile(GL_FRAGMENT_SHADER, fname);
	if (!vs || !fs)
	{
		return false;
	}
	ID_ = glCreateProgram();
	glAttachShader(ID_, fs);
	glDeleteShader(fs);
	glAttachShader(ID_, vs);
	glDeleteShader(vs);
	glLinkProgram(ID_);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(ID_, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint info = 0;
		glGetProgramiv(ID_, GL_INFO_LOG_LENGTH, &info);
		if (info > 0)
		{
			char* buf = new char[info];
			glGetProgramInfoLog(ID_, info, NULL, buf);
			ERROR_LOG("シェーダーのリンクに失敗...\n" << buf);
		}
		glDeleteProgram(ID_);
		return false;
	}

	glUseProgram(ID_);
	matVP_ = GetUniformLocation("matVP");
	matModel_ = GetUniformLocation("matModel");
	texColor_ = GetUniformLocation("texColor");
	glUniform1i(texColor_, 0);
	glUseProgram(0);

	Name_ = vname;
	Name_ += "/";
	Name_ += fname;
	INFO_LOG("シェーダー" << Name_ << "を読み込み...");
	return true;
}

/**
 * @brief		シェーダーの有効化
 * @return		true	成功
 *				false	失敗
 */
bool Shader::Enable() {
	glUseProgram(ID_);
	return true;
}

bool Shader::BindTexture(GLuint no, GLuint id) {
	glActiveTexture(GL_TEXTURE0 + no);
	glBindTexture(GL_TEXTURE_2D, id);
	return true;
}

bool Shader::SetViewProjectionMatrix(const float* fv) {
	glUniformMatrix4fv(matVP_, 1, GL_FALSE, fv);
	return true;
}

bool Shader::SetModelMatrix(const float* fv) {
	glUniformMatrix4fv(matModel_, 1, GL_FALSE, fv);
	return true;
}

GLint Shader::GetUniformLocation(const char* name) {
	return glGetUniformLocation(ID_, name);
}

/**
 * @brief		解放処理
 */
void Shader::Release() {
	if (ID_ == 0)
	{
		return;
	}
	glDeleteProgram(ID_);
	ID_ = 0;
	INFO_LOG("シェーダー" << Name_ << "を解放...");
	Name_ = "";
}