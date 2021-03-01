#pragma once
#include "Mof.h"

namespace sip
{
	// ********************************************************************************
	/// <summary>
	/// ファイルダイアログ関連のクラス
	/// </summary>
	// ********************************************************************************
	class CFileDialog
	{
	public:

		// ********************************************************************************
		/// <summary>
		/// ダイアログのモード
		/// </summary>
		// ********************************************************************************
		enum class Mode
		{
			Open,
			Save,
		};

	private:

		static std::string m_sProjectPath; //! プロジェクトパス

		CFileDialog(void)  = delete;       //! スタティッククラスの宿命
	
		~CFileDialog(void) = delete;       //! スタティッククラスの宿命

	public:

		// ********************************************************************************
		/// <summary>
		/// 
		/// </summary>
		/// <param name="hWnd">ハンドル</param>
		/// <param name="mode">ダイアログのモード</param>
		/// <param name="Title">ダイアログのタイトル</param>
		/// <param name="Filter">開くファイルのフィルタ</param>
		/// <param name="DefExt">開くファイルの拡張子</param>
		/// <param name="path">パスの出力先</param>
		/// <param name="bOutArray">複数ファイルが選択されているかのフラグ出力先</param>
		/// <returns></returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static bool Open(HWND hWnd, Mode mode, const char* Title, const char* Filter, const char* DefExt, char* path, bool& bOutArray);

		// ********************************************************************************
		/// <summary>
		/// 拡張子の取得
		/// </summary>
		/// <param name="path">拡張子の取りたいファイル</param>
		/// <returns>拡張子</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static std::string GetExt(LPCMofChar path);

#if _MSVC_LANG >= 201703L //! C++17以降の機能を使用
		// ********************************************************************************
		/// <summary>
		/// カレントディレクトリからの相対パスへ変換
		/// </summary>
		/// <param name="path">変換したい絶対パス</param>
		/// <param name="base">指定位置：ここからの相対パスになる</param>
		/// <returns>ベースからパスまでの相対パス</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static std::string ChangeRelativePath(LPCMofChar path, LPCMofChar base = GetCurrentDirectory().c_str());

		// ********************************************************************************
		/// <summary>
		/// 相対パスから絶対パスへの変換
		/// </summary>
		/// <param name="path">変換したい相対パス</param>
		/// <returns>絶対パス</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static std::string ChangeFullPath(LPCMofChar path);

		// ********************************************************************************
		/// <summary>
		/// カレントディレクトリの取得
		/// </summary>
		/// <returns>カレントディレクトリの取得</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static std::string GetCurrentDirectory(void);
#endif

		// ********************************************************************************
		/// <summary>
		/// プロジェクトディレクトリの設定
		/// </summary>
		/// <param name="path">プロジェクトディレクトリ</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static void SetProjectDirectory(LPCMofChar path);

		// ********************************************************************************
		/// <summary>
		/// プロジェクトディレクトリの取得
		/// </summary>
		/// <returns>プロジェクトディレクトリ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static std::string GetProjectDirectry(void);

		// ********************************************************************************
		/// <summary>
		/// 最初の\0以降の\0区切りの文字列配列を\0\0まで分解する。
		/// </summary>
		/// <param name="pPath">文字列配列</param>
		/// <param name="outArray">分解後の文字列を格納する配列</param>
		/// <param name="outCurrentDirPath">最初の\0までの文字列の格納場所</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static void SeparatePath(LPCMofChar pPath, std::vector<std::string>& outArray, std::string* outCurrentDirPath = nullptr);
	};
}