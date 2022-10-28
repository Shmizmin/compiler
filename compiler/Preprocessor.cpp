#include "Preprocessor.hpp"

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <fmt/format.h>

namespace
{
    std::vector<std::string> split(std::string text, char delim)
    {
        std::string line;
        std::vector<std::string> vec;
        std::stringstream ss(text);
        
        while (std::getline(ss, line, delim))
        {
            vec.emplace_back(line);
        }
        
        return vec;
    }
    
    void replace(std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty()) return;
        
        std::size_t start_pos = 0;
        
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
}
    
namespace
{
    struct Macro
    {
        std::string name;
        std::vector<std::string> args;
        std::string code;
    };
    
    struct Context
    {
        std::vector<Macro> defined_macros, invoked_macros;
    };
    
    
    void preprocess_include(Context& context, std::string& code, std::string_view parent_filepath) noexcept
    {
        std::regex rx_incl(R"((\.include \"(.*)\"\s*((;.*$)?|$)))");
        std::smatch matches;
        
        while (std::regex_search(code, matches, rx_incl))
        {
            std::string filepath = matches[2];
            
            if (filepath == parent_filepath)
            {
                std::cerr << fmt::format("[Error] File include recursion is unsupported\nExiting...\n");
                std::exit(EXIT_FAILURE);
            }
            
            const auto size = std::filesystem::file_size(filepath); //bytes
            std::ifstream file(filepath);
            
            if (!file.good())
            {
                std::cerr << fmt::format("[Error] Could not open file {} for writing\nExiting...\n", filepath);
                std::exit(EXIT_FAILURE);
            }
            
            std::string buffer(size, '\0');
            file.read(buffer.data(), size);
            
            ::replace(code, matches[1], buffer);
            
        }
    }

    void preprocess_definition(Context& context, std::string& code) noexcept
    {
        std::regex rx_pdef;
        std::smatch matches;
    }

    void preprocess_macro_definition(Context& context, std::string& code) noexcept
    {
        std::regex rx_mcro(R"((\.macro ([a-zA-Z_][a-zA-Z0-0_]*) = \((([a-zA-Z])(, ?[a-zA-Z])*)?\)\:\s*\{([^\}]*)\}))");
        std::smatch matches;
        
        while (std::regex_search(code, matches, rx_mcro))
        {
            std::string name = matches[2];
            std::string args = matches[3];
            
            args.erase(std::remove_if(args.begin(), args.end(), ::isspace), args.end());
            auto args_delim = ::split(args, ',');
            
            std::string mcode = matches[6];
            
            Macro macro{ name, args_delim, mcode };
            
            bool found = false;
            for (const auto& m : context.defined_macros)
            {
                if (m.name == macro.name) [[unlikely]]
                {
                    found = true;
                    break;
                }
            }
            
            if (!found)
            {
                std::cerr << fmt::format("[Error] Macro {} is multiply defined\nExiting...\n", macro.name);
                std::exit(EXIT_FAILURE);
            }
            
            context.defined_macros.emplace_back(macro);
            
            ::replace(code, matches[1], "");
        }
    }

    void preprocess_macro_invokation(Context& context, std::string& code) noexcept
    {
        std::regex rx_invk(R"((\$([a-zA-Z_][a-zA-Z0-9_]*)\(([^\,\)]*\s*(\,[^\,\)]*)*)?\)))");
        std::smatch matches;
        
        while (std::regex_search(code, matches, rx_invk))
        {
            std::string name = matches[2];
            std::string args = matches[3];
            
            args.erase(std::remove_if(args.begin(), args.end(), ::isspace), args.end());
            auto args_delim = ::split(args, ',');
            
            Macro macro{ name, args_delim, "" }, defined_macro;
            
            bool found = false;
            for (auto&& m : context.defined_macros)
            {
                if (m.name == macro.name)
                {
                    defined_macro = m;
                    found = true;
                    break;
                }
            }
            
            if (!found)
            {
                std::cerr << fmt::format("[Error] Macro {} is undefined\nExiting...\n", macro.name);
                std::exit(EXIT_FAILURE);
            }
            
            if (defined_macro.args.size() != args_delim.size())
            {
                std::cerr << fmt::format("[Error] Invokation of macro {} had {} arguments, but expected {}\nExiting...\n", defined_macro.name, args_delim.size(), defined_macro.args.size());
                std::exit(EXIT_FAILURE);
            }
            
            if (args_delim.size() != 0)
            {
                std::string copy = defined_macro.code;
                
                // FIXME: implement this formatting thing properly (should remove extra new lines)
                
                /*if (copy[0] == '\n')
                
                if (copy[0] == '\n' &&
                    copy[copy.length()] == '\n')
                {
                    
                }*/
                
                for (auto i = 0u; i < args_delim.size(); ++i)
                {
                    ::replace(copy, std::string("[") + defined_macro.args[i] + std::string("]"), args_delim[i]);
                }
                
                ::replace(code, matches[1], copy);
            }
        }
    }
}


namespace ti
{
    void preprocess(std::string& code, std::string_view parent_filepath) noexcept
    {
        Context context{ {}, {} };
        
        ::preprocess_include         (context, code, parent_filepath);
        ::preprocess_definition      (context, code);
        ::preprocess_macro_definition(context, code);
        ::preprocess_macro_invokation(context, code);
    }
}
