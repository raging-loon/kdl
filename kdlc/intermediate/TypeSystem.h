#ifndef KDLC_INTERMEDIATE_TYPE_SYSTEM_H_
#define KDLC_INTERMEDIATE_TYPE_SYSTEM_H_

#include <unordered_map>
#include <initializer_list>
#include <string>
#include <memory>
#include "ast/Nodes.h"

namespace kdl
{

///
/// @brief
///     Base/Primitive Types
/// 
enum class BaseType : int
{
    UNTYPED,
    STRING,
    INT,
    ARRAY
};

enum Trait : int
{
    ANY_COMP = (1 << 0),
    EQUALS   = (1 << 1),
    SEARCH   = (1 << 2)
};

using TypeTrait = int;

static std::unordered_map<BaseType, TypeTrait> s_baseTypeTraits = {
    { BaseType::STRING,     Trait::ANY_COMP | Trait::EQUALS | Trait::SEARCH },
    { BaseType::INT,        Trait::ANY_COMP }
};



} // kdl

//namespace std
//{
//
//template <>
//struct std::hash<kdl::OperatorSignature>
//{
//    std::size_t operator()(const kdl::OperatorSignature& s) const noexcept
//    {
//        static constexpr int PRIME = 0x9e3779b9;
//
//        std::size_t lh =  std::hash<int>{}((int)s.lhs);
//        std::size_t rh =  std::hash<int>{}((int)s.rhs);
//        std::size_t oph = std::hash<int>{}((int)s.op);
//
//        std::size_t hash = lh;
//
//        hash ^= rh  + PRIME + (hash << 6) + (hash >> 2);
//        hash ^= oph + PRIME + (hash << 6) + (hash >> 2);
//
//        return hash;
//        
//    }
//
//};
//
//}


#endif // KDLC_INTERMEDIATE_TYPE_SYSTEM_H_