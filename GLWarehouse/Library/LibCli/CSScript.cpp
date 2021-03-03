#include	<iostream>
#include	"CSScript.h"
#include	"IScript.h"
#include	"SharedWrapper.h"

using namespace Sample;
using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::CodeDom;
using namespace System::CodeDom::Compiler;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace Microsoft::CSharp;

std::list< CSScriptImplPtr >			scriptLists;

public ref class ScriptCompiler {
private:
	CompilerParameters^						params;
	Dictionary<String^, String^>^			providerOptions;
	CSharpCodeProvider^						codeProvider;
	Assembly^								assembly;
	String^									Directory;
	bool									bChanged;
	bool									bWatch;
public:
	/**
	 * @brief		������
	 * @param[in]	bWatch			�t�@�C���X�V�Ď��t���O
	 *								true�̏ꍇ�A�X�V���������ꍇ��Reload�����s����
	 */
	void Initialize(bool bwatch) {
		bWatch = bwatch;
		//�R���p�C���p�̃p�����[�^�[��ݒ�
		params = gcnew CompilerParameters();
		params->GenerateInMemory = true;
		params->IncludeDebugInformation = false;
		params->ReferencedAssemblies->Add(Assembly::GetEntryAssembly()->Location);

		//-------------------------------------------------------------------
		//CS�X�N���v�g��DLL�̒ǉ����K�v�ȏꍇ�͂����Œ�`
		//params->ReferencedAssemblies->Add("System.Windows.Forms.dll");

		providerOptions = gcnew Dictionary<String^, String^>();
		providerOptions->Add("CompilerVersion", "v4.0");
		codeProvider = gcnew CSharpCodeProvider();
	}

	/**
	 * @brief		�ǂݍ���
	 *				�w�肳�ꂽ�f�B���N�g���ɑ��݂���cs�t�@�C�����R���p�C������
	 * @param[in]	dir			�X�N���v�g�ۑ��f�B���N�g��
	 */
	void Load(String^ dir) {
		//�f�B���N�g���ۑ�
		Directory = dir;
		//�t�@�C���Ď�
		if (bWatch)
		{
			// �X�V���������ꍇ�ɕύX�C�x���g�����s����
			FileSystemWatcher^ fsWatcher = gcnew FileSystemWatcher();
			fsWatcher->Path = dir;
			fsWatcher->NotifyFilter = static_cast<NotifyFilters>
				(NotifyFilters::FileName | NotifyFilters::Attributes | NotifyFilters::LastAccess |
					NotifyFilters::LastWrite | NotifyFilters::Security | NotifyFilters::Size);
			fsWatcher->Changed += gcnew FileSystemEventHandler(this, &ScriptCompiler::OnChanged);
			fsWatcher->EnableRaisingEvents = true;
		}
		// cs �t�@�C�������X�g�ɒǉ��ϊ�
		array<String^>^ file = Directory::GetFiles(dir);
		ArrayList^ alist = gcnew ArrayList();
		for each(String^ fname in file)
		{
			if (fname->IndexOf(".cs") >= 0)
			{
				alist->Add(System::IO::File::ReadAllText(fname));
			}
		}
		//�\�[�X���X�g�Ƃ��ăR���p�C�����ăA�Z���u�����擾
		array<String^>^ sources = reinterpret_cast<array<String^>^>(alist->ToArray(String::typeid));
		CompilerResults^ compilerResults = codeProvider->CompileAssemblyFromSource(params, sources);
		for each (CompilerError^ error in compilerResults->Errors)
		{
			System::Windows::Forms::MessageBox::Show(error->ErrorText);
		}
		assembly = compilerResults->CompiledAssembly;
	}

	/**
	 * @brief		�X�N���v�g�̍ēǂݍ���
	 */
	bool ReLoad()
	{
		// cs �t�@�C���ϊ�
		array<String^>^ file = Directory::GetFiles(Directory);
		ArrayList^ alist = gcnew ArrayList();
		for each (String ^ fname in file)
		{
			if (fname->IndexOf(".cs") >= 0)
			{
				try
				{
					alist->Add(System::IO::File::ReadAllText(fname));
				}
				catch (IOException^ e)
				{
					std::string emsg = msclr::interop::marshal_as<std::string>(e->Message);
					std::cout << emsg << std::endl;
					return false;
				}
			}
		}
		array<String^>^ sources = reinterpret_cast<array<String^>^>(alist->ToArray(String::typeid));
		CompilerResults^ compilerResults = codeProvider->CompileAssemblyFromSource(params, sources);
		for each (CompilerError^ error in compilerResults->Errors)
		{
			System::Windows::Forms::MessageBox::Show(error->ErrorText);
		}
		assembly = compilerResults->CompiledAssembly;
		bChanged = false;
		return true;
	}

	void OnChanged(Object^ source, FileSystemEventArgs^ e)
	{
		bChanged = true;
		for (auto& v : scriptLists)
		{
			v->SetChange(true);
		}
	}

	Assembly^ GetAssembly() {
		return assembly;
	}

	bool IsChanged() {
		return bChanged;
	}
};
msclr::gcroot<ScriptCompiler^>			scripts;

public class CSScriptImpl : public ICSScriptImpl {
private:
	/** �X�N���v�g�� */
	msclr::gcroot<String^>			name;
	/** �X�N���v�g�Q�� */
	msclr::gcroot<IScript^>			scriptRef;
	/** ���L�f�[�^�Q�� */
	msclr::gcroot<SharedWrapper^>	shared;
	/** �ύX�t���O */
	bool							bChanged;
public:
	/**
	 * @brief		CS�X�N���v�g�̃A�Z���u�����擾
	 * @param[in]	fname		�擾����A�Z���u����
	 */
	void Load(const std::string& fname) override
	{
		// �R���p�C�������A�Z���u�����擾
		name = msclr::interop::marshal_as<String^>(fname);
		Type^ type = scripts->GetAssembly()->GetType(name);
		scriptRef = (IScript^)Activator::CreateInstance(type);
	}


	/**
 	 * @brief		�X�N���v�g�̍Ď擾
	 */
	void ReLoad() {
		//�X�N���v�g���g�̍X�V
		if (scripts->IsChanged())
		{
			if (!scripts->ReLoad())
			{
				return;
			}
		}
		// �R���p�C�������A�Z���u�����Ď擾
		Type^ type = scripts->GetAssembly()->GetType(name);
		scriptRef = (IScript^)Activator::CreateInstance(type);
		bChanged = false;
	}

	/**
	* @brief		���L�f�[�^�̐ݒ�
	*/
	void SetShared(ISharedActor* pShared) override {
		shared = gcnew SharedWrapper(pShared);
	}

	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	void Start() override {
		if (bChanged)
		{
			ReLoad();
		}
		scriptRef->CallStart(shared);
	}

	/**
	 * @brief		�X�N���v�g�̎��s
	 */
	void Update() override {
		if (bChanged)
		{
			ReLoad();
		}
		scriptRef->CallUpdate(shared);
	}


	/**
	 * @brief		�ύX�t���O�̐ݒ�
	 */
	void SetChange(bool change) override {
		bChanged = change;
	}
};

/**
 * @brief		�R���X�g���N�^
 */
CSScript::CSScript(ISharedActor* ps)
: pImpl()
, pShared(ps) {
}

/**
 * @brief		�f�X�g���N�^
 */
CSScript::~CSScript() {
	scriptLists.remove(pImpl);
	pImpl.reset();
}

/**
 * @brief		CS�X�N���v�g�̃A�Z���u�����擾
 * @param[in]	classname		�擾����A�Z���u����
 */
void CSScript::Load(const std::string& classname) {
	//�����N���X����
	pImpl = std::make_shared< CSScriptImpl >();
	pImpl->SetShared(pShared);
	pImpl->Load(classname);
	scriptLists.push_back(pImpl);
}

/**
* @brief		�X�N���v�g�̎��s
*/
void CSScript::Start() {
	pImpl->Start();
}

/**
 * @brief		�X�N���v�g�̎��s
 */
void CSScript::Update() {
	pImpl->Update();
}

/**
 * @brief		�w�肳�ꂽ�f�B���N�g���ɑ��݂���cs�t�@�C�����R���p�C������
 * @param[in]	directory		�X�N���v�g�ۑ��f�B���N�g��
 * @param[in]	bWatch			�t�@�C���X�V�Ď��t���O
 *								true�̏ꍇ�A�X�V���������ꍇ��Reload�����s����
 */
void Sample::ScriptCompile(const std::string& directory, bool bwatch) {
	String^ dir = msclr::interop::marshal_as<String^>(directory);
	scripts = gcnew ScriptCompiler();
	scripts->Initialize(bwatch);
	scripts->Load(dir);
}