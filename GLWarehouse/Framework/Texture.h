#pragma once

#include	"ResourceBase.h"

namespace Sample {

	class Texture : public ResourceBase
	{
	public:
		enum class FormatType {
			R8G8B8,
			R8G8B8A8,
			Depth,
			Unknown,
		};
	protected:
		GLuint			ID_;
		GLint			Width_;
		GLint			Height_;
		FormatType		Format_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Texture();

		/**
		 * @brief		コンストラクタ
		 *				生成と同時に読み込みをおこなう
		 * @param[in]	name	読み込むテクスチャ名
		 */
		Texture(const char* name);

		/**
		 * @brief		デストラクタ
		 */
		virtual ~Texture();

		/**
		 * @brief		読み込み
		 * @param[in]	name	読み込むテクスチャ名
		 * @return		true	読み込み成功
		 *				false	読み込み失敗
		 */
		bool Load(const char* name);

		/**
		 * @brief		生成
		 * @param[in]	w		幅
		 * @param[in]	h		高さ
		 * @param[in]	format	フォーマット
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Create(const int w, const int h, const FormatType format);

		/**
		 * @brief		解放処理
		 */
		void Release();

		const GLuint ID() const noexcept { return ID_; }
		const GLint Width() const noexcept { return Width_; }
		const GLint Height() const noexcept { return Height_; }

	};
	using TexturePtr = std::shared_ptr< Texture >;

}