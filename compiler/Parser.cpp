// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "Parser.hpp"


// Unqualified %code blocks.
#line 35 "Parser.yy"

#include "Types.hpp"
#include "Driver.hpp"
#include "Context.hpp"
#include "Central.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

#line 56 "Parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 129 "Parser.cpp"

  /// Build a parser object.
  parser::parser (driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.YY_MOVE_OR_COPY< std::uint8_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.YY_MOVE_OR_COPY< std::vector<ti::Argument> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.YY_MOVE_OR_COPY< std::vector<ti::Expression*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.YY_MOVE_OR_COPY< std::vector<ti::Function> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.YY_MOVE_OR_COPY< std::vector<ti::Statement*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.YY_MOVE_OR_COPY< std::vector<ti::Variable*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.YY_MOVE_OR_COPY< ti::CompleteType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.YY_MOVE_OR_COPY< ti::Expression* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.YY_MOVE_OR_COPY< ti::Function > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.YY_MOVE_OR_COPY< ti::Statement* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.YY_MOVE_OR_COPY< ti::TypeQualifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.YY_MOVE_OR_COPY< ti::TypeSpecifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.YY_MOVE_OR_COPY< ti::TypeVisibility > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.move< std::uint8_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.move< std::vector<ti::Argument> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.move< std::vector<ti::Expression*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.move< std::vector<ti::Function> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.move< std::vector<ti::Statement*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.move< std::vector<ti::Variable*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.move< ti::CompleteType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.move< ti::Expression* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.move< ti::Function > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< ti::Statement* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.move< ti::TypeQualifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< ti::TypeSpecifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.move< ti::TypeVisibility > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.copy< std::uint8_t > (that.value);
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.copy< std::vector<ti::Argument> > (that.value);
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.copy< std::vector<ti::Expression*> > (that.value);
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.copy< std::vector<ti::Function> > (that.value);
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.copy< std::vector<ti::Statement*> > (that.value);
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.copy< std::vector<ti::Variable*> > (that.value);
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.copy< ti::CompleteType > (that.value);
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.copy< ti::Expression* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.copy< ti::Function > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.copy< ti::Statement* > (that.value);
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.copy< ti::TypeQualifier > (that.value);
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.copy< ti::TypeSpecifier > (that.value);
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.copy< ti::TypeVisibility > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        value.move< std::uint8_t > (that.value);
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        value.move< std::vector<ti::Argument> > (that.value);
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        value.move< std::vector<ti::Expression*> > (that.value);
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        value.move< std::vector<ti::Function> > (that.value);
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        value.move< std::vector<ti::Statement*> > (that.value);
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        value.move< std::vector<ti::Variable*> > (that.value);
        break;

      case symbol_kind::S_complete_type: // complete_type
        value.move< ti::CompleteType > (that.value);
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        value.move< ti::Expression* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        value.move< ti::Function > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.move< ti::Statement* > (that.value);
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        value.move< ti::TypeQualifier > (that.value);
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< ti::TypeSpecifier > (that.value);
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        value.move< ti::TypeVisibility > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRINGCONST: // STRINGCONST
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_NUMCONST: // NUMCONST
        yylhs.value.emplace< std::uint8_t > ();
        break;

      case symbol_kind::S_fdecl_args_opt: // fdecl_args_opt
      case symbol_kind::S_fdecl_args: // fdecl_args
        yylhs.value.emplace< std::vector<ti::Argument> > ();
        break;

      case symbol_kind::S_fcall_args_opt: // fcall_args_opt
      case symbol_kind::S_fcall_args: // fcall_args
        yylhs.value.emplace< std::vector<ti::Expression*> > ();
        break;

      case symbol_kind::S_definitions_opt: // definitions_opt
      case symbol_kind::S_definitions: // definitions
        yylhs.value.emplace< std::vector<ti::Function> > ();
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statements_opt: // statements_opt
        yylhs.value.emplace< std::vector<ti::Statement*> > ();
        break;

      case symbol_kind::S_variable_declarator: // variable_declarator
      case symbol_kind::S_variable_declarator_i: // variable_declarator_i
        yylhs.value.emplace< std::vector<ti::Variable*> > ();
        break;

      case symbol_kind::S_complete_type: // complete_type
        yylhs.value.emplace< ti::CompleteType > ();
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression_opt: // expression_opt
        yylhs.value.emplace< ti::Expression* > ();
        break;

      case symbol_kind::S_definition: // definition
      case symbol_kind::S_function_declarator: // function_declarator
      case symbol_kind::S_function_header: // function_header
        yylhs.value.emplace< ti::Function > ();
        break;

      case symbol_kind::S_statement: // statement
        yylhs.value.emplace< ti::Statement* > ();
        break;

      case symbol_kind::S_type_qualifier: // type_qualifier
        yylhs.value.emplace< ti::TypeQualifier > ();
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        yylhs.value.emplace< ti::TypeSpecifier > ();
        break;

      case symbol_kind::S_type_visibility: // type_visibility
        yylhs.value.emplace< ti::TypeVisibility > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 3: // definitions_opt: %empty
#line 142 "Parser.yy"
              { yylhs.value.as < std::vector<ti::Function> > () = {}; }
#line 836 "Parser.cpp"
    break;

  case 4: // definitions_opt: definitions
#line 143 "Parser.yy"
              { yylhs.value.as < std::vector<ti::Function> > () = yystack_[0].value.as < std::vector<ti::Function> > (); }
#line 842 "Parser.cpp"
    break;

  case 5: // definitions: definitions definition
#line 148 "Parser.yy"
{
    drv.definition_queue.emplace_back(yystack_[0].value.as < ti::Function > ());
    yylhs.value.as < std::vector<ti::Function> > () = drv.definition_queue;
}
#line 851 "Parser.cpp"
    break;

  case 6: // definitions: definition
#line 153 "Parser.yy"
{
    drv.definition_queue.emplace_back(yystack_[0].value.as < ti::Function > ());
    yylhs.value.as < std::vector<ti::Function> > () = drv.definition_queue;
}
#line 860 "Parser.cpp"
    break;

  case 7: // definition: function_declarator
#line 160 "Parser.yy"
                      { yylhs.value.as < ti::Function > () = yystack_[0].value.as < ti::Function > (); }
#line 866 "Parser.cpp"
    break;

  case 8: // type_specifier: "byte"
#line 167 "Parser.yy"
         { yylhs.value.as < ti::TypeSpecifier > () = ti::TypeSpecifier::BYTE; }
#line 872 "Parser.cpp"
    break;

  case 9: // type_specifier: "void"
#line 168 "Parser.yy"
         { yylhs.value.as < ti::TypeSpecifier > () = ti::TypeSpecifier::VOID; }
#line 878 "Parser.cpp"
    break;

  case 10: // type_qualifier: "val"
#line 172 "Parser.yy"
        { yylhs.value.as < ti::TypeQualifier > () = ti::TypeQualifier::VAL; }
#line 884 "Parser.cpp"
    break;

  case 11: // type_visibility: "local"
#line 176 "Parser.yy"
           { yylhs.value.as < ti::TypeVisibility > () = ti::TypeVisibility::LOCAL; }
#line 890 "Parser.cpp"
    break;

  case 12: // type_visibility: "global"
#line 177 "Parser.yy"
           { yylhs.value.as < ti::TypeVisibility > () = ti::TypeVisibility::GLOBAL; }
#line 896 "Parser.cpp"
    break;

  case 13: // complete_type: type_specifier type_qualifier
#line 181 "Parser.yy"
                                { yylhs.value.as < ti::CompleteType > () = ti::CompleteType{ yystack_[1].value.as < ti::TypeSpecifier > (), yystack_[0].value.as < ti::TypeQualifier > () }; }
#line 902 "Parser.cpp"
    break;

  case 14: // function_declarator: "proto" function_header ";"
#line 188 "Parser.yy"
                              { yylhs.value.as < ti::Function > () = yystack_[1].value.as < ti::Function > (); }
#line 908 "Parser.cpp"
    break;

  case 15: // function_declarator: function_header statement
#line 189 "Parser.yy"
                              { auto a = yystack_[1].value.as < ti::Function > (); a.body = yystack_[0].value.as < ti::Statement* > (); yylhs.value.as < ti::Function > () = a; }
#line 914 "Parser.cpp"
    break;

  case 16: // function_header: "function" complete_type IDENTIFIER "=" "(" fdecl_args_opt ")"
#line 193 "Parser.yy"
                                                                 { yylhs.value.as < ti::Function > () = ti::Function{ yystack_[4].value.as < std::string > (), yystack_[5].value.as < ti::CompleteType > (), yystack_[1].value.as < std::vector<ti::Argument> > (), NULL }; }
#line 920 "Parser.cpp"
    break;

  case 17: // fdecl_args_opt: %empty
#line 198 "Parser.yy"
                 { yylhs.value.as < std::vector<ti::Argument> > () = {}; }
#line 926 "Parser.cpp"
    break;

  case 18: // fdecl_args_opt: fdecl_args
#line 199 "Parser.yy"
                 { yylhs.value.as < std::vector<ti::Argument> > () = yystack_[0].value.as < std::vector<ti::Argument> > (); }
#line 932 "Parser.cpp"
    break;

  case 19: // fdecl_args: complete_type IDENTIFIER
#line 204 "Parser.yy"
    {
        drv.fdecl_args_queue.emplace_back(ti::Argument{ yystack_[0].value.as < std::string > (), yystack_[1].value.as < ti::CompleteType > () });
        yylhs.value.as < std::vector<ti::Argument> > () = drv.fdecl_args_queue;
    }
#line 941 "Parser.cpp"
    break;

  case 20: // fdecl_args: fdecl_args "," complete_type IDENTIFIER
#line 209 "Parser.yy"
    {
        drv.fdecl_args_queue.emplace_back(ti::Argument{ yystack_[0].value.as < std::string > (), yystack_[1].value.as < ti::CompleteType > () });
        yylhs.value.as < std::vector<ti::Argument> > () = drv.fdecl_args_queue;
    }
#line 950 "Parser.cpp"
    break;

  case 21: // fcall_args_opt: %empty
#line 218 "Parser.yy"
                 { yylhs.value.as < std::vector<ti::Expression*> > () = {}; }
#line 956 "Parser.cpp"
    break;

  case 22: // fcall_args_opt: fcall_args
#line 219 "Parser.yy"
                 { yylhs.value.as < std::vector<ti::Expression*> > () = yystack_[0].value.as < std::vector<ti::Expression*> > (); }
#line 962 "Parser.cpp"
    break;

  case 23: // fcall_args: expression
#line 224 "Parser.yy"
    {
        drv.fcall_args_queue.emplace_back(yystack_[0].value.as < ti::Expression* > ());
        yylhs.value.as < std::vector<ti::Expression*> > () = drv.fcall_args_queue;
    }
#line 971 "Parser.cpp"
    break;

  case 24: // fcall_args: fcall_args "," expression
#line 229 "Parser.yy"
    {
        drv.fcall_args_queue.emplace_back(yystack_[0].value.as < ti::Expression* > ());
        yylhs.value.as < std::vector<ti::Expression*> > () = drv.fcall_args_queue;
    }
#line 980 "Parser.cpp"
    break;

  case 25: // variable_declarator: type_visibility complete_type variable_declarator_i
#line 242 "Parser.yy"
{
    drv.active_visibility = yystack_[2].value.as < ti::TypeVisibility > ();
    drv.active_type = yystack_[1].value.as < ti::CompleteType > ();
    yylhs.value.as < std::vector<ti::Variable*> > () = yystack_[0].value.as < std::vector<ti::Variable*> > ();
}
#line 990 "Parser.cpp"
    break;

  case 26: // variable_declarator: variable_declarator "," variable_declarator_i
#line 248 "Parser.yy"
{
    yylhs.value.as < std::vector<ti::Variable*> > () = yystack_[0].value.as < std::vector<ti::Variable*> > ();
}
#line 998 "Parser.cpp"
    break;

  case 27: // variable_declarator_i: IDENTIFIER "=" expression
#line 255 "Parser.yy"
{
    drv.var_decl_queue.emplace_back(new ti::Variable{ drv.active_visibility, drv.active_type, yystack_[2].value.as < std::string > (), yystack_[0].value.as < ti::Expression* > () });
    yylhs.value.as < std::vector<ti::Variable*> > () = drv.var_decl_queue;
}
#line 1007 "Parser.cpp"
    break;

  case 28: // variable_declarator_i: IDENTIFIER
#line 260 "Parser.yy"
{
    drv.var_decl_queue.emplace_back(new ti::Variable{ drv.active_visibility, drv.active_type, yystack_[0].value.as < std::string > (), NULL });
    yylhs.value.as < std::vector<ti::Variable*> > () = drv.var_decl_queue;
}
#line 1016 "Parser.cpp"
    break;

  case 29: // statement: "{" statements_opt "}"
#line 288 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::Block{ yystack_[1].value.as < std::vector<ti::Statement*> > () }; }
#line 1022 "Parser.cpp"
    break;

  case 30: // statement: "if" "(" expression ")" statement
#line 289 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::If{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Statement* > () }; }
#line 1028 "Parser.cpp"
    break;

  case 31: // statement: "while" "(" expression ")" statement
#line 290 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::While{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Statement* > () }; }
#line 1034 "Parser.cpp"
    break;

  case 32: // statement: "return" expression_opt ";"
#line 291 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::Return{ yystack_[1].value.as < ti::Expression* > () }; }
#line 1040 "Parser.cpp"
    break;

  case 33: // statement: ";"
#line 292 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::Null{}; }
#line 1046 "Parser.cpp"
    break;

  case 34: // statement: variable_declarator ";"
#line 293 "Parser.yy"
                                       { yylhs.value.as < ti::Statement* > () = new ti::stmt::Variable{ yystack_[1].value.as < std::vector<ti::Variable*> > () }; }
#line 1052 "Parser.cpp"
    break;

  case 35: // statements: statements statement
#line 297 "Parser.yy"
                       { drv.statement_queue.emplace_back(yystack_[0].value.as < ti::Statement* > ()); yylhs.value.as < std::vector<ti::Statement*> > () = drv.statement_queue; }
#line 1058 "Parser.cpp"
    break;

  case 36: // statements: statement
#line 298 "Parser.yy"
                       { drv.statement_queue.emplace_back(yystack_[0].value.as < ti::Statement* > ()); yylhs.value.as < std::vector<ti::Statement*> > () = drv.statement_queue; }
#line 1064 "Parser.cpp"
    break;

  case 37: // statements_opt: %empty
#line 302 "Parser.yy"
             { yylhs.value.as < std::vector<ti::Statement*> > () = {}; }
#line 1070 "Parser.cpp"
    break;

  case 38: // statements_opt: statements
#line 303 "Parser.yy"
             { yylhs.value.as < std::vector<ti::Statement*> > () = yystack_[0].value.as < std::vector<ti::Statement*> > (); }
#line 1076 "Parser.cpp"
    break;

  case 39: // expression: "(" expression ")"
#line 310 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = yystack_[1].value.as < ti::Expression* > (); }
#line 1082 "Parser.cpp"
    break;

  case 40: // expression: NUMCONST
#line 311 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::Numconst{ yystack_[0].value.as < std::uint8_t > () }; }
#line 1088 "Parser.cpp"
    break;

  case 41: // expression: STRINGCONST
#line 312 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::Stringconst{ yystack_[0].value.as < std::string > () }; }
#line 1094 "Parser.cpp"
    break;

  case 42: // expression: IDENTIFIER
#line 313 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::Identifier{ yystack_[0].value.as < std::string > () }; }
#line 1100 "Parser.cpp"
    break;

  case 43: // expression: IDENTIFIER "(" fcall_args_opt ")"
#line 314 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::FCall{ new ti::expr::Identifier{ yystack_[3].value.as < std::string > () }, yystack_[1].value.as < std::vector<ti::Expression*> > () }; }
#line 1106 "Parser.cpp"
    break;

  case 44: // expression: IDENTIFIER "=" expression
#line 315 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::Equals{ new ti::expr::Identifier{ yystack_[2].value.as < std::string > () }, yystack_[0].value.as < ti::Expression* > () }; }
#line 1112 "Parser.cpp"
    break;

  case 45: // expression: expression "+" expression
#line 316 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::Plus{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1118 "Parser.cpp"
    break;

  case 46: // expression: expression "-" expression
#line 317 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::Minus{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1124 "Parser.cpp"
    break;

  case 47: // expression: expression "<<" expression
#line 318 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::LeftShift{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1130 "Parser.cpp"
    break;

  case 48: // expression: expression ">>" expression
#line 319 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::RightShift{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1136 "Parser.cpp"
    break;

  case 49: // expression: expression "^" expression
#line 320 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::BitXor{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1142 "Parser.cpp"
    break;

  case 50: // expression: expression "&" expression
#line 321 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::BitAnd{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1148 "Parser.cpp"
    break;

  case 51: // expression: expression "|" expression
#line 322 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::BitOr{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1154 "Parser.cpp"
    break;

  case 52: // expression: expression "++"
#line 323 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::unary::PlusPlus{ yystack_[1].value.as < ti::Expression* > () }; }
#line 1160 "Parser.cpp"
    break;

  case 53: // expression: expression "--"
#line 324 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::unary::MinusMinus{ yystack_[1].value.as < ti::Expression* > () }; }
#line 1166 "Parser.cpp"
    break;

  case 54: // expression: expression "==" expression
#line 325 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::EqualsEquals{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1172 "Parser.cpp"
    break;

  case 55: // expression: expression "!=" expression
#line 326 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::NotEquals{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1178 "Parser.cpp"
    break;

  case 56: // expression: "+" expression
#line 327 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::unary::Positive{ yystack_[0].value.as < ti::Expression* > () }; }
#line 1184 "Parser.cpp"
    break;

  case 57: // expression: "-" expression
#line 328 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::unary::Negative{ yystack_[0].value.as < ti::Expression* > () }; }
#line 1190 "Parser.cpp"
    break;

  case 58: // expression: expression "?" expression ":" expression
#line 329 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::Ternary{ yystack_[4].value.as < ti::Expression* > (), yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1196 "Parser.cpp"
    break;

  case 59: // expression: expression "<" expression
#line 330 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::Less{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1202 "Parser.cpp"
    break;

  case 60: // expression: expression ">" expression
#line 331 "Parser.yy"
                                            { yylhs.value.as < ti::Expression* > () = new ti::expr::binary::Greater{ yystack_[2].value.as < ti::Expression* > (), yystack_[0].value.as < ti::Expression* > () }; }
#line 1208 "Parser.cpp"
    break;

  case 61: // expression_opt: %empty
#line 335 "Parser.yy"
             { yylhs.value.as < ti::Expression* > () = NULL; }
#line 1214 "Parser.cpp"
    break;

  case 62: // expression_opt: expression
#line 336 "Parser.yy"
             { yylhs.value.as < ti::Expression* > () = yystack_[0].value.as < ti::Expression* > (); }
#line 1220 "Parser.cpp"
    break;


#line 1224 "Parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -34;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -16,   -10,   -14,    21,    25,   -16,   -34,   -34,    27,   -34,
     -34,    22,    11,    33,   -34,   -34,   -34,   -34,    27,   -34,
     -34,    31,    34,     5,   -10,     2,   -34,   -34,   -34,    36,
     -34,   -34,    27,    30,     5,     5,     5,     5,     5,    20,
     -34,   -34,   189,    44,    26,   -34,    26,    47,   -34,   -34,
      96,   127,    41,    41,   158,     5,     5,   -34,   -34,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,   -34,    45,   -34,   -34,   -10,    27,    27,   -34,   189,
      48,    50,   189,    41,    41,   140,   171,   202,     9,     9,
     114,   114,    68,    68,    81,     5,    62,    78,    65,   -34,
     -34,   -34,     5,     5,   189,   -34,   -34,   -10,   189,   189,
      77,   -34
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     0,     0,     4,     6,     7,     0,     9,
       8,     0,     0,     0,     1,     2,     5,    33,    37,    11,
      12,     0,     0,    61,     0,     0,    15,    10,    13,     0,
      14,    36,    38,     0,     0,     0,     0,     0,     0,    42,
      41,    40,    62,     0,     0,    34,     0,     0,    35,    29,
       0,     0,    57,    56,     0,     0,    21,    53,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    28,    25,    26,    17,     0,     0,    39,    44,
       0,    22,    23,    46,    45,    51,    50,    49,    47,    48,
      59,    60,    55,    54,     0,     0,     0,     0,    18,    30,
      31,    43,     0,     0,    27,    19,    16,     0,    24,    58,
       0,    20
  };

  const signed char
  parser::yypgoto_[] =
  {
     -34,   -34,   -34,   -34,   108,   -34,   -34,   -34,   -24,   -34,
     124,   -34,   -34,   -34,   -34,   -34,    82,   -12,   -34,   -34,
     -33,   -34
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     3,     4,     5,     6,    11,    28,    24,    12,     7,
       8,    97,    98,    80,    81,    25,    73,    26,    32,    33,
      42,    43
  };

  const signed char
  parser::yytable_[] =
  {
      44,    50,    51,    52,    53,    54,    31,     1,     2,     1,
      36,    37,    57,    58,    59,    60,     9,    10,    45,    46,
      48,    14,    79,    82,    38,    15,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    55,    56,
      39,    40,    41,    17,    57,    58,    29,    27,    18,    30,
      34,    96,    49,    35,    47,    19,    20,    21,    22,    23,
      71,    72,   104,    95,    99,   100,    75,   102,   101,   108,
     109,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,   107,   110,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,   105,   106,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,   111,    16,    70,   103,    76,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    13,     0,    74,    70,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    57,    58,    59,    60,    77,    62,    63,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      70,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    57,    58,    59,    60,    78,     0,
      63,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    57,    58,    59,    60,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    70
  };

  const signed char
  parser::yycheck_[] =
  {
      24,    34,    35,    36,    37,    38,    18,    23,    24,    23,
       5,     6,     3,     4,     5,     6,    26,    27,    16,    17,
      32,     0,    55,    56,    19,     0,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    18,    19,
      35,    36,    37,    16,     3,     4,    35,    25,    21,    16,
      19,    75,    22,    19,    18,    28,    29,    30,    31,    32,
      16,    35,    95,    18,    76,    77,    19,    17,    20,   102,
     103,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    17,   107,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    35,    20,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    35,     5,    33,    34,    20,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     2,    -1,    46,    33,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     3,     4,     5,     6,    20,     8,     9,
      10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,     3,     4,     5,     6,    20,    -1,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33
  };

  const signed char
  parser::yystos_[] =
  {
       0,    23,    24,    39,    40,    41,    42,    47,    48,    26,
      27,    43,    46,    48,     0,     0,    42,    16,    21,    28,
      29,    30,    31,    32,    45,    53,    55,    25,    44,    35,
      16,    55,    56,    57,    19,    19,     5,     6,    19,    35,
      36,    37,    58,    59,    46,    16,    17,    18,    55,    22,
      58,    58,    58,    58,    58,    18,    19,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      33,    16,    35,    54,    54,    19,    20,    20,    20,    58,
      51,    52,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    18,    46,    49,    50,    55,
      55,    20,    17,    34,    58,    35,    20,    17,    58,    58,
      46,    35
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    38,    39,    40,    40,    41,    41,    42,    43,    43,
      44,    45,    45,    46,    47,    47,    48,    49,    49,    50,
      50,    51,    51,    52,    52,    53,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    56,    56,    57,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    59,    59
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     2,     7,     0,     1,     2,
       4,     0,     1,     1,     3,     3,     3,     3,     1,     3,
       5,     5,     3,     1,     2,     2,     1,     0,     1,     3,
       1,     1,     1,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     3,     2,     2,     5,     3,
       3,     0,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"--\"", "\"++\"",
  "\"-\"", "\"+\"", "\"|\"", "\"&\"", "\"^\"", "\"<<\"", "\">>\"", "\"<\"",
  "\">\"", "\"!=\"", "\"==\"", "\";\"", "\",\"", "\"=\"", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"function\"", "\"proto\"", "\"val\"", "\"void\"",
  "\"byte\"", "\"local\"", "\"global\"", "\"if\"", "\"while\"",
  "\"return\"", "\"?\"", "\":\"", "IDENTIFIER", "STRINGCONST", "NUMCONST",
  "$accept", "translation_unit", "definitions_opt", "definitions",
  "definition", "type_specifier", "type_qualifier", "type_visibility",
  "complete_type", "function_declarator", "function_header",
  "fdecl_args_opt", "fdecl_args", "fcall_args_opt", "fcall_args",
  "variable_declarator", "variable_declarator_i", "statement",
  "statements", "statements_opt", "expression", "expression_opt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   137,   137,   142,   143,   147,   152,   160,   167,   168,
     172,   176,   177,   181,   188,   189,   193,   198,   199,   203,
     208,   218,   219,   223,   228,   241,   247,   254,   259,   288,
     289,   290,   291,   292,   293,   297,   298,   302,   303,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   335,   336
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1788 "Parser.cpp"

#line 352 "Parser.yy"


void yy::parser::error(const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
