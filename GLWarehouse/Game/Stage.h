#pragma once

#include	"CellPosition.h"

namespace Sample {

	class Stage : public ParameterBase<Stage> {
	private:
		/** ネットワークフラグ */
		bool	network_;
		/** ステージの横サイズ */
		int		Width_;
		/** ステージの縦サイズ */
		int		Height_;
		/** ステージの情報 */
		char*	Cell_;
		/** オブジェクト配列 */
		std::vector< CellPositionPtr > Objects_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Stage()
		: ParameterBase()
		, network_(false)
		, Width_(0)
		, Height_(0) 
		, Cell_(nullptr) 
		, Objects_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		Stage(const Stage& obj)
		: ParameterBase(obj)
		, network_(obj.network_)
		, Width_(0)
		, Height_(0)
		, Cell_(nullptr) 
		, Objects_() {
		}

		/**
		 * @brief		デストラクタ
		 */
		~Stage() {
			Objects_.clear();
			SAFE_DELETE_ARRAY(Cell_);
		}

		void SetNetwork(bool flag)
		{
			network_ = flag;
		}
		void CreateNetworkStage()
		{
			//ステージの縦×横分の配列として定義
			constexpr char		Stage[10][20] = {
				{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
				{'W', ' ', ' ', 'W', ' ', ' ', 'O', ' ', '*', 'W', 'W', ' ', ' ', 'W', ' ', ' ', 'O', ' ', '*', 'W'},
				{'W', ' ', ' ', 'W', ' ', ' ', ' ', 'W', 'W', 'W', 'W', ' ', ' ', 'W', ' ', ' ', ' ', 'W', 'W', 'W'},
				{'W', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W', 'W', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W'},
				{'W', ' ', ' ', 'W', 'W', 'W', 'W', ' ', ' ', 'W', 'W', ' ', ' ', 'W', 'W', 'W', 'W', ' ', ' ', 'W'},
				{'W', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', 'W', 'W', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', 'W'},
				{'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W', ' ', 'W', 'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W', ' ', 'W'},
				{'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'W', 'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'W'},
				{'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'W', 'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'W'},
				{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
			};
			Width_ = 20;
			Height_ = 10;
			Cell_ = new char[Width_ * Height_];
			for (int y = 0; y < Height_; y++)
			{
				for (int x = 0; x < Width_; x++)
				{
					Cell_[y * Width_ + x] = Stage[y][x];

					//壁
					if (Stage[y][x] == 'W')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Wall");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//目標地点
					else if (Stage[y][x] == '*')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Target");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//地面
					else
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Ground");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);

						if (Stage[y][x] == 'O')
						{
							ActorPtr pt = SceneManager::Instance().Instaniate("Object");
							CellPositionPtr cp = pt->GetParameter<CellPosition>();
							cp->Position(x, y, 0);
							Objects_.push_back(cp);
						}
					}
				}
			}
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
			//ネットワークのみ別生成
			if (network_)
			{
				CreateNetworkStage();
				return;
			}
			//ステージの縦×横分の配列として定義
			constexpr char		Stage[10][10] = {
				{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
				{'W', 'P', ' ', 'W', ' ', ' ', 'O', ' ', '*', 'W'},
				{'W', ' ', ' ', 'W', ' ', ' ', ' ', 'W', 'W', 'W'},
				{'W', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W'},
				{'W', ' ', ' ', 'W', 'W', 'W', 'W', ' ', ' ', 'W'},
				{'W', ' ', 'S', ' ', ' ', '-', ' ', ' ', ' ', 'W'},
				{'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W', ' ', 'W'},
				{'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'W'},
				{'W', '*', 'W', ' ', ' ', ' ', '1', 'W', '1', 'W'},
				{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
			};
			Width_ = 10;
			Height_ = 10;
			Cell_ = new char[Width_ * Height_];
			for (int y = 0; y < Height_; y++)
			{
				for (int x = 0; x < Width_; x++)
				{
					Cell_[y * Width_ + x] = Stage[y][x];

					//壁
					if (Stage[y][x] == 'W')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Wall");
						pt->GetParameter<CellPosition>()->Position(x,y,0);
					}
					//目標地点
					else if (Stage[y][x] == '*')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Target");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//地面
					else
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Ground");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);

						if (Stage[y][x] == 'P')
						{
							ActorPtr pt = SceneManager::Instance().Instaniate("Player");
							pt->GetParameter<CellPosition>()->Position(x, y, 0);
						}
						else if (Stage[y][x] == 'O')
						{
							ActorPtr pt = SceneManager::Instance().Instaniate("Object");
							CellPositionPtr cp = pt->GetParameter<CellPosition>();
							cp->Position(x, y, 0);
							Objects_.push_back(cp);
						}
					}
				}
			}
		}

		/**
		 * @brief		横サイズ取得
		 * @return		横サイズ
		 */
		const int Width() const noexcept { return Width_; }
		/**
		 * @brief		縦サイズ取得
		 * @return		縦サイズ
		 */
		const int Height() const noexcept { return Height_; }

		/**
		 * @brief		指定されたマスが壁かどうかを調べる
		 * @param[in]	x		X位置
		 * @param[in]	y		Y位置
		 * @return		true	壁
		 *				false	違う
		 */
		bool IsWall(int x, int y) const noexcept {
			return Cell_[y * Width_ + x] == 'W';
		}

		/**
		 * @brief		指定されたマスにオブジェクトがあるかどうかを調べる
		 * @param[in]	x		X位置
		 * @param[in]	y		Y位置
		 * @return		true	ある
		 *				false	ない
		 */
		CellPositionPtr GetObject(int x, int y) const noexcept {
			auto it = std::find_if(Objects_.begin(), Objects_.end(),
				[&](const CellPositionPtr& obj) { return obj->Position().x == x && obj->Position().y == y; });
			if (it != Objects_.end())
			{
				return *it;
			}
			return CellPositionPtr();
		}

		/**
		 * @brief		ゴール位置に到達しているオブジェクトの数を取得する
		 * @return		ゴールしたオブジェクトの数
		 */
		int GetGoalObjectCount() const
		{
			int re = 0;
			for (auto& cp : Objects_)
			{
				if (Cell_[cp->Position().y * Width_ + cp->Position().x] == '*')
				{
					re++;
				}
			}
			return re;
		}

		/**
		 * @brief		クリア判定
		 * @return		true	クリア
		 *				false	まだ
		 */
		bool IsClear() const
		{
			return GetGoalObjectCount() >= Objects_.size();
		}

	};
	//ポインタ置き換え
	using StagePtr = std::shared_ptr<Stage>;

}