/**
 * このプログラムは画面遷移処理を作成したサンプルです。
 * このサンプルを過去２週で学んだデザインパターンの知識を使って書き換えてみましょう
 *
 * やってみてほしいこと (活用可能なデザインパターン)
 * 1.シーンの管理クラスを作成する (Singletonなど)
 * 2.シーンの生成をswitchではなくそう (Factoryメソッドなど)
 * 3.各シーンが管理クラスに依存しないようにする (Observerなど)
 */

#include	"TitleScene.h"
#include	"GameScene.h"
#include	"RankingScene.h"

//実行中のシーン
SceneNo		gNowScene = SceneNo::Title;
//変更するシーン
SceneNo		gChangeScene = SceneNo::Title;

int main()
{
	//シーンの生成
	SceneSample::ScenePointer scene = std::make_shared<SceneSample::TitleScene>();

	//シーンの初期化
	scene->Initialize(std::make_shared<SceneSample::TitleScene::InitializeParam>());

	//ゲームループ
	while (true)
	{
		//シーンの更新
		scene->Update();

		//シーンの描画
		scene->Render();

		//シーンの変更
		if (gNowScene != gChangeScene)
		{
			//前のシーンを破棄
			scene->Destroy();
			//次のシーンの番号に応じて生成
			switch (gChangeScene)
			{
			//タイトル画面への遷移
			case SceneNo::Title:
				//シーンの生成
				scene = std::make_shared<SceneSample::TitleScene>();
				//シーンの初期化
				scene->Initialize(std::make_shared<SceneSample::TitleScene::InitializeParam>());
				break;

			//ゲーム画面への遷移
			case SceneNo::Game:
				//シーンの生成
				scene = std::make_shared<SceneSample::GameScene>();
				//シーンの初期化
				scene->Initialize(std::make_shared<SceneSample::GameScene::InitializeParam>());
				break;

			//ランキング画面への遷移
			case SceneNo::Ranking:
				//シーンの生成
				scene = std::make_shared<SceneSample::RankingScene>();
				//シーンの初期化
				scene->Initialize(std::make_shared<SceneSample::RankingScene::InitializeParam>());
				break;
			}
			gNowScene = gChangeScene;
		}
	}

	//シーンの破棄
	scene->Destroy();
	return 0;
}