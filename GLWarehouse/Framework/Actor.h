#pragma once

#include	"Interface.h"

namespace Sample {

//--------------------------------------------------------------
#pragma region ���A�N�^�[�֘A
//--------------------------------------------------------------
class Actor : public IActor {
protected:
	/** �A�N�^�[�̖��O */
	std::string		name_;
	/** �A�N�^�[�̈ʒu */
	Vector3F		pos_;
	/** �A�N�^�[�̌��� */
	Vector3F		rotation_;
	/** �A�N�^�[�̑傫�� */
	Vector3F		scale_;

	/** �s�� */
	glm::mat4		LocalMatrix_;
	/** �s�� */
	glm::mat4		WorldMatrix_;

	/** �A�N�e�B�u�t���O */
	bool			active_;
	/** �I���t���O */
	bool			end_;

	/** �A�N�^�[�p�����[�^�[ */
	ParameterArray	parameter_;
	/** �A�N�^�[����@�\ */
	ActionArray		action_;
	/** �A�N�^�[�`��@�\ */
	RenderArray		render_;

	/** �e�A�N�^�[ */
	ActorPtr		parent_;
	/** �e�A�N�^�[�̖��O */
	std::string		parentName_;
	/** �q�� */
	ActorListPtr	children_;
public:
	/**
	 * @brief		�R���X�g���N�^
	 * @param[in]	nm	���O
	 */
	Actor(const char* nm)
	: name_(nm)
	, pos_()
	, rotation_()
	, scale_(1, 1, 1)
	, LocalMatrix_()
	, WorldMatrix_()
	, active_(true)
	, end_(false)
	, parameter_()
	, action_()
	, render_() 
	, parent_()
	, parentName_()
	, children_(std::make_shared<ActorList>()) {
	}
	/**
	 * @brief		�R�s�[�R���X�g���N�^
	 * @param[in]	obj	�R�s�[����A�N�^�[
	 */
	Actor(const Actor& obj)
	: name_(obj.name_)
	, pos_(obj.pos_)
	, rotation_(obj.rotation_)
	, scale_(obj.scale_)
	, LocalMatrix_(obj.LocalMatrix_)
	, WorldMatrix_(obj.WorldMatrix_)
	, active_(obj.active_)
	, end_(obj.end_)
	, parameter_()
	, action_()
	, render_()
	, parent_()
	, parentName_(obj.parentName_)
	, children_(std::make_shared<ActorList>()) {
		std::transform(obj.parameter_.begin(), obj.parameter_.end(), back_inserter(parameter_), [&](const ParameterPtr& base)
			{
				ParameterPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.action_.begin(), obj.action_.end(), back_inserter(action_), [&](const ActionPtr& base)
			{
				ActionPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.render_.begin(), obj.render_.end(), back_inserter(render_), [&](const RenderPtr& base)
			{
				RenderPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.children_->begin(), obj.children_->end(), back_inserter(*children_), [&](const ActorPtr& base)
			{
				return std::make_shared<Actor>(*base);
			}
		);
	}

	/**
	 * @brief		�f�X�g���N�^
	 */
	virtual ~Actor() {
		children_.reset();
	}

	/**
	 * @brief		�J�n
	 */
	void Start() override {
		std::for_each(parameter_.begin(), parameter_.end(), [](const ParameterPtr& act) { act->Start(); });
		std::for_each(action_.begin(), action_.end(), [](const ActionPtr& act) { act->Start(); });
		std::for_each(render_.begin(), render_.end(), [](const RenderPtr& act){ act->Start(); } );
	}

	/**
	 * @brief		�A�N�^�[�𓮍삳����
	 */
	void Update() override {
		//����X�V
		for (auto it : action_)
		{
			if (!it->Enabled())
			{
				continue;
			}
			it->Update();
		}

		//�s��X�V
		RefreshMatrix();

		//�q���̍X�V
		for (auto it : *children_)
		{
			if (!it->IsActive())
			{
				continue;
			}
			it->Update();
		}
	}

	/**
	 * @brief		�s��̍X�V
	 */
	void RefreshMatrix() override {
		const glm::mat4 matT = glm::translate(glm::mat4(1), glm::vec3(pos_.x, -pos_.y, pos_.z));
		const glm::mat4 matRY = glm::rotate(matT, glm::radians(rotation_.y), glm::vec3(0, 1, 0));
		const glm::mat4 matRXY = glm::rotate(matRY, glm::radians(rotation_.x), glm::vec3(1, 0, 0));
		const glm::mat4 matRZXY = glm::rotate(matRXY, glm::radians(-rotation_.z), glm::vec3(0, 0, 1));
		const glm::mat4 matS = glm::scale(matRZXY, glm::vec3(scale_.x, scale_.y, scale_.z));
		LocalMatrix_ = matS;
		if (parent_)
		{
			WorldMatrix_ = parent_->Matrix() * LocalMatrix_;
		}
		else
		{
			WorldMatrix_ = LocalMatrix_;
		}
	}

	/**
	 * @brief		�A�N�^�[��\��������
	 */
	void Render() override {
		for (auto it : render_)
		{
			if (!it->Enabled())
			{
				continue;
			}
			it->Render();
		}
	}

	/**
	 * @brief		name�ւ�Accessor
	 * @param[in]	nm	���O
	 */
	void Name(const char* nm) override { name_ = nm; }
	/**
	 * @brief		name�ւ�Accessor
	 * @return		���O
	 */
	const std::string& Name() const noexcept override { return name_; }

	/**
	 * @brief		�p�����[�^�[�̒ǉ�
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddParameter(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		parameter_.push_back(add);
		return add;
	}

	/**
	 * @brief		����@�\�̒ǉ�
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddAction(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		action_.push_back(add);
		return add;
	}

	/**
	 * @brief		�`��@�\�̒ǉ�
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddRender(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		render_.push_back(add);
		return add;
	}

	/**
	 * @brief		�R���|�[�l���g�̎擾
	 * @return		�e���v���[�g�̌^�̃R���|�[�l���g������ꍇ�擾����
	 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
	 */
	template< typename T >
	std::shared_ptr< T > GetComponent(void) const {
		auto pt = GetParameter< T >();
		if (pt)
		{
			return pt;
		}
		auto at = GetAction< T >();
		if (at)
		{
			return at;
		}
		auto rt = GetRender< T >();
		if (rt)
		{
			return rt;
		}
		return std::shared_ptr< T >();
	}
	
	/**
	 * @brief		�R���|�[�l���g�̎擾
	 * @return		�e���v���[�g�̌^�̃R���|�[�l���g������ꍇ�擾����
	 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetComponents(void) {
		std::vector< std::shared_ptr< T > > re;
		auto pt = GetParameters< T >();
		auto at = GetActions< T >();
		auto rt = GetRenders< T >();
		std::copy(pt.begin(), pt.end(), std::back_inserter(re));
		std::copy(at.begin(), at.end(), std::back_inserter(re));
		std::copy(rt.begin(), rt.end(), std::back_inserter(re));
		return re;
	}

	/**
	 * @brief		�p�����[�^�[�̎擾
	 * @return		�e���v���[�g�̌^�̃p�����[�^�[������ꍇ�擾����
	 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
	 */
	template< typename T >
	std::shared_ptr< T > GetParameter(void) const {
		for (auto it : parameter_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast<T>(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		�p�����[�^�[�̎擾
	 * @return		�e���v���[�g�̌^�̃p�����[�^�[������ꍇ�擾����
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetParameters() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : parameter_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		����@�\�̎擾
	 * @return		�e���v���[�g�̌^�̓��삪����ꍇ�擾����
	 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
	 */
	template< typename T >
	std::shared_ptr< T > GetAction(void) const {
		for (auto it : action_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast< T >(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		����@�\�̎擾
	 * @return		�e���v���[�g�̌^�̃p�����[�^�[������ꍇ�擾����
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetActions() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : action_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		�`��@�\�̎擾
	 * @return		�e���v���[�g�̌^�̕`�悪����ꍇ�擾����
	 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
	 */
	template< typename T >
	std::shared_ptr< T > GetRender(void) const {
		for (auto it : render_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast< T >(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		�`��@�\�̎擾
	 * @return		�e���v���[�g�̌^�̃p�����[�^�[������ꍇ�擾����
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetRenders() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : render_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		�p�����[�^�[�̎擾
	 * @return		�p�����[�^�[
	 */
	const ParameterArray& GetParameters(void) const override {
		return parameter_;
	}

	/**
	 * @brief		�X�V�@�\�̎擾
	 * @return		�X�V�@�\
	 */
	const ActionArray& GetActions(void) const override {
		return action_;
	}

	/**
	 * @brief		�`��@�\�̎擾
	 * @return		�`��@�\
	 */
	const RenderArray& GetRenders(void) const override {
		return render_;
	}

	/**
	 * @brief		�A�N�^�[�̍X�V�L��
	 * @return		true	�X�V����
	 *				false	�X�V�Ȃ�
	 */
	bool IsAction() const override {
		return action_.size() > 0;
	}

	/**
	 * @brief		�A�N�^�[�̕`��L��
	 * @return		true	�`�悠��
	 *				false	�`��Ȃ�
	 */
	bool IsRender() const override {
		return render_.size() > 0;
	}

	/**
	 * @brief		pos�ւ�Accessor
	 * @param[in]	p	�ʒu
	 */
	void Position(const Vector3F& p) override { pos_ = p; }
	/**
	 * @brief		pos�ւ�Accessor
	 * @param[in]	x	X�ʒu
	 * @param[in]	y	Y�ʒu
	 */
	void Position(const float x, const float y) override { pos_.x = x;	 pos_.y = y; }
	/**
	 * @brief		pos�ւ�Accessor
	 * @return		�ʒu
	 */
	const Vector3F& Position() const noexcept override { return pos_; }

	/**
	 * @brief		posx�ւ�Accessor
	 * @param[in]	x	X�ʒu
	 */
	void X(const float x) override { pos_.x = x; }
	/**
	 * @brief		posx�ւ�Accessor
	 * @return		X�ʒu
	 */
	const float X() const noexcept override { return pos_.x; }

	/**
	 * @brief		posy�ւ�Accessor
	 * @param[in]	y	Y�ʒu
	 */
	void Y(const float y) override { pos_.y = y; }
	/**
	 * @brief		posy�ւ�Accessor
	 * @return		Y�ʒu
	 */
	const float Y() const noexcept override { return pos_.y; }
	
	/**
	 * @brief		posz�ւ�Accessor
	 * @param[in]	z	Z�ʒu
	 */
	void Z(const float z) override { pos_.z = z; }
	/**
	 * @brief		posz�ւ�Accessor
	 * @return		Z�ʒu
	 */
	const float Z() const noexcept override { return pos_.z; }

	/**
	 * @brief		rotation�ւ�Accessor
	 * @param[in]	p	����
	 */
	void Rotation(const Vector3F& p) override { rotation_ = p; }
	/**
	 * @brief		rotation�ւ�Accessor
	 * @return		����
	 */
	const Vector3F& Rotation() const noexcept override { return rotation_; }

	/**
	 * @brief		scale�ւ�Accessor
	 * @param[in]	p	�傫��
	 */
	void Scale(const Vector3F& p) override { scale_ = p; }
	/**
	 * @brief		scale�ւ�Accessor
	 * @return		�傫��
	 */
	const Vector3F& Scale() const noexcept override { return scale_; }

	/**
	 * @brief		WorldMatrix�ւ�Accessor
	 * @return		�s��
	 */
	const glm::mat4& Matrix() const noexcept override { return WorldMatrix_; }

	/**
	 * @brief		parent�ւ�Accessor
	 * @param[in]	p	�e�|�C���^
	 */
	void Parent(const ActorPtr& p) override;
	/**
	 * @brief		parent�ւ�Accessor
	 * @return		�e�|�C���^
	 */
	const ActorPtr& Parent() const noexcept override { return parent_; }
	/**
	 * @brief		name�ւ�Accessor
	 * @return		���O
	 */
	const std::string& ParentName() const noexcept override { return parentName_; }

	/**
	 * @brief		�I���t���O�𗧂Ă�
	 */
	void Destroy() override { end_ = true; }

	/**
	 * @brief		�A�N�e�B�u�t���O����
	 * @param[in]	f	�t���O
	 */
	void SetActive(const bool f) override { active_ = f; }

	/**
	 * @brief		���씻��
	 * @return		����t���O
	 */
	bool IsActive() const override { return active_; }
	/**
	 * @brief		�I������
	 * @return		�I���t���O
	 */
	bool IsEnd() const override { return end_; }

	/**
	 * @brief		JSON����
	 * @param[in]	os	JSON�t�@�C��
	 */
	void ToJson(nlohmann::json& os) override;
	/**
	 * @brief		JSON����
	 * @param[in]	os	JSON�t�@�C��
	 */
	void ToJsonImpl(nlohmann::json& os) override;

	/**
	 * @brief		JSON���琶��
	 * @param[in]	os	JSON�t�@�C��
	 */
	void FromJson(nlohmann::json& os) override;
	/**
	 * @brief		JSON���琶��
	 * @param[in]	os	JSON�t�@�C��
	 */
	void FromJsonImpl(nlohmann::json& os) override;
};

template <typename T>
class ParameterBase : public IParameter {
protected:
	/** �A�N�^�[ */
	Actor* actor;
public:
	/**
	 * @brief		�R���X�g���N�^
	 */
	ParameterBase()
	: actor(nullptr) {
	}
	/**
	 * @brief		�R�s�[�R���X�g���N�^
	 */
	ParameterBase(const ParameterBase& act)
	: actor(nullptr) {
	}

	/**
	 * @brief		������
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		�J�n
	 */
	virtual void Start() {
	}

	/**
	 * @brief		�R�s�[�̐���
	 */
	ParameterPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		�Ăяo�����A�N�^�[�̐ݒ�
	 * @param[in]	ac	���A�N�^�[
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		�Ăяo�����A�N�^�[�̎擾
	 * @return		���A�N�^�[
	 */
	Actor* GetActor(void) override { return actor; }
};

template <typename T>
class ActionBase : public IAction {
protected:
	/** �A�N�^�[ */
	Actor* actor;

	/** �L���t���O */
	bool Enabled_;
public:
	/**
	 * @brief		�R���X�g���N�^
	 */
	ActionBase()
	: actor(nullptr)
	, Enabled_(true) {
	}
	/**
	 * @brief		�R�s�[�R���X�g���N�^
	 */
	ActionBase(const ActionBase& act)
	: actor(nullptr) 
	, Enabled_(act.Enabled_){
	}

	/**
	 * @brief		������
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		�J�n
	 */
	virtual void Start() {
	}

	/**
	 * @brief		�R�s�[�̐���
	 */
	ActionPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		�Ăяo�����A�N�^�[�̐ݒ�
	 * @param[in]	ac	���A�N�^�[
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		�Ăяo�����A�N�^�[�̎擾
	 * @return		���A�N�^�[
	 */
	Actor* GetActor(void) override { return actor; }

	/**
	 * @brief		�L���t���O�̐ݒ�
	 * @param[in]	p	�L���t���O
	 */
	void Enabled(const bool p) noexcept override { Enabled_ = p; }
	/**
	 * @brief		�L���t���O�̎擾
	 * @return		�L���t���O
	 */
	const bool Enabled(void) const noexcept override { return Enabled_; }
};

template <typename T>
class RenderBase : public IRender {
protected:
	/** �A�N�^�[ */
	Actor* actor;

	/** �L���t���O */
	bool Enabled_;

	/** �`��D�揇�� */
	int Priority_;
public:
	/**
	 * @brief		�R���X�g���N�^
	 */
	RenderBase() 
	: actor(nullptr) 
	, Enabled_(true) 
	, Priority_(0) {
	}
	/**
	 * @brief		�R�s�[�R���X�g���N�^
	 */
	RenderBase(const RenderBase& act)
	: actor(nullptr) 
	, Enabled_(act.Enabled_)
	, Priority_(act.Priority_) {
	}

	/**
	 * @brief		������
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		�J�n
	 */
	virtual void Start() {
	}

	/**
	 * @brief		�R�s�[�̐���
	 */
	RenderPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		�Ăяo�����A�N�^�[�̐ݒ�
	 * @param[in]	ac	���A�N�^�[
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		�Ăяo�����A�N�^�[�̎擾
	 * @return		���A�N�^�[
	 */
	Actor* GetActor(void) override { return actor; }

	/**
	 * @brief		�L���t���O�̐ݒ�
	 * @param[in]	p	�L���t���O
	 */
	void Enabled(const bool p) noexcept override { Enabled_ = p; }
	/**
	 * @brief		�L���t���O�̎擾
	 * @return		�L���t���O
	 */
	const bool Enabled(void) const noexcept override { return Enabled_; }

	/**
	 * @brief		�`��D�揇�ʂ̐ݒ�
	 * @param[in]	p	�`��D�揇��
	 */
	void Priority(const int p) noexcept override { Priority_ = p; }
	/**
	 * @brief		�`��D�揇�ʂ̎擾
	 * @return		�`��D�揇��
	 */
	const int Priority(void) const noexcept override{ return Priority_; }
};
#pragma endregion
//--------------------------------------------------------------

}