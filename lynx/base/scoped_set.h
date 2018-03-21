// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_SCOPED_SET_H_
#define LYNX_BASE_SCOPED_SET_H_

#include <unordered_set>
#include "base/debug/memory_debug.h"

namespace base {
	template<class T>
	class ScopedSet {
	public:
		typedef typename std::unordered_set<T*>::iterator iterator;

		ScopedSet() {}
		ScopedSet(ScopedSet&& other) { set_.swap(other.set_); }

		~ScopedSet() { clear(); }

		ScopedSet& operator= (ScopedSet&& other) {
			set_.swap(other.set_);
			return *this;
		}

		void clear() {
			iterator iter = set_.begin();
			for (iter; iter != set_.end(); ++iter) {
				lynx_delete(*iter);
			}
			set_.clear();
		}

		void release() {
			set_.clear();
		}

		iterator begin() {return set_.begin();}

		iterator end() {return set_.end();}

		int size() { return set_.size(); }

		bool empty() { return set_.empty(); }

		iterator find(T* ptr) { return set_.find(ptr); }

		std::pair<iterator, bool> add(T* ptr) {
			std::pair<iterator, bool> result = set_.insert(ptr);
			return result;
		}

		void erase(iterator iter) {
			lynx_delete(iter->second);
			set_.erase(iter);
		}

		void erase(T* t) {
            iterator it = find(t);
            if (it != set_.end()) {
                lynx_delete(t);
                set_.erase(it);
            }
		}

		void release(T* t) {
            iterator it = find(t);
            if (it != set_.end()) {
                set_.erase(it);
            }
		}

	private:
		std::unordered_set<T*> set_;
	};
}
#endif // LYNX_BASE_SCOPED_SET_H_
