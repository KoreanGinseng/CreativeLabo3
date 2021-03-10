#pragma once

#include		<memory>
#include		<list>

namespace Behaviour {

	//ノードポインタ置き換えのための前方定義
	template < typename T > class Node;
	template < typename T > using NodePtr  = std::shared_ptr<Node< T >>;
	template < typename T > using NodeList = std::list<NodePtr< T >>;

	//ノードポインタ置き換え
	template < typename T > class INodeExecutor;
	template < typename T > using NodeExecutorPtr     = std::shared_ptr<INodeExecutor<T>>;
	template < typename T > using NodeExecutorWeakPtr = std::weak_ptr<INodeExecutor<T>>;
	template < typename T > using NodeExecutorList    = std::list<NodeExecutorPtr<T>>;

	/** ノード実行用クラス */
	template < typename T >
	class INodeExecutor : public std::enable_shared_from_this<INodeExecutor<T>>
	{
	public:
		/**
		 * @brief		ノードの状態
		 */
		enum class State {
			Inactive,           //未実行
			Running,            //実行中
			Completed,          //実行終了
			Incompleted,		//実行失敗
		};

		/**
		 * @brief		ノードの結果
		 */
		enum class Result {
			None,               //まだ実行前/実行中
			Sucess,             //実行成功
			Failure,            //実行失敗
		};

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		Succeeded	実行の成功
		 *				Failed		実行の失敗
		 */
		virtual Result Exec(T& actor) = 0;

		/**
		 * @brief		ノードの実行状態を全てリセットして
		 *				状態をInactiveに設定する
		 */
		virtual void Reset() = 0;

		/**
		 * @brief		親の設定
		 * @param[in]	ptr			親アクター
		 */
		virtual void SetParent(NodeExecutorWeakPtr<T>& ptr) = 0;

		virtual State GetState() = 0;

		virtual const NodePtr<T>& GetNode() const = 0;

		virtual const NodeExecutorList<T>& GetChild() const = 0;
	};
}