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
	 * @brief		初期化
	 * @param[in]	bWatch			ファイル更新監視フラグ
	 *								trueの場合、更新があった場合にReloadを実行する
	 */
	void Initialize(bool bwatch) {
		bWatch = bwatch;
		//コンパイル用のパラメーターを設定
		params = gcnew CompilerParameters();
		params->GenerateInMemory = true;
		params->IncludeDebugInformation = false;
		params->ReferencedAssemblies->Add(Assembly::GetEntryAssembly()->Location);

		//-------------------------------------------------------------------
		//CSスクリプトでDLLの追加が必要な場合はここで定義
		//params->ReferencedAssemblies->Add("System.Windows.Forms.dll");

		providerOptions = gcnew Dictionary<String^, String^>();
		providerOptions->Add("CompilerVersion", "v4.0");
		codeProvider = gcnew CSharpCodeProvider();
	}

	/**
	 * @brief		読み込み
	 *				指定されたディレクトリに存在するcsファイルをコンパイルする
	 * @param[in]	dir			スクリプト保存ディレクトリ
	 */
	void Load(String^ dir) {
		//ディレクトリ保存
		Directory = dir;
		//ファイル監視
		if (bWatch)
		{
			// 更新があった場合に変更イベントを実行する
			FileSystemWatcher^ fsWatcher = gcnew FileSystemWatcher();
			fsWatcher->Path = dir;
			fsWatcher->NotifyFilter = static_cast<NotifyFilters>
				(NotifyFilters::FileName | NotifyFilters::Attributes | NotifyFilters::LastAccess |
					NotifyFilters::LastWrite | NotifyFilters::Security | NotifyFilters::Size);
			fsWatcher->Changed += gcnew FileSystemEventHandler(this, &ScriptCompiler::OnChanged);
			fsWatcher->EnableRaisingEvents = true;
		}
		// cs ファイルをリストに追加変換
		array<String^>^ file = Directory::GetFiles(dir);
		ArrayList^ alist = gcnew ArrayList();
		for each(String^ fname in file)
		{
			if (fname->IndexOf(".cs") >= 0)
			{
				alist->Add(System::IO::File::ReadAllText(fname));
			}
		}
		//ソースリストとしてコンパイルしてアセンブリを取得
		array<String^>^ sources = reinterpret_cast<array<String^>^>(alist->ToArray(String::typeid));
		CompilerResults^ compilerResults = codeProvider->CompileAssemblyFromSource(params, sources);
		for each (CompilerError^ error in compilerResults->Errors)
		{
			System::Windows::Forms::MessageBox::Show(error->ErrorText);
		}
		assembly = compilerResults->CompiledAssembly;
	}

	/**
	 * @brief		スクリプトの再読み込み
	 */
	bool ReLoad()
	{
		// cs ファイル変換
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
	/** スクリプト名 */
	msclr::gcroot<String^>			name;
	/** スクリプト参照 */
	msclr::gcroot<IScript^>			scriptRef;
	/** 共有データ参照 */
	msclr::gcroot<SharedWrapper^>	shared;
	/** 変更フラグ */
	bool							bChanged;
public:
	/**
	 * @brief		CSスクリプトのアセンブリを取得
	 * @param[in]	fname		取得するアセンブリ名
	 */
	void Load(const std::string& fname) override
	{
		// コンパイルしたアセンブリを取得
		name = msclr::interop::marshal_as<String^>(fname);
		Type^ type = scripts->GetAssembly()->GetType(name);
		scriptRef = (IScript^)Activator::CreateInstance(type);
	}


	/**
 	 * @brief		スクリプトの再取得
	 */
	void ReLoad() {
		//スクリプト自身の更新
		if (scripts->IsChanged())
		{
			if (!scripts->ReLoad())
			{
				return;
			}
		}
		// コンパイルしたアセンブリを再取得
		Type^ type = scripts->GetAssembly()->GetType(name);
		scriptRef = (IScript^)Activator::CreateInstance(type);
		bChanged = false;
	}

	/**
	* @brief		共有データの設定
	*/
	void SetShared(ISharedActor* pShared) override {
		shared = gcnew SharedWrapper(pShared);
	}

	/**
	 * @brief		スクリプトの実行
	 */
	void Start() override {
		if (bChanged)
		{
			ReLoad();
		}
		scriptRef->CallStart(shared);
	}

	/**
	 * @brief		スクリプトの実行
	 */
	void Update() override {
		if (bChanged)
		{
			ReLoad();
		}
		scriptRef->CallUpdate(shared);
	}


	/**
	 * @brief		変更フラグの設定
	 */
	void SetChange(bool change) override {
		bChanged = change;
	}
};

/**
 * @brief		コンストラクタ
 */
CSScript::CSScript(ISharedActor* ps)
: pImpl()
, pShared(ps) {
}

/**
 * @brief		デストラクタ
 */
CSScript::~CSScript() {
	scriptLists.remove(pImpl);
	pImpl.reset();
}

/**
 * @brief		CSスクリプトのアセンブリを取得
 * @param[in]	classname		取得するアセンブリ名
 */
void CSScript::Load(const std::string& classname) {
	//内部クラス生成
	pImpl = std::make_shared< CSScriptImpl >();
	pImpl->SetShared(pShared);
	pImpl->Load(classname);
	scriptLists.push_back(pImpl);
}

/**
* @brief		スクリプトの実行
*/
void CSScript::Start() {
	pImpl->Start();
}

/**
 * @brief		スクリプトの実行
 */
void CSScript::Update() {
	pImpl->Update();
}

/**
 * @brief		指定されたディレクトリに存在するcsファイルをコンパイルする
 * @param[in]	directory		スクリプト保存ディレクトリ
 * @param[in]	bWatch			ファイル更新監視フラグ
 *								trueの場合、更新があった場合にReloadを実行する
 */
void Sample::ScriptCompile(const std::string& directory, bool bwatch) {
	String^ dir = msclr::interop::marshal_as<String^>(directory);
	scripts = gcnew ScriptCompiler();
	scripts->Initialize(bwatch);
	scripts->Load(dir);
}