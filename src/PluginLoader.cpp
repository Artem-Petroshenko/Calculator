#include "PluginLoader.h"

using Function = double(*)(double);

void PluginLoader::loadDll() {
    WIN32_FIND_DATAA wfd;
    HANDLE const hFind = FindFirstFileA((pluginsPath + std::string("*.dll")).c_str(), &wfd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string dllFileName(wfd.cFileName);
            loadFunc(dllFileName);
        } while (NULL != FindNextFileA(hFind, &wfd));
        FindClose(hFind);
    }
    else
    {
        throw std::exception("invalid plugin's path");
    }
}

void PluginLoader::loadFunc(std::string const& fileName) {
    HMODULE hm = LoadLibraryA((pluginsPath + fileName).c_str());
    if (hm == nullptr) {
        throw std::exception("dll not found");
    }

    std::string funcName = fileName.substr(0, fileName.find(".dll"));
    Function func = (Function)GetProcAddress(hm, ("My" + funcName).c_str());

    if (func == nullptr) {
        throw std::exception("function not found");     
    }
    else {
        Functions.insert(std::pair<std::string, Function>("My" + funcName, func));
    }
}

double PluginLoader::executeFunction(double const& a, std::string const& funcName) {
    return Functions[funcName](a);
}