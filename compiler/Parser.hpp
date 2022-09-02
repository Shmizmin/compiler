// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file Parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 9 "Parser.yy"

#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
  class driver;

#include "Types.hpp"
#include "Driver.hpp"
#include "Context.hpp"
#include "Central.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include <cstdint>
#include <cstdlib>

#line 67 "Parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif

#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 207 "Parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // IDENTIFIER
      // STRINGCONST
      char dummy1[sizeof (std::string)];

      // NUMCONST
      char dummy2[sizeof (std::uint8_t)];

      // fdecl_args_opt
      // fdecl_args
      char dummy3[sizeof (std::vector<ti::Argument>)];

      // fcall_args_opt
      // fcall_args
      char dummy4[sizeof (std::vector<ti::Expression*>)];

      // definitions_opt
      // definitions
      char dummy5[sizeof (std::vector<ti::Function>)];

      // statements
      // statements_opt
      char dummy6[sizeof (std::vector<ti::Statement*>)];

      // variable_declarator
      // variable_declarator_i
      char dummy7[sizeof (std::vector<ti::Variable*>)];

      // complete_type
      char dummy8[sizeof (ti::CompleteType)];

      // expression
      // expression_opt
      char dummy9[sizeof (ti::Expression*)];

      // definition
      // function_declarator
      // function_header
      char dummy10[sizeof (ti::Function)];

      // statement
      char dummy11[sizeof (ti::Statement*)];

      // type_qualifier
      char dummy12[sizeof (ti::TypeQualifier)];

      // type_specifier
      char dummy13[sizeof (ti::TypeSpecifier)];

      // type_visibility
      char dummy14[sizeof (ti::TypeVisibility)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        T_YYEMPTY = -2,
    T_END = 0,                     // "end of file"
    T_YYerror = 256,               // error
    T_YYUNDEF = 257,               // "invalid token"
    T_MINUS_MINUS = 258,           // "--"
    T_PLUS_PLUS = 259,             // "++"
    T_MINUS = 260,                 // "-"
    T_PLUS = 261,                  // "+"
    T_BITOR = 262,                 // "|"
    T_BITAND = 263,                // "&"
    T_BITXOR = 264,                // "^"
    T_BITLSHIFT = 265,             // "<<"
    T_BITRSHIFT = 266,             // ">>"
    T_LESS = 267,                  // "<"
    T_GREATER = 268,               // ">"
    T_NOT_EQ = 269,                // "!="
    T_IS_EQ = 270,                 // "=="
    T_SEMICOLON = 271,             // ";"
    T_COMMA = 272,                 // ","
    T_EQ = 273,                    // "="
    T_LPAREN = 274,                // "("
    T_RPAREN = 275,                // ")"
    T_LBRACE = 276,                // "{"
    T_RBRACE = 277,                // "}"
    T_FUNCTION = 278,              // "function"
    T_PROTO = 279,                 // "proto"
    T_VAL = 280,                   // "val"
    T_VOID = 281,                  // "void"
    T_BYTE = 282,                  // "byte"
    T_LOCAL = 283,                 // "local"
    T_GLOBAL = 284,                // "global"
    T_IF = 285,                    // "if"
    T_WHILE = 286,                 // "while"
    T_RETURN = 287,                // "return"
    T_IDENTIFIER = 290,            // IDENTIFIER
    T_STRINGCONST = 291,           // STRINGCONST
    T_NUMCONST = 292               // NUMCONST
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 38, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_MINUS_MINUS = 3,                       // "--"
        S_PLUS_PLUS = 4,                         // "++"
        S_MINUS = 5,                             // "-"
        S_PLUS = 6,                              // "+"
        S_BITOR = 7,                             // "|"
        S_BITAND = 8,                            // "&"
        S_BITXOR = 9,                            // "^"
        S_BITLSHIFT = 10,                        // "<<"
        S_BITRSHIFT = 11,                        // ">>"
        S_LESS = 12,                             // "<"
        S_GREATER = 13,                          // ">"
        S_NOT_EQ = 14,                           // "!="
        S_IS_EQ = 15,                            // "=="
        S_SEMICOLON = 16,                        // ";"
        S_COMMA = 17,                            // ","
        S_EQ = 18,                               // "="
        S_LPAREN = 19,                           // "("
        S_RPAREN = 20,                           // ")"
        S_LBRACE = 21,                           // "{"
        S_RBRACE = 22,                           // "}"
        S_FUNCTION = 23,                         // "function"
        S_PROTO = 24,                            // "proto"
        S_VAL = 25,                              // "val"
        S_VOID = 26,                             // "void"
        S_BYTE = 27,                             // "byte"
        S_LOCAL = 28,                            // "local"
        S_GLOBAL = 29,                           // "global"
        S_IF = 30,                               // "if"
        S_WHILE = 31,                            // "while"
        S_RETURN = 32,                           // "return"
        S_33_ = 33,                              // "?"
        S_34_ = 34,                              // ":"
        S_IDENTIFIER = 35,                       // IDENTIFIER
        S_STRINGCONST = 36,                      // STRINGCONST
        S_NUMCONST = 37,                         // NUMCONST
        S_YYACCEPT = 38,                         // $accept
        S_translation_unit = 39,                 // translation_unit
        S_definitions_opt = 40,                  // definitions_opt
        S_definitions = 41,                      // definitions
        S_definition = 42,                       // definition
        S_type_specifier = 43,                   // type_specifier
        S_type_qualifier = 44,                   // type_qualifier
        S_type_visibility = 45,                  // type_visibility
        S_complete_type = 46,                    // complete_type
        S_function_declarator = 47,              // function_declarator
        S_function_header = 48,                  // function_header
        S_fdecl_args_opt = 49,                   // fdecl_args_opt
        S_fdecl_args = 50,                       // fdecl_args
        S_fcall_args_opt = 51,                   // fcall_args_opt
        S_fcall_args = 52,                       // fcall_args
        S_variable_declarator = 53,              // variable_declarator
        S_variable_declarator_i = 54,            // variable_declarator_i
        S_statement = 55,                        // statement
        S_statements = 56,                       // statements
        S_statements_opt = 57,                   // statements_opt
        S_expression = 58,                       // expression
        S_expression_opt = 59                    // expression_opt
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
      {
        switch (this->kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.move< std::string > (std::move (that.value));
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.move< std::uint8_t > (std::move (that.value));
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.move< std::vector<ti::Argument> > (std::move (that.value));
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.move< std::vector<ti::Expression*> > (std::move (that.value));
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.move< std::vector<ti::Function> > (std::move (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.move< std::vector<ti::Statement*> > (std::move (that.value));
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.move< std::vector<ti::Variable*> > (std::move (that.value));
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.move< ti::CompleteType > (std::move (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.move< ti::Expression* > (std::move (that.value));
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.move< ti::Function > (std::move (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< ti::Statement* > (std::move (that.value));
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.move< ti::TypeQualifier > (std::move (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< ti::TypeSpecifier > (std::move (that.value));
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.move< ti::TypeVisibility > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#else
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::uint8_t&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::uint8_t& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ti::Argument>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ti::Argument>& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ti::Expression*>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ti::Expression*>& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ti::Function>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ti::Function>& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ti::Statement*>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ti::Statement*>& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ti::Variable*>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ti::Variable*>& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::CompleteType&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::CompleteType& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::Expression*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::Expression*& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::Function&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::Function& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::Statement*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::Statement*& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::TypeQualifier&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::TypeQualifier& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::TypeSpecifier&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::TypeSpecifier& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ti::TypeVisibility&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ti::TypeVisibility& v)
        : Base (t)
        , value (v)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.template destroy< std::string > ();
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.template destroy< std::uint8_t > ();
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.template destroy< std::vector<ti::Argument> > ();
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.template destroy< std::vector<ti::Expression*> > ();
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.template destroy< std::vector<ti::Function> > ();
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.template destroy< std::vector<ti::Statement*> > ();
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.template destroy< std::vector<ti::Variable*> > ();
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.template destroy< ti::CompleteType > ();
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.template destroy< ti::Expression* > ();
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.template destroy< ti::Function > ();
        break;

      case symbol_kind::S_statement: // statement
        value.template destroy< ti::Statement* > ();
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.template destroy< ti::TypeQualifier > ();
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.template destroy< ti::TypeSpecifier > ();
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.template destroy< ti::TypeVisibility > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#else
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::T_END
                   || (token::T_YYerror <= tok && tok <= 289));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const std::string& v)
        : super_type (token_kind_type (tok), v)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::T_IDENTIFIER <= tok && tok <= token::T_STRINGCONST));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::uint8_t v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const std::uint8_t& v)
        : super_type (token_kind_type (tok), v)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::T_NUMCONST);
#endif
      }
    };

    /// Build a parser object.
    parser (driver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END ()
      {
        return symbol_type (token::T_END);
      }
#else
      static
      symbol_type
      make_END ()
      {
        return symbol_type (token::T_END);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::T_YYerror);
      }
#else
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::T_YYerror);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::T_YYUNDEF);
      }
#else
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::T_YYUNDEF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS_MINUS ()
      {
        return symbol_type (token::T_MINUS_MINUS);
      }
#else
      static
      symbol_type
      make_MINUS_MINUS ()
      {
        return symbol_type (token::T_MINUS_MINUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS_PLUS ()
      {
        return symbol_type (token::T_PLUS_PLUS);
      }
#else
      static
      symbol_type
      make_PLUS_PLUS ()
      {
        return symbol_type (token::T_PLUS_PLUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::T_MINUS);
      }
#else
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::T_MINUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::T_PLUS);
      }
#else
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::T_PLUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITOR ()
      {
        return symbol_type (token::T_BITOR);
      }
#else
      static
      symbol_type
      make_BITOR ()
      {
        return symbol_type (token::T_BITOR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITAND ()
      {
        return symbol_type (token::T_BITAND);
      }
#else
      static
      symbol_type
      make_BITAND ()
      {
        return symbol_type (token::T_BITAND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITXOR ()
      {
        return symbol_type (token::T_BITXOR);
      }
#else
      static
      symbol_type
      make_BITXOR ()
      {
        return symbol_type (token::T_BITXOR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITLSHIFT ()
      {
        return symbol_type (token::T_BITLSHIFT);
      }
#else
      static
      symbol_type
      make_BITLSHIFT ()
      {
        return symbol_type (token::T_BITLSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITRSHIFT ()
      {
        return symbol_type (token::T_BITRSHIFT);
      }
#else
      static
      symbol_type
      make_BITRSHIFT ()
      {
        return symbol_type (token::T_BITRSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LESS ()
      {
        return symbol_type (token::T_LESS);
      }
#else
      static
      symbol_type
      make_LESS ()
      {
        return symbol_type (token::T_LESS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GREATER ()
      {
        return symbol_type (token::T_GREATER);
      }
#else
      static
      symbol_type
      make_GREATER ()
      {
        return symbol_type (token::T_GREATER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOT_EQ ()
      {
        return symbol_type (token::T_NOT_EQ);
      }
#else
      static
      symbol_type
      make_NOT_EQ ()
      {
        return symbol_type (token::T_NOT_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IS_EQ ()
      {
        return symbol_type (token::T_IS_EQ);
      }
#else
      static
      symbol_type
      make_IS_EQ ()
      {
        return symbol_type (token::T_IS_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON ()
      {
        return symbol_type (token::T_SEMICOLON);
      }
#else
      static
      symbol_type
      make_SEMICOLON ()
      {
        return symbol_type (token::T_SEMICOLON);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::T_COMMA);
      }
#else
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::T_COMMA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ ()
      {
        return symbol_type (token::T_EQ);
      }
#else
      static
      symbol_type
      make_EQ ()
      {
        return symbol_type (token::T_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPAREN ()
      {
        return symbol_type (token::T_LPAREN);
      }
#else
      static
      symbol_type
      make_LPAREN ()
      {
        return symbol_type (token::T_LPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPAREN ()
      {
        return symbol_type (token::T_RPAREN);
      }
#else
      static
      symbol_type
      make_RPAREN ()
      {
        return symbol_type (token::T_RPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LBRACE ()
      {
        return symbol_type (token::T_LBRACE);
      }
#else
      static
      symbol_type
      make_LBRACE ()
      {
        return symbol_type (token::T_LBRACE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RBRACE ()
      {
        return symbol_type (token::T_RBRACE);
      }
#else
      static
      symbol_type
      make_RBRACE ()
      {
        return symbol_type (token::T_RBRACE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FUNCTION ()
      {
        return symbol_type (token::T_FUNCTION);
      }
#else
      static
      symbol_type
      make_FUNCTION ()
      {
        return symbol_type (token::T_FUNCTION);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PROTO ()
      {
        return symbol_type (token::T_PROTO);
      }
#else
      static
      symbol_type
      make_PROTO ()
      {
        return symbol_type (token::T_PROTO);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VAL ()
      {
        return symbol_type (token::T_VAL);
      }
#else
      static
      symbol_type
      make_VAL ()
      {
        return symbol_type (token::T_VAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VOID ()
      {
        return symbol_type (token::T_VOID);
      }
#else
      static
      symbol_type
      make_VOID ()
      {
        return symbol_type (token::T_VOID);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BYTE ()
      {
        return symbol_type (token::T_BYTE);
      }
#else
      static
      symbol_type
      make_BYTE ()
      {
        return symbol_type (token::T_BYTE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOCAL ()
      {
        return symbol_type (token::T_LOCAL);
      }
#else
      static
      symbol_type
      make_LOCAL ()
      {
        return symbol_type (token::T_LOCAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLOBAL ()
      {
        return symbol_type (token::T_GLOBAL);
      }
#else
      static
      symbol_type
      make_GLOBAL ()
      {
        return symbol_type (token::T_GLOBAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IF ()
      {
        return symbol_type (token::T_IF);
      }
#else
      static
      symbol_type
      make_IF ()
      {
        return symbol_type (token::T_IF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHILE ()
      {
        return symbol_type (token::T_WHILE);
      }
#else
      static
      symbol_type
      make_WHILE ()
      {
        return symbol_type (token::T_WHILE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RETURN ()
      {
        return symbol_type (token::T_RETURN);
      }
#else
      static
      symbol_type
      make_RETURN ()
      {
        return symbol_type (token::T_RETURN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v)
      {
        return symbol_type (token::T_IDENTIFIER, std::move (v));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v)
      {
        return symbol_type (token::T_IDENTIFIER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRINGCONST (std::string v)
      {
        return symbol_type (token::T_STRINGCONST, std::move (v));
      }
#else
      static
      symbol_type
      make_STRINGCONST (const std::string& v)
      {
        return symbol_type (token::T_STRINGCONST, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMCONST (std::uint8_t v)
      {
        return symbol_type (token::T_NUMCONST, std::move (v));
      }
#else
      static
      symbol_type
      make_NUMCONST (const std::uint8_t& v)
      {
        return symbol_type (token::T_NUMCONST, v);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 222,     ///< Last index in yytable_.
      yynnts_ = 22,  ///< Number of nonterminal symbols.
      yyfinal_ = 14 ///< Termination state number.
    };


    // User arguments.
    driver& drv;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
    };
    // Last valid token kind.
    const int code_max = 292;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.copy< std::uint8_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.copy< std::vector<ti::Argument> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.copy< std::vector<ti::Expression*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.copy< std::vector<ti::Function> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.copy< std::vector<ti::Statement*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.copy< std::vector<ti::Variable*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.copy< ti::CompleteType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.copy< ti::Expression* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.copy< ti::Function > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.copy< ti::Statement* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.copy< ti::TypeQualifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.copy< ti::TypeSpecifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.copy< ti::TypeVisibility > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.move< std::uint8_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.move< std::vector<ti::Argument> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.move< std::vector<ti::Expression*> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.move< std::vector<ti::Function> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.move< std::vector<ti::Statement*> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.move< std::vector<ti::Variable*> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.move< ti::CompleteType > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.move< ti::Expression* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.move< ti::Function > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< ti::Statement* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.move< ti::TypeQualifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< ti::TypeSpecifier > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.move< ti::TypeVisibility > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // yy
#line 2344 "Parser.hpp"




#endif // !YY_YY_PARSER_HPP_INCLUDED
