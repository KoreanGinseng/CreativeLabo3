#pragma once

#include	"AudioManager.h"
#include	"../Vector3.h"
#include	"Sound.h"

namespace Sample {

	class Speaker {
	protected:
		/** スピーカーパラメーター */
		template < typename T >
		struct SpeakerParameter {
			/** 有効フラグ */
			bool		Enable;
			/** 設定値 */
			T			Value;

			/** パラメーター設定 */
			SpeakerParameter(const T& val) 
			: Enable(false)
			, Value(val) {
			}
			SpeakerParameter(const SpeakerParameter& obj)
			: Enable(obj.Enable)
			, Value(obj.Value) {
			}
		};
		/** スピーカーの位置 */
		SpeakerParameter<Vector3F>			Position_;
		/** スピーカーの速度 */
		SpeakerParameter<Vector3F>			Velocity_;
		/** スピーカーの前向き */
		SpeakerParameter<Vector3F>			Direction_;
		/** ゲイン */
		SpeakerParameter<ALfloat>			Gain_;
		/** ピッチ */
		SpeakerParameter<ALfloat>			Pitch_;
		/** 参照距離 */
		SpeakerParameter<ALfloat>			ReferenceDistance_;
		/** 最大距離 */
		SpeakerParameter<ALfloat>			MaxDistance_;
		/** ロールオフ */
		SpeakerParameter<ALfloat>			Rolloff_;
		/** コーンの内角 */
		SpeakerParameter<ALfloat>			ConeInnerAngle_;
		/** コーンの外角 */
		SpeakerParameter<ALfloat>			ConeOuterAngle_;
		/** コーンの外側でのゲイン */
		SpeakerParameter<ALfloat>			ConeOuterGain_;

		/** 音ファイル */
		SoundArray							Sounds_;

		/**
		 * @brief		ソースの更新をおこなう
		 */
		virtual void SetState(ALuint source);
	public:
		/**
		 * @brief		コンストラクタ
		 * @param[in]	nm	名前
		 */
		Speaker();
		/**
		 * @brief		コピーコンストラクタ
		 * @param[in]	obj	コピーするオブジェクト
		 */
		Speaker(const Speaker& obj);

		/**
		 * @brief		デストラクタ
		 */
		virtual ~Speaker();

		/**
		 * @brief		スピーカーの更新をおこなう
		 */
		virtual void Update();

		/**
		 * @brief		音の再生をおこなう
		 */
		void Play(SoundPtr pt);

		/**
		 * @brief		PositionへのAccessor
		 * @param[in]	p	位置
		 */
		void Position(const Vector3F& p) { Position_.Enable = true; Position_.Value = p; }
		/**
		 * @brief		PositionへのAccessor
		 * @return		位置
		 */
		const Vector3F& Position() const noexcept { return Position_.Value; }

		/**
		 * @brief		VelocityへのAccessor
		 * @param[in]	p	速度
		 */
		void Velocity(const Vector3F& p) { Velocity_.Enable = true; Velocity_.Value = p; }
		/**
		 * @brief		VelocityへのAccessor
		 * @return		速度
		 */
		const Vector3F& Velocity() const noexcept { return Velocity_.Value; }

		/**
		 * @brief		DirectionへのAccessor
		 * @param[in]	p	向き
		 */
		void Direction(const Vector3F& p) { Direction_.Enable = true; Direction_.Value = p; }
		/**
		 * @brief		DirectionへのAccessor
		 * @return		向き
		 */
		const Vector3F& Direction() const noexcept { return Direction_.Value; }

		/**
		 * @brief		GainへのAccessor
		 * @param[in]	p	ゲイン
		 */
		void Gain(const ALfloat& p) { Gain_.Enable = true; Gain_.Value = p; }
		/**
		 * @brief		GainへのAccessor
		 * @return		ゲイン
		 */
		const ALfloat Gain() const noexcept { return Gain_.Value; }

		/**
		 * @brief		PitchへのAccessor
		 * @param[in]	p	ピッチ
		 */
		void Pitch(const ALfloat& p) { Pitch_.Enable = true; Pitch_.Value = p; }
		/**
		 * @brief		PitchへのAccessor
		 * @return		ピッチ
		 */
		const ALfloat Pitch() const noexcept { return Pitch_.Value; }

		/**
		 * @brief		ReferenceDistanceへのAccessor
		 * @param[in]	p	参照距離
		 */
		void ReferenceDistance(const ALfloat& p) { ReferenceDistance_.Enable = true; ReferenceDistance_.Value = p; }
		/**
		 * @brief		ReferenceDistanceへのAccessor
		 * @return		参照距離
		 */
		const ALfloat ReferenceDistance() const noexcept { return ReferenceDistance_.Value; }

		/**
		 * @brief		MaxDistanceへのAccessor
		 * @param[in]	p	最大距離
		 */
		void MaxDistance(const ALfloat& p) { MaxDistance_.Enable = true; MaxDistance_.Value = p; }
		/**
		 * @brief		MaxDistanceへのAccessor
		 * @return		最大距離
		 */
		const ALfloat MaxDistance() const noexcept { return MaxDistance_.Value; }

		/**
		 * @brief		RolloffへのAccessor
		 * @param[in]	p	ロールオフ
		 */
		void Rolloff(const ALfloat& p) { Rolloff_.Enable = true; Rolloff_.Value = p; }
		/**
		 * @brief		RolloffへのAccessor
		 * @return		ロールオフ
		 */
		const ALfloat Rolloff() const noexcept { return Rolloff_.Value; }

		/**
		 * @brief		ConeInnerAngleへのAccessor
		 * @param[in]	p	コーンの内角
		 */
		void ConeInnerAngle(const ALfloat& p) { ConeInnerAngle_.Enable = true; ConeInnerAngle_.Value = p; }
		/**
		 * @brief		ConeInnerAngleへのAccessor
		 * @return		コーンの内角
		 */
		const ALfloat ConeInnerAngle() const noexcept { return ConeInnerAngle_.Value; }

		/**
		 * @brief		ConeOuterAngleへのAccessor
		 * @param[in]	p	コーンの外角
		 */
		void ConeOuterAngle(const ALfloat& p) { ConeOuterAngle_.Enable = true; ConeOuterAngle_.Value = p; }
		/**
		 * @brief		ConeOuterAngleへのAccessor
		 * @return		コーンの外角
		 */
		const ALfloat ConeOuterAngle() const noexcept { return ConeOuterAngle_.Value; }

		/**
		 * @brief		ConeOuterGainへのAccessor
		 * @param[in]	p	コーンの外側でのゲイン
		 */
		void ConeOuterGain(const ALfloat& p) { ConeOuterGain_.Enable = true; ConeOuterGain_.Value = p; }
		/**
		 * @brief		ConeOuterGainへのAccessor
		 * @return		コーンの外側でのゲイン
		 */
		const ALfloat ConeOuterGain() const noexcept { return ConeOuterGain_.Value; }
	};
}