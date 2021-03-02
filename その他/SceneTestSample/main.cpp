/**
 * 作成例
 * ※あくまで１例で紹介したデザインパターンの考えを全て使うようにしたため実践では不要な部分もある
 *   実践では必要なパターンだけ選択して活用していこう
 * 
 * ※Flyweightはリソースがないため利用無し、
 *   シーンごとの画像素材、音素材などをFlyweightで管理する
 *
 * 1.シーンの管理クラスを作成する (Singletonなど)
 *   ⇒SceneManagerをSingletonで作成
 *     SceneManagerがmain.cppに書いていた遷移などの処理を実行する
 *
 * 2.シーンの生成をswitchではなくそう (Factoryメソッドなど)
 *   ⇒SceneManagerにシーンを登録、登録された生成クラスからシーンを生成
 *     SceneManager内からシーン番号での遷移を消去、シーン番号の列挙からシーン名での遷移に変更
 *     シーンの変更が名前になったことにより、シーンの追加で列挙の変更が不要に、
 *     また登録されたシーンへ遷移するようにすることでSceneManagerが全てのシーンを知る必要はなくなった
 *  
 * 3.各シーンが管理クラスに依存しないようにする (Observerなど)
 *   ⇒SceneをSubject、SceneManagerをObserverにすることでシーンから通知を送信することで遷移1
 *	   またSceneがSubjectになったことにより、画面遷移を受け取りたいオブジェクトはObserverにすれば遷移時に通知を受け取れる
 */

#include	"SceneManager.h"
#include	"TitleScene.h"
#include	"GameScene.h"
#include	"RankingScene.h"

int main()
{
	//シーンの登録
	SceneManagerInstance
		.Register<SceneSample::TitleScene>("Title")
		.Register<SceneSample::GameScene>("Game")
		.Register<SceneSample::RankingScene>("Ranking");

	//シーン初期化
	SceneManagerInstance.ChangeScene("Title");

	//ゲームループ
	while (true)
	{
		//シーンの更新
		SceneManagerInstance.Update();

		//シーンの描画
		SceneManagerInstance.Render();
	}

	//シーンの破棄
	SceneSample::SceneManager::Release();
	return 0;
}