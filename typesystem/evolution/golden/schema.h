// The `current.h` file is the one from `https://github.com/C5T/Current`.
// Compile with `-std=c++11` or higher.

#include "current.h"

// clang-format off

namespace current_userspace {

#ifndef CURRENT_SCHEMA_FOR_T9200560626419622480
#define CURRENT_SCHEMA_FOR_T9200560626419622480
namespace t9200560626419622480 {
CURRENT_STRUCT(SimpleStruct) {
  CURRENT_FIELD(x, int32_t);
  CURRENT_FIELD(y, int32_t);
  CURRENT_FIELD(z, std::string);
};
}  // namespace t9200560626419622480
#endif  // CURRENT_SCHEMA_FOR_T_9200560626419622480

#ifndef CURRENT_SCHEMA_FOR_T9200847454180217914
#define CURRENT_SCHEMA_FOR_T9200847454180217914
namespace t9200847454180217914 {
CURRENT_STRUCT(StructWithStruct) {
  CURRENT_FIELD(s, t9200560626419622480::SimpleStruct);
};
}  // namespace t9200847454180217914
#endif  // CURRENT_SCHEMA_FOR_T_9200847454180217914

#ifndef CURRENT_SCHEMA_FOR_T9010000003056575809
#define CURRENT_SCHEMA_FOR_T9010000003056575809
namespace t9010000003056575809 {
CURRENT_ENUM(EnumClassType, bool) {};
}  // namespace t9010000003056575809
#endif  // CURRENT_SCHEMA_FOR_T_9010000003056575809

#ifndef CURRENT_SCHEMA_FOR_T9201362263922935425
#define CURRENT_SCHEMA_FOR_T9201362263922935425
namespace t9201362263922935425 {
CURRENT_STRUCT(OtherTypes) {
  CURRENT_FIELD(enum_class, t9010000003056575809::EnumClassType);
  CURRENT_FIELD(optional, Optional<std::string>);
};
}  // namespace t9201362263922935425
#endif  // CURRENT_SCHEMA_FOR_T_9201362263922935425

#ifndef CURRENT_SCHEMA_FOR_T9222804237523418434
#define CURRENT_SCHEMA_FOR_T9222804237523418434
namespace t9222804237523418434 {
CURRENT_VARIANT(Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E, t9200560626419622480::SimpleStruct, t9200847454180217914::StructWithStruct, t9201362263922935425::OtherTypes);
}  // namespace t9222804237523418434
#endif  // CURRENT_SCHEMA_FOR_T_9222804237523418434

#ifndef CURRENT_SCHEMA_FOR_T9202784996115577694
#define CURRENT_SCHEMA_FOR_T9202784996115577694
namespace t9202784996115577694 {
CURRENT_STRUCT(StructWithVariant) {
  CURRENT_FIELD(v, t9222804237523418434::Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E);
};
}  // namespace t9202784996115577694
#endif  // CURRENT_SCHEMA_FOR_T_9202784996115577694

#ifndef CURRENT_SCHEMA_FOR_T9203533648527088493
#define CURRENT_SCHEMA_FOR_T9203533648527088493
namespace t9203533648527088493 {
CURRENT_STRUCT(Name) {
  CURRENT_FIELD(first, std::string);
  CURRENT_FIELD(last, std::string);
};
}  // namespace t9203533648527088493
#endif  // CURRENT_SCHEMA_FOR_T_9203533648527088493

#ifndef CURRENT_SCHEMA_FOR_T9204974803222981782
#define CURRENT_SCHEMA_FOR_T9204974803222981782
namespace t9204974803222981782 {
CURRENT_STRUCT(StructWithVectorOfNames) {
  CURRENT_FIELD(w, std::vector<t9203533648527088493::Name>);
};
}  // namespace t9204974803222981782
#endif  // CURRENT_SCHEMA_FOR_T_9204974803222981782

}  // namespace current_userspace

#ifndef CURRENT_NAMESPACE_USERSPACE_F055D51FBF78DB84_DEFINED
#define CURRENT_NAMESPACE_USERSPACE_F055D51FBF78DB84_DEFINED
CURRENT_NAMESPACE(USERSPACE_F055D51FBF78DB84) {
  CURRENT_NAMESPACE_TYPE(EnumClassType, current_userspace::t9010000003056575809::EnumClassType);
  CURRENT_NAMESPACE_TYPE(SimpleStruct, current_userspace::t9200560626419622480::SimpleStruct);
  CURRENT_NAMESPACE_TYPE(StructWithStruct, current_userspace::t9200847454180217914::StructWithStruct);
  CURRENT_NAMESPACE_TYPE(OtherTypes, current_userspace::t9201362263922935425::OtherTypes);
  CURRENT_NAMESPACE_TYPE(StructWithVariant, current_userspace::t9202784996115577694::StructWithVariant);
  CURRENT_NAMESPACE_TYPE(Name, current_userspace::t9203533648527088493::Name);
  CURRENT_NAMESPACE_TYPE(StructWithVectorOfNames, current_userspace::t9204974803222981782::StructWithVectorOfNames);
  CURRENT_NAMESPACE_TYPE(Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E, current_userspace::t9222804237523418434::Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E);
};  // CURRENT_NAMESPACE(USERSPACE_F055D51FBF78DB84)
#endif  // CURRENT_NAMESPACE_USERSPACE_F055D51FBF78DB84_DEFINED

namespace current {
namespace type_evolution {

// Default evolution for `CURRENT_ENUM(EnumClassType)`.
#ifndef DEFAULT_EVOLUTION_0EFC610EAA90005E980B02111A929865784B04C47419D8D3279378CAE96F393A  // USERSPACE_F055D51FBF78DB84::EnumClassType
#define DEFAULT_EVOLUTION_0EFC610EAA90005E980B02111A929865784B04C47419D8D3279378CAE96F393A  // USERSPACE_F055D51FBF78DB84::EnumClassType
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, USERSPACE_F055D51FBF78DB84::EnumClassType, CURRENT_ACTIVE_EVOLVER> {
  template <typename INTO>
  static void Go(USERSPACE_F055D51FBF78DB84::EnumClassType from,
                 typename INTO::EnumClassType& into) {
    into = static_cast<typename INTO::EnumClassType>(from);
  }
};
#endif

// Default evolution for struct `SimpleStruct`.
#ifndef DEFAULT_EVOLUTION_9850B4E5F628696CA9E60DF29450B2961F0A045117CFDF273A060075719AB636  // typename USERSPACE_F055D51FBF78DB84::SimpleStruct
#define DEFAULT_EVOLUTION_9850B4E5F628696CA9E60DF29450B2961F0A045117CFDF273A060075719AB636  // typename USERSPACE_F055D51FBF78DB84::SimpleStruct
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::SimpleStruct, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::SimpleStruct& from,
                 typename INTO::SimpleStruct& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::SimpleStruct>::value == 3,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(x);
      CURRENT_COPY_FIELD(y);
      CURRENT_COPY_FIELD(z);
  }
};
#endif

// Default evolution for struct `StructWithStruct`.
#ifndef DEFAULT_EVOLUTION_6E4B78BC03384BBFC04C0C083B1D4818552414583CEFA7969923A7AAF02AAD59  // typename USERSPACE_F055D51FBF78DB84::StructWithStruct
#define DEFAULT_EVOLUTION_6E4B78BC03384BBFC04C0C083B1D4818552414583CEFA7969923A7AAF02AAD59  // typename USERSPACE_F055D51FBF78DB84::StructWithStruct
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::StructWithStruct, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::StructWithStruct& from,
                 typename INTO::StructWithStruct& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::StructWithStruct>::value == 1,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(s);
  }
};
#endif

// Default evolution for struct `OtherTypes`.
#ifndef DEFAULT_EVOLUTION_0CD0FB4488BE4FCACFEF34083301F19BD5C8A23AC7DBD12045CB31BD9E925EA2  // typename USERSPACE_F055D51FBF78DB84::OtherTypes
#define DEFAULT_EVOLUTION_0CD0FB4488BE4FCACFEF34083301F19BD5C8A23AC7DBD12045CB31BD9E925EA2  // typename USERSPACE_F055D51FBF78DB84::OtherTypes
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::OtherTypes, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::OtherTypes& from,
                 typename INTO::OtherTypes& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::OtherTypes>::value == 2,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(enum_class);
      CURRENT_COPY_FIELD(optional);
  }
};
#endif

// Default evolution for struct `StructWithVariant`.
#ifndef DEFAULT_EVOLUTION_047F4B4BAEB8AA23AE9D63DA1BBDE3054F370E5202C02CAC751BDE1B4087A612  // typename USERSPACE_F055D51FBF78DB84::StructWithVariant
#define DEFAULT_EVOLUTION_047F4B4BAEB8AA23AE9D63DA1BBDE3054F370E5202C02CAC751BDE1B4087A612  // typename USERSPACE_F055D51FBF78DB84::StructWithVariant
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::StructWithVariant, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::StructWithVariant& from,
                 typename INTO::StructWithVariant& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::StructWithVariant>::value == 1,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(v);
  }
};
#endif

// Default evolution for struct `Name`.
#ifndef DEFAULT_EVOLUTION_AF9DF4C4F593A1A0A1E48E88D4D394E6CFEA4A1607BD341B7931182E44964756  // typename USERSPACE_F055D51FBF78DB84::Name
#define DEFAULT_EVOLUTION_AF9DF4C4F593A1A0A1E48E88D4D394E6CFEA4A1607BD341B7931182E44964756  // typename USERSPACE_F055D51FBF78DB84::Name
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::Name, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::Name& from,
                 typename INTO::Name& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Name>::value == 2,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(first);
      CURRENT_COPY_FIELD(last);
  }
};
#endif

// Default evolution for struct `StructWithVectorOfNames`.
#ifndef DEFAULT_EVOLUTION_3E57D01D4533C59AF50DCB73DDF7895BEFC1132BF11B3A29D4A86D97273BC22D  // typename USERSPACE_F055D51FBF78DB84::StructWithVectorOfNames
#define DEFAULT_EVOLUTION_3E57D01D4533C59AF50DCB73DDF7895BEFC1132BF11B3A29D4A86D97273BC22D  // typename USERSPACE_F055D51FBF78DB84::StructWithVectorOfNames
template <typename CURRENT_ACTIVE_EVOLVER>
struct Evolve<USERSPACE_F055D51FBF78DB84, typename USERSPACE_F055D51FBF78DB84::StructWithVectorOfNames, CURRENT_ACTIVE_EVOLVER> {
  using FROM = USERSPACE_F055D51FBF78DB84;
  template <typename INTO>
  static void Go(const typename FROM::StructWithVectorOfNames& from,
                 typename INTO::StructWithVectorOfNames& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::StructWithVectorOfNames>::value == 1,
                    "Custom evolver required.");
      CURRENT_COPY_FIELD(w);
  }
};
#endif

// Default evolution for `Variant<SimpleStruct, StructWithStruct, OtherTypes>`.
#ifndef DEFAULT_EVOLUTION_D21B58F3FF6C13FBCFB803488EAED9464A7525BBB2B7C56605C9AF3FD859413C  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_F055D51FBF78DB84::SimpleStruct, USERSPACE_F055D51FBF78DB84::StructWithStruct, USERSPACE_F055D51FBF78DB84::OtherTypes>>
#define DEFAULT_EVOLUTION_D21B58F3FF6C13FBCFB803488EAED9464A7525BBB2B7C56605C9AF3FD859413C  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_F055D51FBF78DB84::SimpleStruct, USERSPACE_F055D51FBF78DB84::StructWithStruct, USERSPACE_F055D51FBF78DB84::OtherTypes>>
template <typename DST, typename FROM_NAMESPACE, typename INTO, typename CURRENT_ACTIVE_EVOLVER>
struct USERSPACE_F055D51FBF78DB84_Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E_Cases {
  DST& into;
  explicit USERSPACE_F055D51FBF78DB84_Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E_Cases(DST& into) : into(into) {}
  void operator()(const typename FROM_NAMESPACE::SimpleStruct& value) const {
    using into_t = typename INTO::SimpleStruct;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::SimpleStruct, CURRENT_ACTIVE_EVOLVER>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::StructWithStruct& value) const {
    using into_t = typename INTO::StructWithStruct;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::StructWithStruct, CURRENT_ACTIVE_EVOLVER>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::OtherTypes& value) const {
    using into_t = typename INTO::OtherTypes;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::OtherTypes, CURRENT_ACTIVE_EVOLVER>::template Go<INTO>(value, Value<into_t>(into));
  }
};
template <typename CURRENT_ACTIVE_EVOLVER, typename VARIANT_NAME_HELPER>
struct Evolve<USERSPACE_F055D51FBF78DB84, ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_F055D51FBF78DB84::SimpleStruct, USERSPACE_F055D51FBF78DB84::StructWithStruct, USERSPACE_F055D51FBF78DB84::OtherTypes>>, CURRENT_ACTIVE_EVOLVER> {
  template <typename INTO,
            typename CUSTOM_INTO_VARIANT_TYPE>
  static void Go(const ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_F055D51FBF78DB84::SimpleStruct, USERSPACE_F055D51FBF78DB84::StructWithStruct, USERSPACE_F055D51FBF78DB84::OtherTypes>>& from,
                 CUSTOM_INTO_VARIANT_TYPE& into) {
    from.Call(USERSPACE_F055D51FBF78DB84_Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E_Cases<decltype(into), USERSPACE_F055D51FBF78DB84, INTO, CURRENT_ACTIVE_EVOLVER>(into));
  }
};
#endif

}  // namespace current::type_evolution
}  // namespace current

#if 0  // Boilerplate evolvers.

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, SimpleStruct, {
  CURRENT_COPY_FIELD(x);
  CURRENT_COPY_FIELD(y);
  CURRENT_COPY_FIELD(z);
});

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, StructWithStruct, {
  CURRENT_COPY_FIELD(s);
});

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, OtherTypes, {
  CURRENT_COPY_FIELD(enum_class);
  CURRENT_COPY_FIELD(optional);
});

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, StructWithVariant, {
  CURRENT_COPY_FIELD(v);
});

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, Name, {
  CURRENT_COPY_FIELD(first);
  CURRENT_COPY_FIELD(last);
});

CURRENT_STRUCT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, StructWithVectorOfNames, {
  CURRENT_COPY_FIELD(w);
});

CURRENT_VARIANT_EVOLVER(CustomEvolver, USERSPACE_F055D51FBF78DB84, t9222804237523418434::Variant_B_SimpleStruct_StructWithStruct_OtherTypes_E, CustomDestinationNamespace) {
  CURRENT_COPY_CASE(SimpleStruct);
  CURRENT_COPY_CASE(StructWithStruct);
  CURRENT_COPY_CASE(OtherTypes);
};

#endif  // Boilerplate evolvers.

// clang-format on
