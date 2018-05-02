#include "base/scoped_ptr.h"

#include "base/macros.h"
#include "gtest/gtest.h"

namespace base {
class TestClazzBase {
 public:
  virtual ~TestClazzBase() {}
  virtual void SetCounter(int* counter) = 0;
  virtual int DoSomething(int some_thing) const = 0;
};

class TestClazz : public TestClazzBase {
 public:
  TestClazz(): counter_(NULL) {}
  explicit TestClazz(int* counter) { SetCounter(counter); }
  ~TestClazz() { --*counter_; }

  virtual void SetCounter(int* counter) {
    counter_ = counter;
    ++*counter_;
  }

  virtual int DoSomething(int some_thing) const { return some_thing; }

 private:
  int* counter_;
};

void TestDrop(ScopedPtr<TestClazz> test_clazz) {}

ScopedPtr<TestClazz> TestPass(ScopedPtr<TestClazz> ptr) {
  return ptr;
}

ScopedPtr<TestClazz> TestReturn(int* counter) {
  return ScopedPtr<TestClazz>(new TestClazz(counter));
}



TEST(ScopedPtrTest, ScopedPtr) {
  int counter = 0;
  {
    ScopedPtr<TestClazz> ScopedPtr(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(ScopedPtr.Get());

    EXPECT_EQ(123, ScopedPtr->DoSomething(123));
    EXPECT_EQ(123, ScopedPtr.Get()->DoSomething(123));
    EXPECT_EQ(123, (*ScopedPtr).DoSomething(123));
  }
  EXPECT_EQ(0, counter);

  {
    ScopedPtr<TestClazz> ScopedPtr(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(ScopedPtr.Get());

    ScopedPtr.Reset(new TestClazz(&counter));
     EXPECT_EQ(1, counter);
     EXPECT_TRUE(ScopedPtr.Get());

    ScopedPtr.Reset();
     EXPECT_EQ(0, counter);
     EXPECT_FALSE(ScopedPtr.Get());
 

    ScopedPtr.Reset(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(ScopedPtr.Get());

    TestClazz* ptr = ScopedPtr.Release();
    EXPECT_EQ(1, counter);
    EXPECT_FALSE(ScopedPtr.Get());
    delete ptr;
    EXPECT_EQ(0, counter);

    ScopedPtr.Reset(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(ScopedPtr.Get());
  }
  EXPECT_EQ(0, counter);
}

TEST(ScopedPtrTest, DerivedClassTest) {
  int counter = 0;
  // Test construction from a ScopedPtr to a derived class.
  {
    ScopedPtr<TestClazz> scoped_ptr(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoped_ptr.Get());

    ScopedPtr<TestClazz> scoped_parent(std::move(scoped_ptr));
    EXPECT_EQ(1, counter);
    EXPECT_FALSE(scoped_ptr.Get());
    EXPECT_TRUE(scoped_parent.Get());

    EXPECT_EQ(123, scoped_parent->DoSomething(123));
    EXPECT_EQ(123, scoped_parent.Get()->DoSomething(123));
    EXPECT_EQ(123, (*scoped_parent).DoSomething(123));
  }
  EXPECT_EQ(0, counter);

  // Test assignment from a ScopedPtr to a derived class.
  {
    ScopedPtr<TestClazz> scoped_ptr(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoped_ptr.Get());

    ScopedPtr<TestClazz> scoped_parent;
    scoped_parent = std::move(scoped_ptr);
    EXPECT_EQ(1, counter);
    EXPECT_FALSE(scoped_ptr.Get());
    EXPECT_TRUE(scoped_parent.Get());

    EXPECT_EQ(123, scoped_parent->DoSomething(123));
    EXPECT_EQ(123, scoped_parent.Get()->DoSomething(123));
    EXPECT_EQ(123, (*scoped_parent).DoSomething(123));
  }
  EXPECT_EQ(0, counter);

// Test construction of a ScopedPtr with an additional const annotation.
  {
    ScopedPtr<TestClazz> scoper(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoper.Get());

    ScopedPtr<const TestClazz> scoped_const(std::move(scoper));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoped_const.Get());
    EXPECT_FALSE(scoper.Get());

    EXPECT_EQ(123, scoped_const->DoSomething(123));
    EXPECT_EQ(123, scoped_const.Get()->DoSomething(123));
    EXPECT_EQ(123, (*scoped_const).DoSomething(123));
  }
  EXPECT_EQ(0, counter);

  // Test assignment to a ScopedPtr with an additional const annotation.
  {
    ScopedPtr<TestClazz> scoper(new TestClazz(&counter));
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoper.Get());

    ScopedPtr<const TestClazz> scoped_const;
    scoped_const = std::move(scoper);
    EXPECT_EQ(1, counter);
    EXPECT_TRUE(scoped_const.Get());
    EXPECT_FALSE(scoper.Get());

    EXPECT_EQ(123, scoped_const->DoSomething(123));
    EXPECT_EQ(123, scoped_const.Get()->DoSomething(123));
    EXPECT_EQ(123, (*scoped_const).DoSomething(123));
  }
  EXPECT_EQ(0, counter);
}

TEST(ScopedPtrTest, ScopedPtrWithArray) {
  static const int kNum = 12;

  int counter = 0;

  {
    ScopedPtr<TestClazz[]> scoper(new TestClazz[kNum]);
    EXPECT_TRUE(scoper.Get());
    EXPECT_EQ(&scoper[0], scoper.Get());
    for (int i = 0; i < kNum; ++i) {
      scoper[i].SetCounter(&counter);
    }
    EXPECT_EQ(12, counter);

    EXPECT_EQ(10, scoper.Get()->DoSomething(10));
    EXPECT_EQ(10, scoper[2].DoSomething(10));
  }
  EXPECT_EQ(0, counter);

  // Test reset() and release()
  {
    ScopedPtr<TestClazz[]> scoper;
    EXPECT_FALSE(scoper.Get());
    EXPECT_FALSE(scoper.Release());
    EXPECT_FALSE(scoper.Get());
    scoper.Reset();
    EXPECT_FALSE(scoper.Get());

    scoper.Reset(new TestClazz[kNum]);
    for (int i = 0; i < kNum; ++i) {
      scoper[i].SetCounter(&counter);
    }
    EXPECT_EQ(12, counter);
    scoper.Reset();
    EXPECT_EQ(0, counter);

    scoper.Reset(new TestClazz[kNum]);
    for (int i = 0; i < kNum; ++i) {
      scoper[i].SetCounter(&counter);
    }
    EXPECT_EQ(12, counter);
    TestClazz* ptr = scoper.Release();
    EXPECT_EQ(12, counter);
    delete[] ptr;
    EXPECT_EQ(0, counter);
  }
  EXPECT_EQ(0, counter);

  {
    TestClazz* loggers = new TestClazz[kNum];
    ScopedPtr<TestClazz[]> scoper(loggers);
    EXPECT_TRUE(scoper.Get());
    for (int i = 0; i < kNum; ++i) {
      scoper[i].SetCounter(&counter);
    }
    EXPECT_EQ(kNum, counter);

    // Test moving with constructor;
    ScopedPtr<TestClazz[]> scoper2(std::move(scoper));
    EXPECT_EQ(kNum, counter);

    // Test moving with assignment;
    ScopedPtr<TestClazz[]> scoper3;
    scoper3 = std::move(scoper2);
    EXPECT_EQ(kNum, counter);
    EXPECT_FALSE(scoper.Get());
    EXPECT_FALSE(scoper2.Get());
    EXPECT_TRUE(scoper3.Get());
  }
  EXPECT_EQ(0, counter);
}

TEST(ScopedPtrTest, MoveBehavior) {
  int counter = 0;
  {
    TestClazz* test_clazz = new TestClazz(&counter);
    ScopedPtr<TestClazz> scoped_ptr(test_clazz);
    EXPECT_EQ(1, counter);

    // moving with constructor
    ScopedPtr<TestClazz> scoped_ptr2(std::move(scoped_ptr));
    EXPECT_EQ(1, counter);

    //move with assign
    ScopedPtr<TestClazz> scoped_ptr3;
    scoped_ptr3 = std::move(scoped_ptr2);
    EXPECT_EQ(1, counter);
    EXPECT_FALSE(scoped_ptr.Get());
    EXPECT_FALSE(scoped_ptr2.Get());
    EXPECT_TRUE(scoped_ptr3.Get());
  }

  // Test that passing to function which does nothing does not leak.
  {
    TestClazz* test_clazz = new TestClazz(&counter);
    ScopedPtr<TestClazz> ScopedPtr(test_clazz);
    EXPECT_EQ(1, counter);

    // Should auto-destruct logger by end of scope.
    TestDrop(std::move(ScopedPtr));
    EXPECT_FALSE(ScopedPtr.Get());
  }
  
  {
    TestClazz* test_clazz = new TestClazz(&counter);
    ScopedPtr<TestClazz> scoped_ptr(test_clazz);
    EXPECT_EQ(1, counter);

    // moving with constructor
    int counter2 = 0;
    ScopedPtr<TestClazz> scoped_ptr2(new TestClazz(&counter2));
    EXPECT_EQ(1, counter2);

    scoped_ptr = scoped_ptr2;

    EXPECT_EQ(0, counter);
    EXPECT_EQ(1, counter2);

  }
  EXPECT_EQ(0, counter);
}

TEST(ScopedPtrTest, ReturnBehavior) {
 int counter = 0;
  {
    TestClazz* test_clazz = new TestClazz(&counter);
    ScopedPtr<TestClazz> scoped_ptr(test_clazz);
    EXPECT_EQ(1, counter);

    ScopedPtr<TestClazz> ptr = TestPass(std::move(scoped_ptr));
    EXPECT_FALSE(scoped_ptr.Get());
    EXPECT_TRUE(ptr.Get());
    EXPECT_EQ(1, counter);
  }
  EXPECT_EQ(0, counter);
  {
    TestReturn(&counter);
  }
  EXPECT_EQ(0, counter);
}

ScopedPtr<int> NullIntReturn() {
  return ScopedPtr<int>(NULL);
}

TEST(ScopedPtrTest, Nullptr) {
  ScopedPtr<int> scoper1(NULL);
  ScopedPtr<int> scoper2(new int);
  scoper2.Reset(NULL);
  ScopedPtr<int> scoper3(NullIntReturn());
  ScopedPtr<int> scoper4 = NullIntReturn();
  EXPECT_EQ(NULL, scoper1.Get());
  EXPECT_EQ(NULL, scoper2.Get());
  EXPECT_EQ(NULL, scoper3.Get());
  EXPECT_EQ(NULL, scoper4.Get());
}

ScopedPtr<int[]> NullIntArrayReturn() {
  return ScopedPtr<int[]>(NULL);
}

TEST(ScopedPtrTest, NullptrArray) {
  ScopedPtr<int[]> scoper1(NULL);
  ScopedPtr<int[]> scoper2(new int[3]);
  scoper2.Reset(NULL);
  ScopedPtr<int[]> scoper3(NullIntArrayReturn());
  ScopedPtr<int[]> scoper4 = NullIntArrayReturn();
  EXPECT_EQ(NULL, scoper1.Get());
  EXPECT_EQ(NULL, scoper2.Get());
  EXPECT_EQ(NULL, scoper3.Get());
  EXPECT_EQ(NULL, scoper4.Get());
}


TEST(ScopedPtrTest, ReferenceCycle) {
  struct StructB;
  struct StructA {
    ScopedPtr<StructB> b;
  };

  struct StructB {
    ScopedPtr<StructA> a;
  };

  // Create a reference cycle.
  StructA* a = new StructA;
  a->b.Reset(new StructB);
  a->b->a.Reset(a);

  a->b.Reset();
}

}  // namespace base
