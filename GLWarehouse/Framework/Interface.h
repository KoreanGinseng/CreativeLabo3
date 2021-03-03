#pragma once

#include	"Common.h"
#include	"Vector3.h"
#include	"SharedInterface.h"

namespace Sample {

//�A�N�^�[�O����`
class Actor;
//�|�C���^�u������
using ActorPtr = std::shared_ptr<Actor>;
//���I�z��u������
using ActorList = std::list<ActorPtr>;
using ActorListPtr = std::shared_ptr<ActorList>;

//--------------------------------------------------------------
#pragma region ����p�C���^�[�t�F�C�X
//--------------------------------------------------------------
class __declspec(novtable) IInterface {
public:
	virtual ~IInterface() = default;
};
//�|�C���^�u������
using InterfacePtr = std::shared_ptr<IInterface>;
//���I�z��u������
using InterfaceArray = std::vector<InterfacePtr>;

class __declspec(novtable) IComponent : public IInterface {
public:
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void SetActor(Actor* ac) = 0;
	virtual Actor* GetActor(void) = 0;

};
//�|�C���^�u������
using ComponentPtr = std::shared_ptr<IComponent>;
//���I�z��u������
using ComponentArray = std::vector<ComponentPtr>;

class __declspec(novtable) IParameter : public IComponent {
public:
	virtual std::shared_ptr<IParameter> Clone() = 0;
};
//�|�C���^�u������
using ParameterPtr = std::shared_ptr<IParameter>;
//���I�z��u������
using ParameterArray = std::vector<ParameterPtr>;

class __declspec(novtable) IAction : public IComponent {
public:
	virtual void Update() = 0;
	virtual std::shared_ptr<IAction> Clone() = 0;
	virtual void Enabled(const bool p) noexcept = 0;
	virtual const bool Enabled(void) const noexcept = 0;
};
//�|�C���^�u������
using ActionPtr = std::shared_ptr<IAction>;
//���I�z��u������
using ActionArray = std::vector<ActionPtr>;

class __declspec(novtable) IRender : public IComponent {
public:
	virtual void Render() = 0;
	virtual std::shared_ptr<IRender> Clone() = 0;
	virtual void Enabled(const bool p) noexcept = 0;
	virtual const bool Enabled(void) const noexcept = 0;
	virtual void Priority(const int p) noexcept = 0;
	virtual const int Priority(void) const noexcept = 0;
};
//�|�C���^�u������
using RenderPtr = std::shared_ptr<IRender>;
//���I�z��u������
using RenderArray = std::vector<RenderPtr>;
#pragma endregion
//--------------------------------------------------------------

//--------------------------------------------------------------
#pragma region ���A�N�^�[�֘A
//--------------------------------------------------------------
class __declspec(novtable) IActor : public ISharedActor {
public:
	/**
	 * @brief		�J�n
	 */
	virtual void Start() = 0;

	/**
	 * @brief		�A�N�^�[�𓮍삳����
	 */
	virtual void Update() = 0;

	/**
	 * @brief		�s��̍X�V
	 */
	virtual void RefreshMatrix() = 0;

	/**
	 * @brief		�A�N�^�[��\��������
	 */
	virtual void Render() = 0;

	/**
	 * @brief		name�ւ�Accessor
	 * @param[in]	nm	���O
	 */
	virtual void Name(const char* nm) = 0;

	/**
	 * @brief		�p�����[�^�[�̎擾
	 * @return		�p�����[�^�[
	 */
	virtual const ParameterArray& GetParameters(void) const = 0;

	/**
	 * @brief		�X�V�@�\�̎擾
	 * @return		�X�V�@�\
	 */
	virtual const ActionArray& GetActions(void) const = 0;

	/**
	 * @brief		�`��@�\�̎擾
	 * @return		�`��@�\
	 */
	virtual const RenderArray& GetRenders(void) const = 0;

	/**
	 * @brief		�A�N�^�[�̍X�V�L��
	 * @return		true	�X�V����
	 *				false	�X�V�Ȃ�
	 */
	virtual bool IsAction() const = 0;

	/**
	 * @brief		�A�N�^�[�̕`��L��
	 * @return		true	�`�悠��
	 *				false	�`��Ȃ�
	 */
	virtual bool IsRender() const = 0;

	/**
	 * @brief		pos�ւ�Accessor
	 * @param[in]	p	�ʒu
	 */
	virtual void Position(const Vector3F& p) = 0;
	/**
	 * @brief		pos�ւ�Accessor
	 * @return		�ʒu
	 */
	virtual const Vector3F& Position() const noexcept = 0;

	/**
	 * @brief		rotation�ւ�Accessor
	 * @param[in]	p	����
	 */
	virtual void Rotation(const Vector3F& p) = 0;
	/**
	 * @brief		rotation�ւ�Accessor
	 * @return		����
	 */
	virtual const Vector3F& Rotation() const noexcept = 0;

	/**
	 * @brief		scale�ւ�Accessor
	 * @param[in]	p	�傫��
	 */
	virtual void Scale(const Vector3F& p) = 0;
	/**
	 * @brief		scale�ւ�Accessor
	 * @return		�傫��
	 */
	virtual const Vector3F& Scale() const noexcept = 0;

	/**
	 * @brief		WorldMatrix�ւ�Accessor
	 * @return		�s��
	 */
	virtual const glm::mat4& Matrix() const noexcept = 0;

	/**
	 * @brief		parent�ւ�Accessor
	 * @param[in]	p	�e�|�C���^
	 */
	virtual void Parent(const ActorPtr& p) = 0;
	/**
	 * @brief		parent�ւ�Accessor
	 * @return		�e�|�C���^
	 */
	virtual const ActorPtr& Parent() const noexcept = 0;
	/**
	 * @brief		name�ւ�Accessor
	 * @return		���O
	 */
	virtual const std::string& ParentName() const noexcept = 0;

	/**
	 * @brief		�I���t���O�𗧂Ă�
	 */
	virtual void Destroy() = 0;

	/**
	 * @brief		�A�N�e�B�u�t���O����
	 * @param[in]	f	�t���O
	 */
	virtual void SetActive(const bool f) = 0;

	/**
	 * @brief		���씻��
	 * @return		����t���O
	 */
	virtual bool IsActive() const = 0;
	/**
	 * @brief		�I������
	 * @return		�I���t���O
	 */
	virtual bool IsEnd() const = 0;

	/**
	 * @brief		JSON����
	 * @param[in]	os	JSON�t�@�C��
	 */
	virtual void ToJson(nlohmann::json& os) = 0;
	/**
	 * @brief		JSON����
	 * @param[in]	os	JSON�t�@�C��
	 */
	virtual void ToJsonImpl(nlohmann::json& os) = 0;

	/**
	 * @brief		JSON���琶��
	 * @param[in]	os	JSON�t�@�C��
	 */
	virtual void FromJson(nlohmann::json& os) = 0;
	/**
	 * @brief		JSON���琶��
	 * @param[in]	os	JSON�t�@�C��
	 */
	virtual void FromJsonImpl(nlohmann::json& os) = 0;
};
#pragma endregion
//--------------------------------------------------------------

}