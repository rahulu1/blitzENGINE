//
//  Utilities.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/29/24.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <string>

#include "document.h"
#include "filereadstream.h"
#include "lua.hpp"
#include "sol/sol.hpp"

static void ReadJsonFile(const std::string& path, rapidjson::Document & out_document)
{
    FILE* file_pointer = nullptr;
#ifdef _WIN32
    fopen_s(&file_pointer, path.c_str(), "rb");
#else
    file_pointer = fopen(path.c_str(), "rb");
#endif
    char buffer[65536];
    rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
    out_document.ParseStream(stream);
    std::fclose(file_pointer);

    if (out_document.HasParseError())
    {
        rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
        std::cout << "error parsing json at [" << path << "]";
        exit(0);
    }
}

static const void ErrorExit(const std::string &error_message)
{
    std::cout << error_message;
    std::exit(EXIT_SUCCESS);
}

static const void ReportError(const std::string &actor_name, const sol::error &e)
{
    std::string error_message = e.what();
    
    /* Normalize file paths across platforms */
    std::replace(error_message.begin(), error_message.end(), '\\', '/');
    
    /* Display (with color codes) */
    std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}

#endif /* Utilities_hpp */
