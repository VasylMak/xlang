#pragma once

#include <assert.h>
#include <string_view>
#include <vector>
#include "meta_reader.h"
#include "base_model.h"

namespace xlang::xmeta
{
 /*   void print_metadata(xlang::meta::reader::cache const& cache)
    {
        for (auto const& db : cache.databases())
        {
            std::cout << "TypeDefs ------------------ " << std::endl;
            for (auto const& type_def : db.TypeDef)
            {
                std::cout << type_def.TypeNamespace() << "." << type_def.TypeName() << std::endl;
                if (type_def.Extends())
                {
                    std::cout << "  extends: " << type_def.Extends().TypeRef().TypeName() << std::endl;
                    std::cout << "           " << type_def.is_struct() << std::endl;
                }
                if (size(type_def.MethodList()) > 0)
                {
                    for (auto const& method : type_def.MethodList())
                    {
                        std::cout << "       " << method.Name() << std::endl;
                    }
                }
            }
            std::cout << "TypeRefs ------------------ " << std::endl;
            for (auto const& type_ref : db.TypeRef)
            {
                std::cout << type_ref.TypeNamespace() << "." << type_ref.TypeName() << std::endl;
            }
        }
    }*/

    //std::string_view remove_path(std::string_view const filename) {
    //    size_t lastdot = filename.find_last_of("\\");
    //    if (lastdot == std::string::npos) return filename;
    //    return filename.substr(lastdot + 1, filename.length());
    //}

    struct symbol_table
    {
        symbol_table() = delete;
        symbol_table(std::vector<std::string> const& path) : cache{ path }
        { }

        std::map<std::string, class_type_semantics> table;
        std::map<std::string_view, type_ref> imported_type_refs;
        std::map<std::string_view, xlang::meta::reader::cache> metadata;
        xlang::meta::reader::cache cache;

        bool set_symbol(std::string_view symbol, class_type_semantics const& class_type)
        {
            return table.insert(std::pair<std::string, class_type_semantics>(symbol, class_type)).second;
        }

        bool set_imported_type_ref(std::string_view symbol)
        {
            return imported_type_refs.emplace(symbol, type_ref{ symbol }).second;
        }

        class_type_semantics get_symbol(std::string symbol)
        {
            auto iter = table.find(symbol);
            if (iter == table.end())
            {
                return std::monostate();
            }
            return iter->second;
        }
    };


    struct compilation_unit
    {
        compilation_unit() = delete;
        compilation_unit(std::string_view const& idl_assembly_name, std::vector<std::string> const& path) : m_assembly{ idl_assembly_name }, symbols{ path }
        { }

        std::map<std::string_view, std::shared_ptr<namespace_model>, std::less<>> namespaces;
        std::string m_assembly;
        std::vector<std::string> m_imported_assembly_names;
        symbol_table symbols;
    };
}
