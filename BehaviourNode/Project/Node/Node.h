#pragma once

#include		"Executor/INodeExecutor.h"

namespace Behaviour {

	//ノードポインタ置き換えのための前方定義
	template < typename T > class Node;
	template < typename T > using NodePtr = std::shared_ptr<Node< T >>;
	template < typename T > using NodeList = std::list<NodePtr< T >>;

	template < typename T >
	class Node : public std::enable_shared_from_this<Node<T>>
	{
	protected:
		/** ノード名 */
		std::string _name;
	public:
		Node(const std::string& name)
			: _name(name) {
		}
		virtual ~Node() = default;

		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		true		実行の成功
		 *				false		実行の失敗
		 */
		virtual bool Exec(T& actor) { return false; }

		virtual NodeExecutorPtr< T > CreateExecutor() const = 0;
		const std::string& GetName() const noexcept { return _name; }
	};
}