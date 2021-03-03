#pragma once

#include	"AudioManager.h"
#include	"../Vector3.h"
#include	"Sound.h"

namespace Sample {

	class Speaker {
	protected:
		/** �X�s�[�J�[�p�����[�^�[ */
		template < typename T >
		struct SpeakerParameter {
			/** �L���t���O */
			bool		Enable;
			/** �ݒ�l */
			T			Value;

			/** �p�����[�^�[�ݒ� */
			SpeakerParameter(const T& val) 
			: Enable(false)
			, Value(val) {
			}
			SpeakerParameter(const SpeakerParameter& obj)
			: Enable(obj.Enable)
			, Value(obj.Value) {
			}
		};
		/** �X�s�[�J�[�̈ʒu */
		SpeakerParameter<Vector3F>			Position_;
		/** �X�s�[�J�[�̑��x */
		SpeakerParameter<Vector3F>			Velocity_;
		/** �X�s�[�J�[�̑O���� */
		SpeakerParameter<Vector3F>			Direction_;
		/** �Q�C�� */
		SpeakerParameter<ALfloat>			Gain_;
		/** �s�b�` */
		SpeakerParameter<ALfloat>			Pitch_;
		/** �Q�Ƌ��� */
		SpeakerParameter<ALfloat>			ReferenceDistance_;
		/** �ő勗�� */
		SpeakerParameter<ALfloat>			MaxDistance_;
		/** ���[���I�t */
		SpeakerParameter<ALfloat>			Rolloff_;
		/** �R�[���̓��p */
		SpeakerParameter<ALfloat>			ConeInnerAngle_;
		/** �R�[���̊O�p */
		SpeakerParameter<ALfloat>			ConeOuterAngle_;
		/** �R�[���̊O���ł̃Q�C�� */
		SpeakerParameter<ALfloat>			ConeOuterGain_;

		/** ���t�@�C�� */
		SoundArray							Sounds_;

		/**
		 * @brief		�\�[�X�̍X�V�������Ȃ�
		 */
		virtual void SetState(ALuint source);
	public:
		/**
		 * @brief		�R���X�g���N�^
		 * @param[in]	nm	���O
		 */
		Speaker();
		/**
		 * @brief		�R�s�[�R���X�g���N�^
		 * @param[in]	obj	�R�s�[����I�u�W�F�N�g
		 */
		Speaker(const Speaker& obj);

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Speaker();

		/**
		 * @brief		�X�s�[�J�[�̍X�V�������Ȃ�
		 */
		virtual void Update();

		/**
		 * @brief		���̍Đ��������Ȃ�
		 */
		void Play(SoundPtr pt);

		/**
		 * @brief		Position�ւ�Accessor
		 * @param[in]	p	�ʒu
		 */
		void Position(const Vector3F& p) { Position_.Enable = true; Position_.Value = p; }
		/**
		 * @brief		Position�ւ�Accessor
		 * @return		�ʒu
		 */
		const Vector3F& Position() const noexcept { return Position_.Value; }

		/**
		 * @brief		Velocity�ւ�Accessor
		 * @param[in]	p	���x
		 */
		void Velocity(const Vector3F& p) { Velocity_.Enable = true; Velocity_.Value = p; }
		/**
		 * @brief		Velocity�ւ�Accessor
		 * @return		���x
		 */
		const Vector3F& Velocity() const noexcept { return Velocity_.Value; }

		/**
		 * @brief		Direction�ւ�Accessor
		 * @param[in]	p	����
		 */
		void Direction(const Vector3F& p) { Direction_.Enable = true; Direction_.Value = p; }
		/**
		 * @brief		Direction�ւ�Accessor
		 * @return		����
		 */
		const Vector3F& Direction() const noexcept { return Direction_.Value; }

		/**
		 * @brief		Gain�ւ�Accessor
		 * @param[in]	p	�Q�C��
		 */
		void Gain(const ALfloat& p) { Gain_.Enable = true; Gain_.Value = p; }
		/**
		 * @brief		Gain�ւ�Accessor
		 * @return		�Q�C��
		 */
		const ALfloat Gain() const noexcept { return Gain_.Value; }

		/**
		 * @brief		Pitch�ւ�Accessor
		 * @param[in]	p	�s�b�`
		 */
		void Pitch(const ALfloat& p) { Pitch_.Enable = true; Pitch_.Value = p; }
		/**
		 * @brief		Pitch�ւ�Accessor
		 * @return		�s�b�`
		 */
		const ALfloat Pitch() const noexcept { return Pitch_.Value; }

		/**
		 * @brief		ReferenceDistance�ւ�Accessor
		 * @param[in]	p	�Q�Ƌ���
		 */
		void ReferenceDistance(const ALfloat& p) { ReferenceDistance_.Enable = true; ReferenceDistance_.Value = p; }
		/**
		 * @brief		ReferenceDistance�ւ�Accessor
		 * @return		�Q�Ƌ���
		 */
		const ALfloat ReferenceDistance() const noexcept { return ReferenceDistance_.Value; }

		/**
		 * @brief		MaxDistance�ւ�Accessor
		 * @param[in]	p	�ő勗��
		 */
		void MaxDistance(const ALfloat& p) { MaxDistance_.Enable = true; MaxDistance_.Value = p; }
		/**
		 * @brief		MaxDistance�ւ�Accessor
		 * @return		�ő勗��
		 */
		const ALfloat MaxDistance() const noexcept { return MaxDistance_.Value; }

		/**
		 * @brief		Rolloff�ւ�Accessor
		 * @param[in]	p	���[���I�t
		 */
		void Rolloff(const ALfloat& p) { Rolloff_.Enable = true; Rolloff_.Value = p; }
		/**
		 * @brief		Rolloff�ւ�Accessor
		 * @return		���[���I�t
		 */
		const ALfloat Rolloff() const noexcept { return Rolloff_.Value; }

		/**
		 * @brief		ConeInnerAngle�ւ�Accessor
		 * @param[in]	p	�R�[���̓��p
		 */
		void ConeInnerAngle(const ALfloat& p) { ConeInnerAngle_.Enable = true; ConeInnerAngle_.Value = p; }
		/**
		 * @brief		ConeInnerAngle�ւ�Accessor
		 * @return		�R�[���̓��p
		 */
		const ALfloat ConeInnerAngle() const noexcept { return ConeInnerAngle_.Value; }

		/**
		 * @brief		ConeOuterAngle�ւ�Accessor
		 * @param[in]	p	�R�[���̊O�p
		 */
		void ConeOuterAngle(const ALfloat& p) { ConeOuterAngle_.Enable = true; ConeOuterAngle_.Value = p; }
		/**
		 * @brief		ConeOuterAngle�ւ�Accessor
		 * @return		�R�[���̊O�p
		 */
		const ALfloat ConeOuterAngle() const noexcept { return ConeOuterAngle_.Value; }

		/**
		 * @brief		ConeOuterGain�ւ�Accessor
		 * @param[in]	p	�R�[���̊O���ł̃Q�C��
		 */
		void ConeOuterGain(const ALfloat& p) { ConeOuterGain_.Enable = true; ConeOuterGain_.Value = p; }
		/**
		 * @brief		ConeOuterGain�ւ�Accessor
		 * @return		�R�[���̊O���ł̃Q�C��
		 */
		const ALfloat ConeOuterGain() const noexcept { return ConeOuterGain_.Value; }
	};
}