#pragma once

#include	"CellPosition.h"

namespace Sample {

	class Stage : public ParameterBase<Stage> {
	private:
		/** �l�b�g���[�N�t���O */
		bool	network_;
		/** �X�e�[�W�̉��T�C�Y */
		int		Width_;
		/** �X�e�[�W�̏c�T�C�Y */
		int		Height_;
		/** �X�e�[�W�̏�� */
		char*	Cell_;
		/** �I�u�W�F�N�g�z�� */
		std::vector< CellPositionPtr > Objects_;
	public:
		/**
		 * @brief		�R���X�g���N�^
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�f�X�g���N�^
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
			//�X�e�[�W�̏c�~�����̔z��Ƃ��Ē�`
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

					//��
					if (Stage[y][x] == 'W')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Wall");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//�ڕW�n�_
					else if (Stage[y][x] == '*')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Target");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//�n��
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
		 * @brief		�J�n
		 */
		void Start() override {
			//�l�b�g���[�N�̂ݕʐ���
			if (network_)
			{
				CreateNetworkStage();
				return;
			}
			//�X�e�[�W�̏c�~�����̔z��Ƃ��Ē�`
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

					//��
					if (Stage[y][x] == 'W')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Wall");
						pt->GetParameter<CellPosition>()->Position(x,y,0);
					}
					//�ڕW�n�_
					else if (Stage[y][x] == '*')
					{
						ActorPtr pt = SceneManager::Instance().Instaniate("Target");
						pt->GetParameter<CellPosition>()->Position(x, y, 0);
					}
					//�n��
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
		 * @brief		���T�C�Y�擾
		 * @return		���T�C�Y
		 */
		const int Width() const noexcept { return Width_; }
		/**
		 * @brief		�c�T�C�Y�擾
		 * @return		�c�T�C�Y
		 */
		const int Height() const noexcept { return Height_; }

		/**
		 * @brief		�w�肳�ꂽ�}�X���ǂ��ǂ����𒲂ׂ�
		 * @param[in]	x		X�ʒu
		 * @param[in]	y		Y�ʒu
		 * @return		true	��
		 *				false	�Ⴄ
		 */
		bool IsWall(int x, int y) const noexcept {
			return Cell_[y * Width_ + x] == 'W';
		}

		/**
		 * @brief		�w�肳�ꂽ�}�X�ɃI�u�W�F�N�g�����邩�ǂ����𒲂ׂ�
		 * @param[in]	x		X�ʒu
		 * @param[in]	y		Y�ʒu
		 * @return		true	����
		 *				false	�Ȃ�
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
		 * @brief		�S�[���ʒu�ɓ��B���Ă���I�u�W�F�N�g�̐����擾����
		 * @return		�S�[�������I�u�W�F�N�g�̐�
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
		 * @brief		�N���A����
		 * @return		true	�N���A
		 *				false	�܂�
		 */
		bool IsClear() const
		{
			return GetGoalObjectCount() >= Objects_.size();
		}

	};
	//�|�C���^�u������
	using StagePtr = std::shared_ptr<Stage>;

}