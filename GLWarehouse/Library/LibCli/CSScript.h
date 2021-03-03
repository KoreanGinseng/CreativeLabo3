#pragma once

#include	"../../Framework/SharedInterface.h"

namespace Sample {

/**
 * @brief		指定されたディレクトリに存在するcsファイルをコンパイルする
 * @param[in]	directory		スクリプト保存ディレクトリ
 * @param[in]	bWatch			ファイル更新監視フラグ
 *								trueの場合、更新があった場合にReloadを実行する
 */
void ScriptCompile(const std::string& directory, bool bwatch);


class ICSScriptImpl {
public:
	/**
	 * @brief		CSスクリプトのアセンブリを取得
	 * @param[in]	fname		取得するアセンブリ名
	 */
	virtual void Load(const std::string& fname) = 0;

	/**
	* @brief		共有データの設定
	*/
	virtual void SetShared(ISharedActor* pShared) = 0;

	/**
	 * @brief		スクリプトの実行
	 */
	virtual void Start() = 0;
	/**
	 * @brief		スクリプトの実行
	 */
	virtual void Update() = 0;

	/**
	 * @brief		変更フラグの設定
	 */
	virtual void SetChange(bool change) = 0;
};
using CSScriptImplPtr = std::shared_ptr < ICSScriptImpl >;

class CSScript {
private:
	/** 内部生成データ */
	CSScriptImplPtr		pImpl;
	/** 共有データ */
	ISharedActor*		pShared;
public:
	/**
	 * @brief		コンストラクタ
	 */
	CSScript(ISharedActor* ps);
	/**
	 * @brief		デストラクタ
	 */
	~CSScript();
	/**
	 * @brief		CSスクリプトのアセンブリを取得
	 * @param[in]	classname		取得するアセンブリ名
	 */
	void Load(const std::string& classname);
	/**
	 * @brief		スクリプトの実行
	 */
	void Start();
	/**
	 * @brief		スクリプトの実行
	 */
	void Update();
};

}