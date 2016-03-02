#pragma once
#include <vector>

#define AUTO_INCREASE_IDX  size_t(-1)
namespace Team
{
	
	/**
	 * 固定个数, 删除的清空的置为空
	 */
	template <class T>
	class TSimpleContainer
	{
	public:
		TSimpleContainer(); 

		typedef typename std::vector<T*> ITEM_VECTOR;
	private:

		ITEM_VECTOR m_Items; 
	public:
		/* 如果使用默认参数则追加, 否则在满足数量限制的情况插入 */
		virtual T* Add(T* pVal, size_t idx = AUTO_INCREASE_IDX);
	
		virtual T* Remove(T* pVal);
	
		virtual T* Remove(size_t idx);

		virtual void UnSafeRemove(T* pVal);//仅仅清空

		void UnSafeClear();//仅仅清空

		bool Find(const T* pVal) const;

		size_t IndexOf(const T* pVal) const;

		size_t Count() const;
		size_t FreeCount() const;
	
		T* Get(size_t idx) const;
	
		typename ITEM_VECTOR::iterator begin();
	
		typename ITEM_VECTOR::iterator end();

		typename ITEM_VECTOR::const_iterator cbegin() const;

		typename ITEM_VECTOR::const_iterator cend()  const;
		
		template <typename _FUNC>
		void foreach(_FUNC FUNC) const;
	
		template <typename _FUNC, typename T1>
		bool foreach1(_FUNC FUNC, T1 t1) const;
	
		template <typename _FUNC, typename T1>
		bool foreach1r(_FUNC FUNC, T1& t1) const;
	
		template <typename _FUNC, typename T1, typename T2>
		void foreach2(_FUNC FUNC, T1 t1, T2 t2) const;
	
		template <typename _FUNC, typename T1, typename T2>
		void foreach2r(_FUNC FUNC, T1& t1, T2& t2) const;

		template <typename _FUNC, typename T1, typename T2>
		void foreach2rend(_FUNC FUNC, T1 t1, T2& t2) const;


		template <typename _FUNC, typename T1, typename T2, typename T3>
		void foreach3(_FUNC FUNC, T1 t1, T2 t2, T3 t3) const;

		template <typename _FUNC, typename T1, typename T2, typename T3>
		void foreach3rend(_FUNC FUNC, T1 t1, T2 t2, T3& t3) const;

		template <typename _FUNC, typename T1, typename T2, typename T3, typename T4>
		void foreach4(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4 t4) const;

		template <typename _FUNC, typename T1, typename T2, typename T3, typename T4>
		void foreach4rend(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4& t4) const;

		template <typename _FUNC, typename T1, typename T2, typename T3, typename T4, typename T5>
		void foreach5rend(_FUNC FUNC, T1 t1, T2 t2, T3 t3, T4 t4, T5& t5) const;

		template <typename _FUNC, typename T1>
		size_t foreach_ra(_FUNC FUNC, T1& t1, size_t max) const;

		template <typename _FUNC, typename T1>
		void each_between1rend(_FUNC FUNC, T1& t1) const;
	
		inline size_t getMaxCount() const;

		void setMaxLimit(size_t num);
	protected:
		//删除之前执行 返回false不执行Add
		virtual bool onAddPre(T* pVal) { return true; }

		virtual T* onAdd(T* pVal) { return pVal; }
		virtual T* onRemove(T* pVal) { return pVal; }

		size_t m_max;
	};

}

#include "TSimpleContainer.inl"