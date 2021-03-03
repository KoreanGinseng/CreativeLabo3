#pragma once

#include	"../../Framework/SharedInterface.h"

namespace Sample {

/**
 * @brief		�w�肳�ꂽ�f�B���N�g���ɑ��݂���cs�t�@�C�����R���p�C������
 * @param[in]	directory		�X�N���v�g�ۑ��f�B���N�g��
 * @param[in]	bWatch			�t�@�C���X�V�Ď��t���O
 *								true�̏ꍇ�A�X�V���������ꍇ��Reload�����s����
 */
void ScriptCompile(const std::string& directory, bool bwatch);


class ICSScriptImpl {
public:
	/**
	 * @brief		CS�X�N���v�g�̃A�Z���u�����擾
	 * @param[in]	fname		�擾����A�Z���u����
	 */
	virtual void Load(const std::string& fname) = 0;

	/**
	* @brief		���L�f�[�^�̐ݒ�
	*/
	virtual void SetShared(ISharedActor* pShared) = 0;

	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	virtual void Start() = 0;
	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	virtual void Update() = 0;

	/**
	 * @brief		�ύX�t���O�̐ݒ�
	 */
	virtual void SetChange(bool change) = 0;
};
using CSScriptImplPtr = std::shared_ptr < ICSScriptImpl >;

class CSScript {
private:
	/** ���������f�[�^ */
	CSScriptImplPtr		pImpl;
	/** ���L�f�[�^ */
	ISharedActor*		pShared;
public:
	/**
	 * @brief		�R���X�g���N�^
	 */
	CSScript(ISharedActor* ps);
	/**
	 * @brief		�f�X�g���N�^
	 */
	~CSScript();
	/**
	 * @brief		CS�X�N���v�g�̃A�Z���u�����擾
	 * @param[in]	classname		�擾����A�Z���u����
	 */
	void Load(const std::string& classname);
	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	void Start();
	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	void Update();
};

}