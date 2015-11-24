/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>

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

#ifndef CURRENT_TYPE_SYSTEM_REFLECTION_SCHEMA_H
#define CURRENT_TYPE_SYSTEM_REFLECTION_SCHEMA_H

#include "reflection.h"

#include "../../Bricks/template/enable_if.h"
#include "../../Bricks/strings/strings.h"

namespace bricks {
namespace strings {

template <>
struct ToStringImpl<::current::reflection::TypeID> {
  static std::string ToString(::current::reflection::TypeID type_id) {
    return ToStringImpl<uint64_t>::ToString(static_cast<uint64_t>(type_id));
  }
};

}  // namespace strings
}  // namespace bricks

namespace current {
namespace reflection {

CURRENT_STRUCT(TypeInfo) {
  CURRENT_FIELD(type_id, TypeID, TypeID::INVALID_TYPE);
  // Used only for storing the name of `enum` types.
  CURRENT_FIELD(enum_name, std::string);
  // Ascending index order in vector corresponds to left-to-right order in the definition of the type,
  // i.e. for `map<int32_t, string>`:
  //   included_types[0] = TypeID::Int32
  //   included_types[1] = TypeID::String
  CURRENT_FIELD(included_types, std::vector<TypeID>);
};

CURRENT_STRUCT(StructInfo) {
  CURRENT_FIELD(type_id, TypeID, TypeID::INVALID_TYPE);
  CURRENT_FIELD(name, std::string, "");
  CURRENT_FIELD(super_type_id, TypeID, TypeID::INVALID_TYPE);
  CURRENT_FIELD(fields, (std::vector<std::pair<TypeID, std::string>>));

  CURRENT_DEFAULT_CONSTRUCTOR(StructInfo) {}
  CURRENT_CONSTRUCTOR(StructInfo)(const std::shared_ptr<ReflectedTypeImpl> r) {
    const ReflectedType_Struct* reflected_struct = dynamic_cast<const ReflectedType_Struct*>(r.get());
    if (reflected_struct != nullptr) {
      type_id = reflected_struct->type_id;
      name = reflected_struct->name;
      const ReflectedType_Struct* super =
          dynamic_cast<const ReflectedType_Struct*>(reflected_struct->reflected_super.get());
      if (super != nullptr) {
        super_type_id = super->type_id;
      }
      for (const auto& f : reflected_struct->fields) {
        fields.emplace_back(f.first->type_id, f.second);
      }
    }
  }
};

CURRENT_STRUCT(SchemaInfo) {
  CURRENT_FIELD(structs, (std::map<TypeID, StructInfo>));
  CURRENT_FIELD(types, (std::map<TypeID, TypeInfo>));
  // List of the struct type_id's contained in schema.
  // Ascending index order corresponds to the order required for proper declaring of all the structs.
  CURRENT_FIELD(ordered_struct_list, std::vector<TypeID>);
};

// Metaprogramming to make it easy to add support for new programming languages to include in the schema.
// TODO(dkorolev): Some of the stuff below could be compile-time, it's just that struct specializations
// are not allowed within other structs, while overloading does its job.
struct Language {
  struct CPP {
    static std::string Header() {
      return "// g++ -c -std=c++11 current.cc\n"
             "\n"
             "#include \"current.h\"  // :TROLLFACE:\n"
             "\n"
             "namespace current_userspace {\n";
    }
    static std::string Footer() { return "}  // namespace current_userspace\n"; }
    static std::string ErrorMessageWithTypeId(const TypeID type_id) {
      return "#error \"Unknown struct with `type_id` = " + bricks::strings::ToString(type_id) + "\"\n";
    }
  };
  struct FSharp {
    static std::string Header() {
      return "// fsharpi -r Newtonsoft.Json.dll current.fsx\n"
             "\n"
             "open Newtonsoft.Json\n"
             "let inline JSON o = JsonConvert.SerializeObject(o)\n"
             "let inline ParseJSON (s : string) : 'T = JsonConvert.DeserializeObject<'T>(s)\n"
             "\n";
    }
    static std::string Footer() { return ""; }
    static std::string ErrorMessageWithTypeId(const TypeID type_id) {
      // TODO(dkorolev): Probably somewhat different syntax.
      return "#error \"Unknown struct with `type_id` = " + bricks::strings::ToString(type_id) + "\"\n";
    }
  };
};

struct PrimitiveTypesList {
  std::map<TypeID, std::string> cpp_name;
  std::map<TypeID, std::string> fsharp_name;
  PrimitiveTypesList() {
#define CURRENT_DECLARE_PRIMITIVE_TYPE(typeid_index, cpp_type, unused_current_type, fsharp_type) \
  cpp_name[static_cast<TypeID>(TYPEID_BASIC_TYPE + typeid_index)] = #cpp_type;                   \
  fsharp_name[static_cast<TypeID>(TYPEID_BASIC_TYPE + typeid_index)] = fsharp_type;
#include "../primitive_types.dsl.h"
#undef CURRENT_DECLARE_PRIMITIVE_TYPE
  }
  std::string PrimitiveTypeName(const Language::CPP&, const TypeID type_id) const {
    if (cpp_name.count(type_id) != 0u) {
      return cpp_name.at(type_id);
    }
    return "UNKNOWN_BASIC_TYPE_" + bricks::strings::ToString(type_id);
  }
  std::string PrimitiveTypeName(const Language::FSharp&, const TypeID type_id) const {
    if (fsharp_name.count(type_id) != 0u) {
      return fsharp_name.at(type_id);
    }
    return "UNKNOWN_BASIC_TYPE_" + bricks::strings::ToString(type_id);
  }
};

struct StructSchema {
  StructSchema() = default;
  StructSchema(const SchemaInfo& schema) : schema_(schema) {}

  template <typename T>
  ENABLE_IF<!IS_CURRENT_STRUCT(T)> AddType() {}

  template <typename T>
  ENABLE_IF<IS_CURRENT_STRUCT(T)> AddType() {
    TraverseType(Reflector().ReflectType<T>());
  }

  // TODO(dkorolev): Polymorphic types.

  const SchemaInfo& GetSchemaInfo() const { return schema_; }

  template <typename L>
  std::string Describe(const L& language, bool headers = true) const {
    std::ostringstream oss;
    Describe(language, oss, headers);
    return oss.str();
  }

  template <typename L>
  void Describe(const L& language, std::ostream& os, bool headers = true) const {
    if (headers) {
      os << L::Header();
    }
    for (TypeID type_id : schema_.ordered_struct_list) {
      const uint64_t type_prefix = TypePrefix(type_id);
      if (type_prefix == TYPEID_STRUCT_PREFIX) {
        const auto cit = schema_.structs.find(type_id);
        if (cit == schema_.structs.end()) {
          os << L::ErrorMessageWithTypeId(type_id);
        }
        DescribeStruct(language, cit->second, os);
      } else {
        os << "Runtime error. Which should really be a compilation error.\n";
        throw std::logic_error("TODO(dkorolev): Refactor this code to make the check compile-time.");
      }
    }
    if (headers) {
      os << L::Footer();
    }
  }

 private:
  SchemaInfo schema_;
  const PrimitiveTypesList primitive_types_;

  void TraverseType(const std::shared_ptr<ReflectedTypeImpl> reflected_type) {
    assert(reflected_type);

    const TypeID type_id = reflected_type->type_id;
    const uint64_t type_prefix = TypePrefix(type_id);

    // Do not process primitive or already known complex type or struct.
    if (type_prefix == TYPEID_BASIC_PREFIX || schema_.structs.count(type_id) || schema_.types.count(type_id)) {
      return;
    }

    if (type_prefix == TYPEID_STRUCT_PREFIX) {
      const ReflectedType_Struct* s = dynamic_cast<const ReflectedType_Struct*>(reflected_type.get());
      // Fill `structs[type_id]` before traversing everything else to break possible circular dependencies.
      schema_.structs[type_id] = StructInfo(reflected_type);
      if (s->reflected_super) {
        TraverseType(s->reflected_super);
      }
      for (const auto& f : s->fields) {
        TraverseType(f.first);
      }
      schema_.ordered_struct_list.push_back(type_id);
      return;
    }

    if (type_prefix == TYPEID_ENUM_PREFIX) {
      TypeInfo type_info;
      type_info.type_id = type_id;
      const ReflectedType_Enum* e = dynamic_cast<const ReflectedType_Enum*>(reflected_type.get());
      type_info.enum_name = e->name;
      type_info.included_types.push_back(e->reflected_underlying_type->type_id);
      schema_.types[type_id] = type_info;
      return;
    }

    if (type_prefix == TYPEID_VECTOR_PREFIX) {
      const std::shared_ptr<ReflectedTypeImpl> reflected_element_type =
          std::dynamic_pointer_cast<ReflectedType_Vector>(reflected_type)->reflected_element_type;
      assert(reflected_element_type);
      TypeInfo type_info;
      type_info.type_id = type_id;
      type_info.included_types.push_back(reflected_element_type->type_id);
      schema_.types[type_id] = type_info;
      TraverseType(reflected_element_type);
      return;
    }

    if (type_prefix == TYPEID_PAIR_PREFIX) {
      const std::shared_ptr<ReflectedTypeImpl> reflected_first_type =
          std::dynamic_pointer_cast<ReflectedType_Pair>(reflected_type)->reflected_first_type;
      const std::shared_ptr<ReflectedTypeImpl> reflected_second_type =
          std::dynamic_pointer_cast<ReflectedType_Pair>(reflected_type)->reflected_second_type;
      assert(reflected_first_type);
      assert(reflected_second_type);
      TypeInfo type_info;
      type_info.type_id = type_id;
      type_info.included_types.push_back(reflected_first_type->type_id);
      type_info.included_types.push_back(reflected_second_type->type_id);
      schema_.types[type_id] = type_info;
      TraverseType(reflected_first_type);
      TraverseType(reflected_second_type);
      return;
    }

    if (type_prefix == TYPEID_MAP_PREFIX) {
      std::shared_ptr<ReflectedTypeImpl> reflected_key_type =
          std::dynamic_pointer_cast<ReflectedType_Map>(reflected_type)->reflected_key_type;
      std::shared_ptr<ReflectedTypeImpl> reflected_value_type =
          std::dynamic_pointer_cast<ReflectedType_Map>(reflected_type)->reflected_value_type;
      assert(reflected_key_type);
      assert(reflected_value_type);
      TypeInfo type_info;
      type_info.type_id = type_id;
      type_info.included_types.push_back(reflected_key_type->type_id);
      type_info.included_types.push_back(reflected_value_type->type_id);
      schema_.types[type_id] = type_info;
      TraverseType(reflected_key_type);
      TraverseType(reflected_value_type);
      return;
    }

    if (type_prefix == TYPEID_OPTIONAL_PREFIX) {
      const std::shared_ptr<ReflectedTypeImpl> reflected_object_type =
          std::dynamic_pointer_cast<ReflectedType_Optional>(reflected_type)->reflected_object_type;
      assert(reflected_object_type);
      TypeInfo type_info;
      type_info.type_id = type_id;
      type_info.included_types.push_back(reflected_object_type->type_id);
      schema_.types[type_id] = type_info;
      TraverseType(reflected_object_type);
      return;
    }
    // Type left unhandled, this should never happen.
    assert(false);
  }

  void DescribeStruct(const Language::CPP& language, const StructInfo& struct_info, std::ostream& os) const {
    os << "struct " << struct_info.name;
    if (struct_info.super_type_id != TypeID::INVALID_TYPE) {
      os << " : " << TypePrintName(language, struct_info.super_type_id);
    }
    os << " {\n";
    for (const auto& f : struct_info.fields) {
      os << "  " << TypePrintName(language, f.first) << " " << f.second << ";\n";
    }
    os << "};\n";
  }

  void DescribeStruct(const Language::FSharp& language, const StructInfo& struct_info, std::ostream& os) const {
    os << "type " << struct_info.name;
    if (struct_info.super_type_id != TypeID::INVALID_TYPE) {
      os << "  // With unsupported for now base type " << TypePrintName(language, struct_info.super_type_id);
    }
    os << " = {\n";
    for (const auto& f : struct_info.fields) {
      os << "  " << f.second << " : " << TypePrintName(language, f.first) << '\n';
    }
    os << "}\n";
  }

  template <typename L>
  std::string TypePrintName(const L& language, const TypeID type_id) const {
    const uint64_t type_prefix = TypePrefix(type_id);
    if (type_prefix == TYPEID_BASIC_PREFIX) {
      return primitive_types_.PrimitiveTypeName(language, type_id);
    } else if (type_prefix == TYPEID_STRUCT_PREFIX) {
      const auto cit = schema_.structs.find(type_id);
      if (cit == schema_.structs.end()) {
        return "UNKNOWN_STRUCT_" + bricks::strings::ToString(type_id);
      } else {
        return cit->second.name;
      }
    } else {
      const auto cit = schema_.types.find(type_id);
      if (cit == schema_.types.end()) {
        return "UNKNOWN_INTERMEDIATE_TYPE_" + bricks::strings::ToString(type_id);
      } else {
        if (type_prefix == TYPEID_ENUM_PREFIX) {
          return cit->second.enum_name;
        } else {
          if (type_prefix == TYPEID_VECTOR_PREFIX) {
            return DescribeVector(language, cit->second);
          }
          if (type_prefix == TYPEID_PAIR_PREFIX) {
            return DescribePair(language, cit->second);
          }
          if (type_prefix == TYPEID_MAP_PREFIX) {
            return DescribeMap(language, cit->second);
          }
          if (type_prefix == TYPEID_OPTIONAL_PREFIX) {
            return DescribeOptional(language, cit->second);
          }
          return "UNHANDLED_TYPE_" + bricks::strings::ToString(type_id);
        }
      }
    }
  }

  std::string DescribeVector(const Language::CPP& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 1u);
    return "std::vector<" + TypePrintName(language, type_info.included_types[0]) + '>';
  }
  std::string DescribeVector(const Language::FSharp& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 1u);
    return TypePrintName(language, type_info.included_types[0]) + " array";
  }

  std::string DescribePair(const Language::CPP& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 2u);
    return "std::pair<" + TypePrintName(language, type_info.included_types[0]) + ", " +
           TypePrintName(language, type_info.included_types[1]) + '>';
  }
  std::string DescribePair(const Language::FSharp& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 2u);
    return TypePrintName(language, type_info.included_types[0]) + " * " +
           TypePrintName(language, type_info.included_types[1]);
  }

  std::string DescribeMap(const Language::CPP& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 2u);
    return "std::map<" + TypePrintName(language, type_info.included_types[0]) + ", " +
           TypePrintName(language, type_info.included_types[1]) + '>';
  }
  std::string DescribeMap(const Language::FSharp& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 2u);
    // TODO(dkorolev): `System.Collections.Generic.Dictionary<>`? See how well does it play with `fsharpi`.
    return "UNSUPPORTED_MAP<" + TypePrintName(language, type_info.included_types[0]) + ", " +
           TypePrintName(language, type_info.included_types[1]) + '>';
  }

  std::string DescribeOptional(const Language::CPP& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 1u);
    return "Optional<" + TypePrintName(language, type_info.included_types[0]) + '>';
  }
  std::string DescribeOptional(const Language::FSharp& language, const TypeInfo& type_info) const {
    assert(type_info.included_types.size() == 1u);
    return TypePrintName(language, type_info.included_types[0]) + " option";
  }
};

}  // namespace reflection
}  // namespace current

#endif  // CURRENT_TYPE_SYSTEM_REFLECTION_SCHEMA_H
