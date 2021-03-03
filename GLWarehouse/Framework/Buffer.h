#pragma once

#include	"Common.h"

namespace Sample {

	class Buffer
	{
	private:
		GLuint			ID_;
		GLenum			Type_;

	public:
		/**
		 * @brief		コンストラクタ
		 */
		Buffer();
		/**
		 * @brief		デストラクタ
		 */
		virtual ~Buffer();

		/**
		 * @brief		バッファの生成
		 * @param[in]	type	バッファの種類
		 * @param[in]	size	バッファサイズ
		 * @param[in]	data	バッファに格納するデータ
		 * @param[in]	usage	バッファの使用法
		 * @return		true	生成成功
		 *				false	生成失敗
		 */
		bool Create(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage);
		/**
		 * @brief		バッファへのデータ書き込み
		 * @param[in]	offset	書き込みオフセット
		 * @param[in]	size	書き込みサイズ
		 * @param[in]	data	バッファに格納するデータ
		 * @return		true	書き込み成功
		 *				false	書き込み失敗
		 */
		bool SubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

		const GLuint ID() const noexcept { return ID_; }
	};

}