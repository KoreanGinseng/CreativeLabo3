#pragma once

template <class... Args>
class Observer {
private:
public:
	/**
	 * @brief	�ʒm���\�b�h
	 */
	virtual void Notify(Args...) = 0;
};