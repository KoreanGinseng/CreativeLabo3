#pragma once

#include	<vector>
#include	<algorithm>
#include	"Observable.h"

template <class... Args>
class Subject : public IObservable<Args...> {
private:
	/** 通知を受けるオブザーバーリスト */
	std::vector<Observer<Args...>*> observerList;
public:
	/**
	 * コンストラクタ
	 */
	Subject()
		: observerList()
	{
	}
	/**
	 * デストラクタ
	 */
	virtual ~Subject()
	{
	}

	/**
	 * @brief	通知を受けるオブザーバーの登録
	 */
	void Subscribe(Observer<Args...>* pobs) override {
		observerList.push_back(pobs);
	}

	/**
	 * @brief	通知を受けるオブザーバーの登録
	 */
	ObserverFunction<Args...>* Subscribe(std::function<void(Args...)> f) override {
		ObserverFunction<Args...>* func = new ObserverFunction<Args...>(f);
		observerList.push_back(func);
		return func;
	}

	/**
	 * @brief	通知を受けるオブザーバーの削除
	 */
	void Dispose(Observer<Args...>* pobs) override {
		observerList.erase(std::remove_if(
			observerList.begin(), observerList.end(),
			[&](const Observer<Args...>* o) {return o == pobs; }),
			observerList.end());
	}

	/**
	 * @brief	通知メソッド
	 */
	void Notify(Args... args) {
		for (auto& obj : observerList) {
			obj->Notify(args...);
		}
	}
};