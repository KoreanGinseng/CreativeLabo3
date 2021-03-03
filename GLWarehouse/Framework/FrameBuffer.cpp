#include "FrameBuffer.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
FrameBuffer::FrameBuffer()
: ID_(0)
, Width_(0)
, Height_(0)
, Color_() 
, Depth_() {
}

/**
 * @brief		デストラクタ
 */
FrameBuffer::~FrameBuffer() {
	Release();
}

/**
 * @brief		生成
 * @param[in]	w		幅
 * @param[in]	h		高さ
 * @return		true	成功
 *				false	失敗
 */
bool FrameBuffer::Create(const int w, const int h) {
	//サイズ保存
	Width_ = w;
	Height_ = h;
	// フレームバッファオブジェクト、デプスとカラー用のテクスチャを作成
	glGenFramebuffersEXT(1, &ID_);
	GL_ERROR_RETURN("フレームバッファの作成に失敗...", false);
	Color_ = std::make_shared<Texture>();
	Color_->Create(w, h, Texture::FormatType::R8G8B8A8);
	Depth_ = std::make_shared<Texture>();
	Depth_->Create(w, h, Texture::FormatType::Depth);

	// テクスチャをフレームバッファオブジェクトにバインド
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID_);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Color_->ID(), 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, Depth_->ID(), 0);
	GL_ERROR_RETURN("フレームバッファのバインドに失敗...", false);

	// デフォルトのレンダーターゲットに戻す
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

/**
 * @brief		バインド
 * @return		true	成功
 *				false	失敗
 */
bool FrameBuffer::Bind() {
	// バッファを変更
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID_);
	GL_ERROR_RETURN("フレームバッファのバインドに失敗...", false);
	glViewport(0, 0, Width_, Height_);
	return true;
}

/**
 * @brief		解放処理
 */
void FrameBuffer::Release() {
	if (ID_ == 0)
	{
		return;
	}
	glDeleteFramebuffersEXT(1, &ID_);
	ID_ = 0;
	Color_->Release();
	Depth_->Release();
}