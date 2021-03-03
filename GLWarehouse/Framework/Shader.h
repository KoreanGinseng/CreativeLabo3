#pragma once

#include	"ResourceBase.h"

namespace Sample {

	class Shader : public ResourceBase
	{
	private:
		GLuint		ID_;
		GLint		matVP_;
		GLint		matModel_;
		GLint		texColor_;

		GLuint Compile(GLenum type, const char* fname);
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Shader();
		/**
		 * @brief		コンストラクタ
		 * @param[in]	name	シェーダー(頂点シェーダー/フラグメントシェーダーで名前指定)
		 */
		Shader(const char* name);
		/**
		 * @brief		コンストラクタ
		 * @param[in]	vname	頂点シェーダー
		 * @param[in]	fname	フラグメントシェーダー
		 */
		Shader(const char* vname, const char* fname);
		/**
		 * @brief		デストラクタ
		 */
		virtual ~Shader();

		/**
		 * @brief		シェーダーの生成
		 * @param[in]	vname	頂点シェーダー
		 * @param[in]	fname	フラグメントシェーダー
		 * @return		true	生成成功
		 *				false	生成失敗
		 */
		bool Load(const char* vname, const char* fname);
		/**
		 * @brief		シェーダーの有効化
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Enable();

		/**
		 * @brief		解放処理
		 */
		void Release();

		bool BindTexture(GLuint no, GLuint id);
		bool SetViewProjectionMatrix(const float* fv);
		bool SetModelMatrix(const float* fv);

		GLint GetUniformLocation(const char* name);

		const GLuint ID() const noexcept { return ID_; }
	};
	using ShaderPtr = std::shared_ptr< Shader >;

}