/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>
          (c) 2015 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "enum.h"
#include "struct.h"
#include "optional.h"
#include "polymorphic.h"

#include "../3rdparty/gtest/gtest-main.h"

#include "Reflection/test.cc"
#include "Serialization/test.cc"

namespace struct_definition_test {

// A few properly defined Current data types.
CURRENT_STRUCT(Foo) {
  CURRENT_FIELD(i, uint64_t, 0u);
  CURRENT_CONSTRUCTOR(Foo)(uint64_t i = 42u) : i(i) {}
};
CURRENT_STRUCT(Bar) {
  CURRENT_FIELD(v1, std::vector<uint64_t>);
  CURRENT_FIELD(v2, std::vector<Foo>);
  CURRENT_FIELD(v3, std::vector<std::vector<Foo>>);
  CURRENT_FIELD(v4, (std::map<std::string, std::string>));
  CURRENT_FIELD(v5, (std::map<Foo, int>));
};
CURRENT_STRUCT(DerivedFromFoo, Foo) { CURRENT_FIELD(bar, Bar); };

static_assert(IS_VALID_CURRENT_STRUCT(Foo), "Struct `Foo` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(Bar), "Struct `Bar` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(DerivedFromFoo), "Struct `DerivedFromFoo` was not properly declared.");

}  // namespace struct_definition_test

// Confirm that Current data types defined in a namespace are accessible from outside it.
static_assert(IS_VALID_CURRENT_STRUCT(struct_definition_test::Foo), "Struct `Foo` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(struct_definition_test::Bar), "Struct `Bar` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(struct_definition_test::DerivedFromFoo),
              "Struct `DerivedFromFoo` was not properly declared.");

namespace some_other_namespace {

// Confirm that Current data types defined in one namespace are accessible from another one.
static_assert(IS_VALID_CURRENT_STRUCT(::struct_definition_test::Foo),
              "Struct `Foo` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(::struct_definition_test::Bar),
              "Struct `Bar` was not properly declared.");
static_assert(IS_VALID_CURRENT_STRUCT(::struct_definition_test::DerivedFromFoo),
              "Struct `DerivedFromFoo` was not properly declared.");

}  // namespace some_other_namespace

namespace struct_definition_test {

// Properly declared structures.
CURRENT_STRUCT(Empty){};
CURRENT_STRUCT(EmptyDerived, Empty){};

static_assert(IS_VALID_CURRENT_STRUCT(Empty), "`Empty` must pass `IS_VALID_CURRENT_STRUCT` check.");
static_assert(IS_VALID_CURRENT_STRUCT(EmptyDerived),
              "`EmptyDerived` must pass `IS_VALID_CURRENT_STRUCT` check.");

// Improperly declared structures.
struct WrongStructNotCurrentStruct {
  int x;
};
struct WrongDerivedStructNotCurrentStruct : ::current::CurrentSuper {};
struct NotCurrentStructDerivedFromCurrentStruct : Empty {};

CURRENT_STRUCT(WrongUsesCOUNTERInternally) {
  CURRENT_FIELD(i1, uint64_t);
  static size_t GetCounter() { return __COUNTER__; }
  CURRENT_FIELD(i2, uint64_t);
};

// The lines below don't compile with various errors.
// static_assert(!IS_VALID_CURRENT_STRUCT(WrongStructNotCurrentStruct),
//               "`WrongStructNotCurrentStruct` must NOT pass `IS_VALID_CURRENT_STRUCT` check.");
// static_assert(!IS_VALID_CURRENT_STRUCT(WrongDerivedStructNotCurrentStruct),
//               "`WrongDerivedStructNotCurrentStruct` must NOT pass `IS_VALID_CURRENT_STRUCT` check.");
// static_assert(!IS_VALID_CURRENT_STRUCT(NotCurrentStructDerivedFromCurrentStruct),
//               "`NotCurrentStructDerivedFromCurrentStruct` must NOT pass `IS_VALID_CURRENT_STRUCT` check.");
// static_assert(!IS_VALID_CURRENT_STRUCT(WrongUsesCOUNTERInternally),
//               "`WrongUsesCOUNTERInternally` must not pass `IS_VALID_CURRENT_STRUCT` check.");

}  // namespace struct_definition_test

TEST(TypeSystemTest, ExistsAndValueSemantics) {
  {
    int x = 1;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(1, Value(x));
    EXPECT_EQ(1, Value(Value(x)));
  }
  {
    const int x = 2;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(2, Value(x));
    EXPECT_EQ(2, Value(Value(x)));
  }
  {
    int y = 3;
    int& x = y;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(3, Value(x));
    EXPECT_EQ(3, Value(Value(x)));
    y = 4;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(4, Value(x));
    EXPECT_EQ(4, Value(Value(x)));
  }
  {
    int y = 5;
    const int& x = y;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(5, Value(x));
    EXPECT_EQ(5, Value(Value(x)));
    y = 6;
    ASSERT_TRUE(Exists(x));
    EXPECT_EQ(6, Value(x));
    EXPECT_EQ(6, Value(Value(x)));
  }
}

TEST(TypeSystemTest, CopyDoesItsJob) {
  using namespace struct_definition_test;

  Foo a;
  Foo b;

  a.i = 1u;
  b.i = 2u;
  EXPECT_EQ(1u, a.i);
  EXPECT_EQ(2u, b.i);

  a.i = 3u;
  EXPECT_EQ(3u, a.i);
  EXPECT_EQ(2u, b.i);

  b = a;
  EXPECT_EQ(3u, a.i);
  EXPECT_EQ(3u, b.i);
}

TEST(TypeSystemTest, ImmutableOptional) {
  {
    ImmutableOptional<int> foo(make_unique<int>(100));
    ASSERT_TRUE(Exists(foo));
    EXPECT_EQ(100, foo.Value());
    EXPECT_EQ(100, static_cast<int>(Value(foo)));
    EXPECT_EQ(100, static_cast<int>(Value(Value(foo))));
  }
  {
    ImmutableOptional<int> bar(nullptr);
    ASSERT_FALSE(Exists(bar));
    try {
      Value(bar);
      ASSERT_TRUE(false);
    } catch (NoValue) {
    }
  }
  {
    ImmutableOptional<int> meh(nullptr);
    ASSERT_FALSE(Exists(meh));
    try {
      Value(meh);
      ASSERT_TRUE(false);
    } catch (NoValueOfType<int>) {
    }
  }
  {
    struct_definition_test::Foo bare;
    ASSERT_TRUE(Exists(bare));
    EXPECT_EQ(42u, Value(bare).i);
    EXPECT_EQ(42u, Value(Value(bare)).i);
  }
  {
    struct_definition_test::Foo bare;
    ImmutableOptional<struct_definition_test::Foo> wrapped(&bare);
    ASSERT_TRUE(Exists(wrapped));
    EXPECT_EQ(42u, wrapped.Value().i);
    EXPECT_EQ(42u, Value(wrapped).i);
    EXPECT_EQ(42u, Value(Value(wrapped)).i);
  }
  {
    struct_definition_test::Foo bare;
    ImmutableOptional<struct_definition_test::Foo> wrapped(&bare);
    const ImmutableOptional<struct_definition_test::Foo>& double_wrapped(wrapped);
    ASSERT_TRUE(Exists(double_wrapped));
    EXPECT_EQ(42u, double_wrapped.Value().i);
    EXPECT_EQ(42u, Value(double_wrapped).i);
    EXPECT_EQ(42u, Value(Value(double_wrapped)).i);
  }
  {
    const auto lambda =
        [](int x) -> ImmutableOptional<int> { return ImmutableOptional<int>(make_unique<int>(x)); };
    ASSERT_TRUE(Exists(lambda(101)));
    EXPECT_EQ(102, lambda(102).Value());
    EXPECT_EQ(102, Value(lambda(102)));
  }
}

TEST(TypeSystemTest, Optional) {
  Optional<int> foo(make_unique<int>(200));
  ASSERT_TRUE(Exists(foo));
  EXPECT_EQ(200, foo.Value());
  EXPECT_EQ(200, Value(foo));
  foo = nullptr;
  ASSERT_FALSE(Exists(foo));
  try {
    Value(foo);
    ASSERT_TRUE(false);
  } catch (NoValue) {
  }
}

namespace enum_class_test {
CURRENT_ENUM(Fruits, uint32_t){APPLE = 1u, ORANGE = 2u};
}  // namespace enum_class_test

TEST(TypeSystemTest, EnumRegistration) {
  using current::reflection::EnumName;
  EXPECT_EQ("Fruits", EnumName<enum_class_test::Fruits>());
}

TEST(TypeSystemTest, PolymorphicStaticAsserts) {
  using namespace struct_definition_test;

  static_assert(std::is_same<Polymorphic<Foo>, Polymorphic<Foo>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo>, Polymorphic<TypeList<Foo>>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo>, Polymorphic<TypeList<Foo, Foo>>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo>, Polymorphic<TypeListImpl<Foo>>>::value, "");
  static_assert(Polymorphic<Foo>::T_TYPELIST_SIZE == 1u, "");
  static_assert(std::is_same<Polymorphic<Foo>::T_TYPELIST, TypeListImpl<Foo>>::value, "");

  static_assert(std::is_same<Polymorphic<Foo, Empty>, Polymorphic<Foo, Empty>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo, Empty>, Polymorphic<TypeList<Foo, Empty>>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo, Empty>, Polymorphic<TypeList<Foo, Empty, Foo>>>::value, "");
  static_assert(
      std::is_same<Polymorphic<Foo, Empty>, Polymorphic<TypeList<Foo, Empty, TypeList<Empty>>>>::value, "");
  static_assert(std::is_same<Polymorphic<Foo, Empty>, Polymorphic<TypeListImpl<Foo, Empty>>>::value, "");
  static_assert(Polymorphic<Foo, Empty>::T_TYPELIST_SIZE == 2u, "");
  static_assert(std::is_same<Polymorphic<Foo, Empty>::T_TYPELIST, TypeListImpl<Foo, Empty>>::value, "");
}

TEST(TypeSystemTest, PolymorphicSmokeTestOneType) {
  using namespace struct_definition_test;

  {
    Polymorphic<Foo> p(make_unique<Foo>());
    const Polymorphic<Foo>& cp(p);

    {
      ASSERT_TRUE(p.Has<Foo>());
      const auto& foo = p.Value<Foo>();
      EXPECT_EQ(42u, foo.i);
    }
    {
      ASSERT_TRUE(cp.Has<Foo>());
      const auto& foo = cp.Value<Foo>();
      EXPECT_EQ(42u, foo.i);
    }

    ++p.Value<Foo>().i;

    EXPECT_EQ(43u, p.Value<Foo>().i);
    EXPECT_EQ(43u, cp.Value<Foo>().i);

    p = Foo(100u);
    EXPECT_EQ(100u, p.Value<Foo>().i);
    EXPECT_EQ(100u, cp.Value<Foo>().i);

    p = static_cast<const Foo&>(Foo(101u));
    EXPECT_EQ(101u, p.Value<Foo>().i);
    EXPECT_EQ(101u, cp.Value<Foo>().i);

    p = std::move(Foo(102u));
    EXPECT_EQ(102u, p.Value<Foo>().i);
    EXPECT_EQ(102u, cp.Value<Foo>().i);

    p = make_unique<Foo>(103u);
    EXPECT_EQ(103u, p.Value<Foo>().i);
    EXPECT_EQ(103u, cp.Value<Foo>().i);

    // TODO(dkorolev): Unsafe? Remove?
    p = new Foo(104u);
    EXPECT_EQ(104u, p.Value<Foo>().i);
    EXPECT_EQ(104u, cp.Value<Foo>().i);
  }

  {
    struct Visitor {
      std::string s;
      void operator()(const Foo& foo) { s += "Foo " + bricks::strings::ToString(foo.i) + '\n'; }
    };
    Visitor v;
    {
      Polymorphic<Foo> p(Foo(501u));
      p.Match(v);
      EXPECT_EQ("Foo 501\n", v.s);
    }
    {
      const Polymorphic<Foo> p(Foo(502u));
      p.Match(v);
      EXPECT_EQ("Foo 501\nFoo 502\n", v.s);
    }
  }

  {
    std::string s;
    const auto lambda = [&s](const Foo& foo) { s += "lambda: Foo " + bricks::strings::ToString(foo.i) + '\n'; };
    {
      Polymorphic<Foo> p(Foo(601u));
      p.Match(lambda);
      EXPECT_EQ("lambda: Foo 601\n", s);
    }
    {
      const Polymorphic<Foo> p(Foo(602u));
      p.Match(lambda);
      EXPECT_EQ("lambda: Foo 601\nlambda: Foo 602\n", s);
    }
  }

  {
    const Polymorphic<Foo> p((Foo()));
    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValue) {
    }
    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValueOfType<Empty>) {
    }
  }
}

TEST(TypeSystemTest, PolymorphicSmokeTestMultipleTypes) {
  using namespace struct_definition_test;

  struct Visitor {
    std::string s;
    void operator()(const Empty&) { s = "Empty"; }
    void operator()(const Foo& foo) { s = "Foo " + bricks::strings::ToString(foo.i); }
    void operator()(const DerivedFromFoo& object) {
      s = "DerivedFromFoo [" + bricks::strings::ToString(object.bar.v1.size()) + "]";
    }
  };
  Visitor v;

  {
    Polymorphic<Empty, Foo, DerivedFromFoo> p((Empty()));
    const Polymorphic<Empty, Foo, DerivedFromFoo>& cp = p;

    p.Match(v);
    EXPECT_EQ("Empty", v.s);
    cp.Match(v);
    EXPECT_EQ("Empty", v.s);

    p = Foo(1u);

    p.Match(v);
    EXPECT_EQ("Foo 1", v.s);
    cp.Match(v);
    EXPECT_EQ("Foo 1", v.s);

    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValue) {
    }
    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValueOfType<Empty>) {
    }

    p = make_unique<DerivedFromFoo>();
    p.Match(v);
    EXPECT_EQ("DerivedFromFoo [0]", v.s);
    cp.Match(v);
    EXPECT_EQ("DerivedFromFoo [0]", v.s);

    p.Value<DerivedFromFoo>().bar.v1.resize(3);
    p.Match(v);
    EXPECT_EQ("DerivedFromFoo [3]", v.s);
    cp.Match(v);
    EXPECT_EQ("DerivedFromFoo [3]", v.s);

    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValue) {
    }
    try {
      p.Value<Empty>();
      ASSERT_TRUE(false);
    } catch (NoValueOfType<Empty>) {
    }
  }
}
