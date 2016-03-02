#include "ETeamDef.h"
#include "TSimpleContainer.h"
#ifndef __T_SIMPLECONTAINER_CPP__
#define __T_SIMPLECONTAINER_CPP__
namespace Team
{
	template <class T>
	bool CountNotNuLL(T* pVal, size_t& rCount)
	{
		if (pVal != NULL)
		{
			++rCount;
		}
		return false;
	}

	template <class T>
	void TSimpleContainer<T>::UnSafeClear()
	{
		m_Items.clear();
	}

	template <class T>
	void TSimpleContainer<T>::UnSafeRemove(T* pVal)
	{
		if (NULL == pVal)
		{
			return;
		}

		ITEM_VECTOR::iterator iter = std::find(m_Items.begin(), m_Items.end(), pVal);
		if (iter != m_Items.end())
		{
			m_Items.erase(iter);
		}
	}

	template <class T>
	size_t TSimpleContainer<T>::Count() const
	{
// 		size_t count = 0;
// 		foreach1r(CountNotNuLL<T>, count);
// 		return count;
		return m_Items.size();
	}

	template <class T>
	bool TSimpleContainer<T>::Find(const T* pVal) const
	{
		if (NULL == pVal)
		{
			return false;
		}

		ITEM_VECTOR::const_iterator iter = std::find(m_Items.begin(), m_Items.end(), pVal);
		if (iter != m_Items.end())
		{
			return NULL != *iter;
		}

		return false;
	}

	template <class T>
	size_t TSimpleContainer<T>::FreeCount() const
	{
		return m_max - m_Items.size();
	}

	template <class T>
	typename TSimpleContainer<T>::ITEM_VECTOR::const_iterator TSimpleContainer<T>::cend() const
	{
		return m_Items.end();
	}

	template <class T>
	typename TSimpleContainer<T>::ITEM_VECTOR::const_iterator TSimpleContainer<T>::cbegin() const
	{
		return m_Items.begin();
	}

	template <class T>
	typename TSimpleContainer<T>::ITEM_VECTOR::iterator TSimpleContainer<T>::begin()
	{
		return m_Items.begin();
	}
	template <class T>
	typename TSimpleContainer<T>::ITEM_VECTOR::iterator TSimpleContainer<T>::end()
	{
		return m_Items.end();
	}

	template <class T>
	size_t TSimpleContainer<T>::IndexOf(const T* pVal) const
	{
		size_t index = 0;
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (*iter == pVal)
				return index;

			++index;
		}

		return AUTO_INCREASE_IDX;
	}


	template <class T>
	template <typename _FUNC>
	void TSimpleContainer<T>::foreach(_FUNC FUNC) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter))
			{
				break;
			}
		}
	}
	template <class T>
	template <typename _FUNC, typename T1>
	bool TSimpleContainer<T>::foreach1(_FUNC FUNC, T1 t1) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1))
			{
				return true;
			}
		}

		return false;
	}
	template <class T>
	template <typename _FUNC, typename T1>
	bool TSimpleContainer<T>::foreach1r(_FUNC FUNC, T1& t1) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1))
			{
				return true;
			}
		}

		return false;
	}
	template <class T>
	template <typename _FUNC, typename T1, typename T2>
	void TSimpleContainer<T>::foreach2(_FUNC FUNC, T1 t1, T2 t2) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2>
	void TSimpleContainer<T>::foreach2r(_FUNC FUNC, T1& t1, T2& t2) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2>
	void TSimpleContainer<T>::foreach2rend(_FUNC FUNC, T1 t1, T2& t2) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2, typename T3>
	void TSimpleContainer<T>::foreach3(_FUNC FUNC, T1 t1, T2 t2, T3 t3) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2, t3))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2, typename T3>
	void TSimpleContainer<T>::foreach3rend(_FUNC FUNC, T1 t1, T2 t2, T3& t3) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2, t3))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2, typename T3, typename T4>
	void TSimpleContainer<T>::foreach4(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4 t4) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2, t3, t4))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2, typename T3, typename T4>
	void TSimpleContainer<T>::foreach4rend(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4& t4) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2, t3, t4))
			{
				break;
			}
		}
	}

	template <class T>
	template <typename _FUNC, typename T1, typename T2, typename T3, typename T4, typename T5>
	void TSimpleContainer<T>::foreach5rend(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4 t4, T5& t5) const
	{
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1, t2, t3, t4, t5))
			{
				break;
			}
		}
	}


	template <class T>
	template <typename _FUNC, typename T1>
	size_t TSimpleContainer<T>::foreach_ra(_FUNC FUNC, T1& t1, size_t max) const
	{
		size_t idx = 0;
		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			if (FUNC(*iter, t1[idx++]))
			{
				break;
			}

			if (max <= idx)
			{
				break;
			}
		}

		return idx;
	}

	template <class T>
	template <typename _FUNC, typename T1>
	void TSimpleContainer<T>::each_between1rend(_FUNC FUNC, T1& t1) const
	{
		if (m_Items.size() < 2)
		{
			return;
		}

		ITEM_VECTOR::const_iterator iter = m_Items.begin();
		ITEM_VECTOR::const_iterator iterNext = iter;
		for (; iter != m_Items.end(); ++iter)
		{
			iterNext = iter;
			for (++iterNext; iterNext != m_Items.end();  ++iterNext)
			{
				FUNC(*iter, *iterNext, t1);
			}
		}
	}


	template <class T>
	inline size_t TSimpleContainer<T>::getMaxCount() const
	{
		return m_max;
	}




	template <class T>
	T* Team::TSimpleContainer<T>::Get(size_t idx) const
	{
		if (idx >= m_Items.size())
		{
			return NULL;
		}

		return m_Items[idx];
	}

	template <class T>
	T* Team::TSimpleContainer<T>::Remove(size_t idx)
	{
		if (idx >= m_max)
		{
			return NULL;
		}
		T* pPreVa = m_Items[idx];
		if (NULL == pPreVa)
		{
			return NULL;
		}
		onRemove(pPreVa);
		m_Items.erase(m_Items.begin() + idx);
		return pPreVa;
	}

	template <class T>
	T* Team::TSimpleContainer<T>::Remove(T* pVal)
	{
		if (NULL == pVal)
		{
			return pVal;
		}

		ITEM_VECTOR::iterator iter = std::find(m_Items.begin(), m_Items.end(), pVal);
		if (iter != m_Items.end())
		{
			T* pPreVa = *iter;
			onRemove(pPreVa);
			m_Items.erase(iter);
			return pPreVa;
		}

		return NULL;
	}

	template <class T>
	Team::TSimpleContainer<T>::TSimpleContainer() : m_max(0)
	{
	}

	template <class T>
	void TSimpleContainer<T>::setMaxLimit(size_t num)
	{
		m_max = num;
	}

	template <class T>
	T* Team::TSimpleContainer<T>::Add(T* pVal, /* 如果使用默认参数则追加, 否则插入 */size_t idx /*= AUTO_INCREASE_IDX*/)
	{
		if (!onAddPre(pVal))
		{
			return NULL;
		}

		if (NULL == pVal)
		{
			return Remove(idx);
		}

		if (idx != AUTO_INCREASE_IDX && idx >= m_max)
		{
			return NULL;
		}

		if (idx == AUTO_INCREASE_IDX || idx >= m_Items.size())
		{
			if (m_Items.size() >= m_max)
			{
				return NULL;
			}
			
			m_Items.push_back(pVal);
		}
		else
		{
			m_Items.insert(m_Items.begin() + idx, pVal);
		}

		onAdd(pVal);
		
		return pVal;
	}
}
#endif //__T_SIMPLECONTAINER_CPP__