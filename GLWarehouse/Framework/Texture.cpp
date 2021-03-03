#include "Texture.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
Texture::Texture()
: ResourceBase()
, ID_(0)
, Width_(0) 
, Height_(0) 
, Format_(FormatType::Unknown) {
}

/**
 * @brief		コンストラクタ
 *				生成と同時に読み込みをおこなう
 * @param[in]	name	読み込むテクスチャ名
 */
Texture::Texture(const char* name)
: ResourceBase()
, ID_(0)
, Width_(0)
, Height_(0)
, Format_(FormatType::Unknown) {
	Load(name);
}

/**
 * @brief		デストラクタ
 */
Texture::~Texture() {
	Release();
}

/**
 * @brief		読み込み
 * @param[in]	name	読み込むテクスチャ名
 * @return		true	読み込み成功
 *				false	読み込み失敗
 */
bool Texture::Load(const char* name) {
	//png画像ファイルのロード
	png_structp sp = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	png_infop   ip = png_create_info_struct(sp);
	FILE* fp = fopen(name, "rb");
	if (!fp)
	{
		ERROR_LOG("テクスチャ" << name << "が開けません");
		return false;
	}
	png_init_io(sp, fp);
	png_read_info(sp, ip);
	int ColorType;
	int bpp;
	png_get_IHDR(sp, ip, (png_uint_32*)&Width_, (png_uint_32*)&Height_, &bpp, &ColorType, nullptr, nullptr, nullptr);
	//情報を変換して更新
	png_set_expand(sp);
	if (bpp > 8)
	{
		png_set_strip_16(sp);
	}
	if (ColorType == PNG_COLOR_TYPE_GRAY)
	{
		png_set_gray_to_rgb(sp);
	}
	png_read_update_info(sp, ip);
	//チャンネル数設定
	png_byte bits = png_get_channels(sp, ip);
	// メモリ領域確保
	int rb = png_get_rowbytes(sp, ip);
	GLubyte* data = new GLubyte[Height_ * rb];
	GLubyte** recv = new GLubyte*[Height_];
	for (int i = 0; i < Height_; i++)
	{
		recv[i] = &data[i * rb];
	}
	//読み込み
	png_read_image(sp, recv);
	png_read_end(sp, ip);
	png_destroy_read_struct(&sp, &ip, NULL);
	fclose(fp);
	delete[] recv;
	fclose(fp);

	//フォーマットを調べる
	int iFormat = GL_RGBA;
	int Format = GL_RGBA;
	int Type = GL_UNSIGNED_BYTE;
	switch (bits)
	{
	case 1:
		Format_ = FormatType::Unknown;
		iFormat = GL_LUMINANCE;
		Format = GL_LUMINANCE;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 2:
		Format_ = FormatType::Unknown;
		Type = GL_UNSIGNED_SHORT_4_4_4_4;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		break;
	case 3:
		Format_ = FormatType::R8G8B8;
		iFormat = GL_RGB;
		Format = GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 4:
		Format_ = FormatType::R8G8B8A8;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	}
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);
	glTexImage2D(GL_TEXTURE_2D, 0, iFormat, Width_, Height_, 0, Format, Type, data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// テクスチャのパラメータを設定する
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] data;
	Name_ = name;
	INFO_LOG("テクスチャ" << name << "を読み込み...");
	return true;
}

/**
 * @brief		生成
 * @param[in]	w		幅
 * @param[in]	h		高さ
 * @param[in]	format	フォーマット
 * @return		true	成功
 *				false	失敗
 */
bool Texture::Create(const int w, const int h, const FormatType format) {
	//パラメーターを保存
	Width_ = w;
	Height_ = h;
	Format_ = format;
	//生成
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);
	switch (Format_)
	{
	case FormatType::R8G8B8:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width_, Height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		break;

	case FormatType::R8G8B8A8:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width_, Height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		break;

	case FormatType::Depth:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width_, Height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	}
	// テクスチャのパラメータを設定する
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);
	GL_ERROR_RETURN("テクスチャの生成に失敗...", false);
	Name_ = "FrameBuffer";
	return true;
}

/**
 * @brief		解放処理
 */
void Texture::Release() {
	if (ID_ == 0)
	{
		return;
	}
	glDeleteTextures(1, &ID_);
	ID_ = 0;
	Width_ = 0;
	Height_ = 0;
	INFO_LOG("テクスチャ" << Name_ << "を解放...");
	Name_ = "";
}