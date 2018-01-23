#include "base/weak_ptr.h"

#include "gtest/gtest.h"

namespace base {

class BaseTest {
 public:
  BaseTest() : weak_ptr_(this) {}
  ~BaseTest() { weak_ptr_.Invalidate(); }
  WeakPtr<BaseTest>& weak_ptr(){
    return weak_ptr_;
  }
 private:
  WeakPtr<BaseTest> weak_ptr_;
};

struct Base {
  std::string member;
};
struct Derived : public Base {};

TEST(WeakPtrTest, Basic) {
  int data;
  WeakPtr<int> ptr = WeakPtr<int>(&data);
  EXPECT_EQ(&data, ptr.Get());
}

TEST(WeakPtrTest, Comparison) {
  int data;
  WeakPtr<int> ptr = WeakPtr<int>(&data);
  WeakPtr<int> ptr2 = ptr;
  EXPECT_EQ(ptr.Get(), ptr2.Get());
}

TEST(WeakPtrTest, Multiple) {
  WeakPtr<BaseTest> a, b;
  {
    BaseTest data;
    a = data.weak_ptr();
    b = data.weak_ptr();
    EXPECT_EQ(&data, a.Get());
    EXPECT_EQ(&data, b.Get());
  }
  EXPECT_EQ(NULL, a.Get());
  EXPECT_EQ(NULL, b.Get());
}

TEST(WeakPtrTest, MultipleStaged) {
  WeakPtr<BaseTest> a;
  {
    BaseTest data;
    a = data.weak_ptr();
    { WeakPtr<BaseTest> b = data.weak_ptr(); }
    EXPECT_TRUE(NULL != a.Get());
  }
  EXPECT_EQ(NULL, a.Get());
}

TEST(WeakPtrTest, Dereference) {
  Base data;
  data.member = "123456";
  WeakPtr<Base> ptr = WeakPtr<Base>(&data);
  EXPECT_EQ(&data, ptr.Get());
  EXPECT_EQ(data.member, (*ptr).member);
  EXPECT_EQ(data.member, ptr->member);
}

TEST(WeakPtrTest, UpCast) {
  Derived data;
  WeakPtr<Derived> p = WeakPtr<Derived>(&data);
  WeakPtr<Base> ptr;
  ptr = p;
  EXPECT_EQ(ptr.Get(), &data);
}

TEST(WeakPtrTest, InvalidateWeakPtrs) {
  int data;
  WeakPtr<int> ptr = WeakPtr<int>(&data);
  EXPECT_EQ(&data, ptr.Get());
  EXPECT_TRUE(ptr.IsValid());
  ptr.Invalidate();
  EXPECT_EQ(NULL, ptr.Get());
  EXPECT_FALSE(ptr.IsValid());
}

}  // namespace base