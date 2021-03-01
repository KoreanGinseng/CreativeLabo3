#pragma once
#include "Mof.h"
#include <unordered_map>
#include "StopWatch.h"

// 参考 SceneManager.hpp in OpenSiv3D ver4.3.0

namespace sip
{
	// プロトタイプ宣言
	template <class State, class Data> class CSceneManager;

	struct EmptyData {};

	// ********************************************************************************
	/// <summary>
	/// シーン・インターフェース
	/// </summary>
	// ********************************************************************************
	template<class State, class Data>
	class IScene
	{
	public:

		// ********************************************************************************
		/// <summary>
		/// 初期化データ
		/// </summary>
		// ********************************************************************************
		struct InitData
		{
			State                       state;    //! ステート

			std::shared_ptr<Data>       pData;    //! 共通データポインタ

			CSceneManager<State, Data>* pManager; //! シーンマネージャポインタ

			// ********************************************************************************
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <created>いのうえ,2020/11/11</created>
			/// <changed>いのうえ,2020/11/11</changed>
			// ********************************************************************************
			InitData(void) = default;

			// ********************************************************************************
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="s">ステート</param>
			/// <param name="d">共通データのポインタ</param>
			/// <param name="m">マネージャのポインタ</param>
			/// <created>いのうえ,2020/11/11</created>
			/// <changed>いのうえ,2020/11/11</changed>
			// ********************************************************************************
			InitData(const State& s, std::shared_ptr<Data>& d, CSceneManager<State, Data>* m) :
				state(s),
				pData(d),
				pManager(m) {}
		};

	private:

		State                           m_State;     //! ステート

		std::shared_ptr<Data>           m_pData;     //! 共通データポインタ

		CSceneManager<State, Data>*     m_pManager;  //! マネージャポインタ

	public:

		// ********************************************************************************
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="init">初期化データ</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		explicit IScene(const InitData& init)
			: m_State(init.state)
			, m_pData(init.pData)
			, m_pManager(init.pManager)
		{
		}

		// ********************************************************************************
		/// <summary>
		/// デストラクタ
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual ~IScene(void) = default;

		// ********************************************************************************
		/// <summary>
		/// フェードイン時の更新
		/// </summary>
		/// <param name="float">フェードイン時間</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void UpdateFadeIn(float) {}

		// ********************************************************************************
		/// <summary>
		/// 通常時の更新
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void Update(void) {}

		// ********************************************************************************
		/// <summary>
		/// フェードアウト時の更新
		/// </summary>
		/// <param name="float">フェードアウト時間</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void UpdateFadeOut(float) {}

		// ********************************************************************************
		/// <summary>
		/// 通常時の描画
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void Render(void) {}

		// ********************************************************************************
		/// <summary>
		/// フェードイン時の描画
		/// </summary>
		/// <param name="t">フェードイン時間</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void RenderFadeIn(float t) 
		{
			Render();

			MofU32 color     = m_pManager->GetFadeColor();

			MofU8  alpha     = (1.0f - t) * 255;

			MofU32 fadeColor = MOF_ARGB(alpha, MOF_GetRed(color), MOF_GetGreen(color), MOF_GetBlue(color));

			CGraphicsUtilities::RenderFillRect(
				0, 0,
				g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(),
				fadeColor
			);
		}

		// ********************************************************************************
		/// <summary>
		/// フェードアウト時の描画
		/// </summary>
		/// <param name="t">フェードアウト時間</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		virtual void RenderFadeOut(float t)
		{
			Render();

			MofU32 color     = m_pManager->GetFadeColor();

			MofU8  alpha     = t * 255;

			MofU32 fadeColor = MOF_ARGB(alpha, MOF_GetRed(color), MOF_GetGreen(color), MOF_GetBlue(color));

			CGraphicsUtilities::RenderFillRect(
				0, 0,
				g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(),
				fadeColor
			);
		}

	protected:

		// ********************************************************************************
		/// <summary>
		/// ステートの取得
		/// </summary>
		/// <returns>ステート</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		const State& GetState(void) const
		{
			return m_State;
		}

		// ********************************************************************************
		/// <summary>
		/// 共有データへの参照を取得
		/// </summary>
		/// <returns>共有データ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		Data& GetData(void) const
		{
			return *m_pData;
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの変更を通知する。
		/// </summary>
		/// <param name="state">次のシーンのキー</param>
		/// <param name="transitionTime">フェードイン・アウトの時間(単位:秒)</param>
		/// <returns>シーンの変更が可能でフェードイン・アウトが開始される場合 true, それ以外 false</returns>
		/// <created>いのうえ,2020/05/22</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool ChangeScene(const State& state, const float& transitionTime = 1.0f)
		{
			return m_pManager->ChangeScene(state, transitionTime);
		}

		// ********************************************************************************
		/// <summary>
		/// エラーの発生を通知する。
		/// この関数を呼ぶと、以降の CSceneManager::Update() / CSceneManager::Render() が false を返す。
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		void NotifyError(void)
		{
			return m_pManager->NotifyError();
		}
	};

	// ********************************************************************************
	/// <summary>
	/// シーン管理
	/// State : シーンを区別するキーの型
	/// Data  : シーン間で共有するデータの型
	/// </summary>
	// ********************************************************************************
	template<class State, class Data = EmptyData>
	class CSceneManager
	{
	private:

		using Scene_t = std::shared_ptr<IScene<State, Data>>;

		using FactoryFunction_t = std::function<Scene_t()>;

		std::unordered_map<State, FactoryFunction_t> m_Factories;                   //! 各ステート毎のファクトリ

		std::shared_ptr<Data>                        m_pData;                       //! 共通データポインタ

		Scene_t                                      m_pCurrent;                    //! 現在のシーン

		Scene_t                                      m_pNext;                       //! 次のシーン

		State                                        m_CurrentState;                //! 現在のステート

		State                                        m_NextState;                   //! 次のステート
		
		std::shared_ptr<State>                       m_pFirst;                      //! 最初のステート

		// ********************************************************************************
		/// <summary>
		/// シーンの遷移列挙
		/// </summary>
		// ********************************************************************************
		enum class TransitionState
		{
			None,      //! なし

			FadeIn,    //! フェードイン

			Active,    //! アクティブ

			FadeOut,   //! フェードアウト

		} m_TransitionState = TransitionState::None;                                //! シーンの遷移状態

		CStopWatch                                   m_StopWatch;                   //! ストップウォッチ

		float                                        m_TransitionTime = 1.0f;       //! 遷移時間(秒)

		MofU32                                       m_FadeColor = MOF_COLOR_BLACK; //! フェード色

		bool                                         m_bError = false;              //! エラーフラグ

		// ********************************************************************************
		/// <summary>
		/// シーン単体更新
		/// </summary>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool UpdateSingle(void)
		{
			m_StopWatch.Update();

			float elapsed = m_StopWatch.GetTime();

			if (m_TransitionState == TransitionState::FadeOut && elapsed >= m_TransitionTime)
			{
				m_pCurrent = nullptr;

				m_pCurrent = m_Factories[m_NextState]();

				if (IsError())
				{
					return false;
				}

				m_CurrentState = m_NextState;

				m_TransitionState = TransitionState::FadeIn;

				m_StopWatch.Reset();

				elapsed = 0.0f;
			}

			if (m_TransitionState == TransitionState::FadeIn && elapsed >= m_TransitionTime)
			{
				m_StopWatch.Reset();

				m_TransitionState = TransitionState::Active;
			}

			switch (m_TransitionState)
			{
			case TransitionState::FadeIn:
				assert(m_TransitionTime);
				m_pCurrent->UpdateFadeIn(elapsed / m_TransitionTime);
				return !IsError();
			case TransitionState::Active:
				m_pCurrent->Update();
				return !IsError();
			case TransitionState::FadeOut:
				assert(m_TransitionTime);
				m_pCurrent->UpdateFadeOut(elapsed / m_TransitionTime);
				return !IsError();
			default:
				return false;
			}
		}

		// ********************************************************************************
		/// <summary>
		/// エラーの取得
		/// </summary>
		/// <returns>エラーフラグ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool IsError(void) const noexcept
		{
			return m_bError;
		}

	public:

		// シーンのインターフェース
		using CScene = IScene<State, Data>;

		// ********************************************************************************
		/// <summary>
		/// シーン管理の初期化をする。
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		CSceneManager(void) :
			m_pData(std::make_shared<Data>()) {}

		// ********************************************************************************
		/// <summary>
		/// シーン管理の初期化をする。
		/// </summary>
		/// <param name="data">初期化データ</param>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		CSceneManager(const std::shared_ptr<Data>& data) :
			m_pData(data) {}

		// ********************************************************************************
		/// <summary>
		/// シーンを追加
		/// </summary>
		/// <param name="state">追加するシーンのステート名</param>
		/// <returns>マネージャ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		template<class CScene>
		CSceneManager& Add(const State& state)
		{
			typename CScene::InitData initData(state, m_pData, this);

			FactoryFunction_t factory = [=] {
				return std::make_shared<CScene>(initData);
			};

			auto it = m_Factories.find(state);

			if (it != m_Factories.end())
			{
				m_Factories[state] = factory;
			}
			else
			{
				m_Factories.emplace(state, factory);

				if (!m_pFirst)
				{
					m_pFirst = std::make_shared<State>();
					*(m_pFirst.get()) = state;
				}
			}

			return *this;
		}

		// ********************************************************************************
		/// <summary>
		/// 最初のシーンを初期化
		/// </summary>
		/// <param name="state">初期化データ</param>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool Initialize(const State& state)
		{
			if (m_pCurrent)
			{
				return false;
			}

			auto it = m_Factories.find(state);

			if (it == m_Factories.end())
			{
				return false;
			}

			m_CurrentState = state;

			m_pCurrent = it->second();

			if (IsError())
			{
				return false;
			}

			m_TransitionState = TransitionState::FadeIn;

			m_StopWatch.Start();

			return true;
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの更新
		/// </summary>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool UpdateScene(void)
		{
			if (IsError())
			{
				return false;
			}

			if (!m_pCurrent)
			{
				if (!m_pFirst)
				{
					return true;
				}
				else if (!Initialize(*m_pFirst.get()))
				{
					return false;
				}
			}

			return UpdateSingle();
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの描画
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		void RenderScene(void)
		{
			if (!m_pCurrent)
			{
				return;
			}

			if (m_TransitionState == TransitionState::Active || !m_TransitionTime)
			{
				m_pCurrent->Render();
			}

			const float elapsed = m_StopWatch.GetTime();

			if (m_TransitionState == TransitionState::FadeIn)
			{
				m_pCurrent->RenderFadeIn(elapsed / m_TransitionTime);
			}
			else if (m_TransitionState == TransitionState::FadeOut)
			{
				m_pCurrent->RenderFadeOut(elapsed / m_TransitionTime);
			}
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの更新
		/// </summary>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool Update(void)
		{
			if (!UpdateScene())
			{
				return false;
			}

			return true;
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの描画
		/// </summary>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool Render(void)
		{
			if (IsError())
			{
				return false;
			}

			RenderScene();

			return true;
		}

		// ********************************************************************************
		/// <summary>
		/// 共有データの取得
		/// </summary>
		/// <returns>共有データ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		std::shared_ptr<Data> GetData(void)
		{
			return m_pData;
		}

		// ********************************************************************************
		/// <summary>
		/// 共有データの取得
		/// </summary>
		/// <returns>共有データ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		const std::shared_ptr<const Data> GetData(void) const
		{
			return m_Data;
		}

		// ********************************************************************************
		/// <summary>
		/// シーンの変更
		/// </summary>
		/// <param name="state">変更先のステート</param>
		/// <param name="transitionTime">遷移時間</param>
		/// <returns>true : 成功, false : 失敗</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		bool ChangeScene(const State& state, float transitionTime)
		{
			if (m_Factories.find(state) == m_Factories.end())
			{
				return false;
			}

			m_NextState = state;

			m_TransitionTime = transitionTime * 0.5f;

			m_TransitionState = TransitionState::FadeOut;

			m_StopWatch.Start();

			return true;
		}

		// ********************************************************************************
		/// <summary>
		/// フェードイン・アウトのデフォルトの色を設定する。
		/// </summary>
		/// <param name="color">設定する色</param>
		/// <returns>マネージャ</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		CSceneManager& SetFadeColor(const MofU32& color)
		{
			m_FadeColor = color;
			return *this;
		}

		// ********************************************************************************
		/// <summary>
		/// フェードイン・アウトのデフォルト色を取得する。
		/// </summary>
		/// <returns>フェード色</returns>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		const MofU32& GetFadeColor(void) const
		{
			return m_FadeColor;
		}

		// ********************************************************************************
		/// <summary>
		/// エラーの発生を通知する。
		/// </summary>
		/// <created>いのうえ,2020/11/11</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		void NotifyError(void)
		{
			m_bError = true;
		}
	};
}

