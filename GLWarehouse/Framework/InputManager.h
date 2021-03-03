#pragma once

#include	"Common.h"
#include	"../Library/include/GLFW/glfw3.h"

namespace Sample {

	class InputManager
	{
	private:
		struct KeyData {
			struct KeyNo {
				int				No;
				bool			bMouse;
			};
			std::vector<KeyNo>	Key;
			bool				bPrev;
			bool				bNow;

			KeyData()
			: Key()
			, bPrev(false)
			, bNow(false){
			}
		};
		using KeyMap = std::unordered_map<std::string, KeyData >;
		std::unique_ptr< KeyMap >					KeyMap_;
		double										MouseX_;
		double										MouseY_;
		GLFWwindow*									window_;

		InputManager()
		: KeyMap_() 
		, MouseX_(0)
		, MouseY_(0)
		, window_(nullptr) {
		}
		InputManager(const InputManager& obj) = delete;
		InputManager& operator=(const InputManager&) = delete;
		virtual ~InputManager() = default;
	public:

		/**
		 * @brief		キーマップの初期化
		 * @param[in]	w		入力ウインドウ
		 */
		void Initialize(GLFWwindow* w) {
			KeyMap_ = std::make_unique< KeyMap >();
			window_ = w;
		}

		/**
		 * @brief		登録キーの追加
		 * @param[in]	kn		登録キー名
		 * @param[in]	Key		登録キー
		 */
		void AddKey(const char* kn, int Key) {
			auto km = KeyMap_->find(kn);
			if (km != KeyMap_->end())
			{
				km->second.Key.push_back({ Key, false });
			}
			else
			{
				(*KeyMap_)[kn].Key.push_back({ Key, false });
			}
		}

		/**
		 * @brief		登録キーの追加
		 * @param[in]	kn		登録キー名
		 * @param[in]	Key		登録キー
		 */
		void AddMouseKey(const char* kn, int Key) {
			auto km = KeyMap_->find(kn);
			if (km != KeyMap_->end())
			{
				km->second.Key.push_back({ Key, true });
			}
			else
			{
				(*KeyMap_)[kn].Key.push_back({ Key, true });
			}
		}

		/**
		 * @brief		フレーム時間の更新
		 *				Framework::Runにて呼び出し
		 */
		void Update() {
			for (auto k = KeyMap_->begin(); k != KeyMap_->end(); ++k)
			{
				k->second.bPrev = k->second.bNow;
				k->second.bNow = 0;
				for (auto key : k->second.Key)
				{
					if (key.bMouse)
					{
						k->second.bNow |= (glfwGetMouseButton(window_, key.No) == GLFW_PRESS);
					}
					else
					{
						k->second.bNow |= (glfwGetKey(window_, key.No) == GLFW_PRESS);
					}
				}
			}
			//マウス位置の取得
			glfwGetCursorPos(window_, &MouseX_, &MouseY_);
		}

		/**
		 * @brief		キー状態の取得
		 * @param[in]	keycode	キーコード
		 */
		int GetKeyState(const int keycode) {
			return glfwGetKey(window_, keycode);
		}

		/**
		 * @brief		指定名称の登録キーがこのフレームに押されたかどうか
		 * @param[in]	kn		登録キー名
		 * @return		true	このフレームで押された
		 *				false	このフレームでは押されていない
		 */
		bool IsPush(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return kd.bNow && !kd.bPrev;
		}

		/**
		 * @brief		指定名称の登録キーがこのフレームに離されたかどうか
		 * @param[in]	kn		登録キー名
		 * @return		true	このフレームで離された
		 *				false	このフレームでは離されていない
		 */
		bool IsPull(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return !kd.bNow && kd.bPrev;
		}

		/**
		 * @brief		指定名称の登録キーが押されているかどうか
		 * @param[in]	kn		登録キー名
		 * @return		true	このフレームで押されている
		 *				false	このフレームで押されていない
		 */
		bool IsPress(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return kd.bNow;
		}

		/**
		 * @brief		マウス横位置
		 * @return		マウス横位置
		 */
		constexpr double GetMouseX() const {
			return MouseX_;
		}

		/**
		 * @brief		マウス縦位置
		 * @return		マウス縦位置
		 */
		constexpr double GetMouseY() const {
			return MouseY_;
		}

		/**
		 * @brief		解放処理
		 */
		void Release() {
			KeyMap_.reset();
			window_ = nullptr;
		}

		static InputManager& Instance() {
			static InputManager t;
			return t;
		}
	};

}