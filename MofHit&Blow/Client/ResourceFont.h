#pragma once
#include "Mof.h"

namespace sip
{
	// ********************************************************************************
	/// <summary>
	/// アセットクラスに対応させるためだけのフォントクラス
	/// </summary>
	// ********************************************************************************
	class CFontA : public CFont
	{
	public:
		// ********************************************************************************
		/// <summary>
		/// 名前のラップ
		/// </summary>
		/// <param name="pName">フォント名</param>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool Load(LPCMofChar pName);
	};

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	// ********************************************************************************
	class CResourceFont : public CFont
	{
	protected:
		
		HFONT       m_hResourceFontHandle;  //! ファイルから読み込むハンドル

		LOGFONT     m_FontInfo;             //! フォント生成情報

		std::string m_FileName;             //! 登録するファイル名

	public:

		// ********************************************************************************
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		explicit CResourceFont(void);
		
		// ********************************************************************************
		/// <summary>
		/// デストラクタ
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual ~CResourceFont(void) override;

		// ********************************************************************************
		/// <summary>
		/// ファイルの読み込み
		/// </summary>
		/// <param name="pFile">読み込むファイル名\0フォント名\0\0</param>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual bool Load(LPCMofChar pFileName);

		// ********************************************************************************
		/// <summary>
		/// サイズの設定
		/// </summary>
		/// <param name="s">フォントサイズ</param>
		/// <returns>true : 成功, それ以外 : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual MofBool SetSize(MofS32 sx, MofS32 sy);

		// ********************************************************************************
		/// <summary>
		/// フォントハンドルの後始末
		/// </summary>
		/// <returns>true : 成功, それ以外 : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual MofBool DeleteHandle(void);
	};
}

