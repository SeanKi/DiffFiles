
/**
@file   fs_utils.h
@author Sergey Shkvyra <sr1919@gmail.com>
@brief  wrapper for std::filesystem, dependeds on compiler (msvc toolset)

28/01/21
*/

#pragma once
#include <filesystem>
#ifndef INLINE_CONSOLE_H
#include <iostream>
#endif

//  determine filesystem namespace dependends on compiler
#ifdef _FSPFX
namespace  fs = std::experimental::filesystem;      //  toolset v140, v141
#else
    #if !defined(_HAS_CXX17) || _HAS_CXX17 == 0
        #error "Error: in order to use <filesystem> with v142 toolset set compiler flag /std:c++17"
    #endif
    namespace  fs = std::filesystem;                    //  toolset v142
#endif // _FSPFX

namespace fs_utils {
inline static std::string const unit{"[fsu]"};

//  check if folder exists and returns it,
//  otherwise returns empty string (if not exists)
inline auto check_and_set(std::string const &fn, bool const &verbose_ = false)
{
    if (fn.empty() || !fs::exists(fn)) {
        if (verbose_)
#ifdef INLINE_CONSOLE_H
            logger::con("file " + fn + " not found");
#else
                std::cout << "file " << fn << " not found\n";
#endif // INLINE_CONSOLE_H
        return std::string();
    }
    return fn;
};

inline bool copy_file(const std::string &who, const std::string &from, const std::string &to)
{
    auto rc = false;
    try {
        if (!fs::exists(from))
#ifdef INLINE_CONSOLE_H
            logger::warn(who + from + " not found.");
#else
                std::cout << who << from << " not found. skipped" << '\n';
#endif // INLINE_CONSOLE_H

        fs::copy_file(from, to, fs::copy_options::overwrite_existing);
#ifdef INLINE_CONSOLE_H
        logger::conok(who + from + " -> " + to);
#else
            std::cout << who << from << " -> " << to << '\n';
#endif // INLINE_CONSOLE_H
        rc = true;
    } catch (fs::filesystem_error const &e) {
#ifdef INLINE_CONSOLE_H
        logger::err(who + "Could not copy: " + from + e.what());
#else
            std::cout << who << "Could not copy: " << from << e.what() << '\n';
#endif // INLINE_CONSOLE_H
    }
    return rc;
};

//  returns true if folder <dir> exists
//  or was not existed, but successfully created
inline bool check_n_create(std::string const &dir)
{
    auto rc{true};
    auto const who(unit + " check_n_create: ");
    std::error_code ec{};
    if (!fs::exists(dir, ec)) {
        rc = false;
        auto msg = who + dir + " does not exists";
#ifdef INLINE_CONSOLE_H
            logger::warn(msg);
#else
            std::cout << msg << '\n';
#endif // INLINE_CONSOLE_H
        try {
            auto const ok = fs::create_directories(dir, ec);
            if (!ok && !fs::exists(dir, ec)) {
                msg = who + "can not create folder " + dir + ", system error: " + ec.message();
#ifdef INLINE_CONSOLE_H
            logger::warn(msg);
#else
            std::cout << msg << '\n';
#endif // INLINE_CONSOLE_H
            } else {
                msg = who + dir + " created";
#ifdef INLINE_CONSOLE_H
                logger::con(msg);
#else
                std::cout << msg << '\n';
#endif // INLINE_CONSOLE_H
                rc = true;
            }
        } catch (const std::exception &e) {
            msg = who + "init: can not create folder " + dir + ", exception: " + e.what();
#ifdef INLINE_CONSOLE_H
            logger::warn(msg);
#else
            std::cout << msg << '\n';
#endif // INLINE_CONSOLE_H
        }
    }
    return rc;
}
    inline bool create_directory_ex(CString path, int isDir0Path1 = 0) {
        bool bret = true;
        if (isDir0Path1 == 1) {
            CString sDirName;
            CString sFilename;
            CString sExt;
            // filename rule can include folder- so it needs to make directory from final path.
            SplitFullFilename(path, sDirName, sFilename, sExt);
            path = sDirName;
        }
        const std::wstring& wdir = path.operator LPCWSTR();
        if (!fs::is_directory(wdir)) {
            bret = fs::create_directories(wdir);
        }
        return bret; // only false when create_directories return false
    }
} // namespace fs_utils
