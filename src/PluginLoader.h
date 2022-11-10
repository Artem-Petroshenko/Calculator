#pragma once
#include <iostream>
#include <map>
#include <Windows.h>
#include <filesystem>

using mapFunction = std::map<std::string, double(*)(double)>;

class PluginLoader {
public:

    PluginLoader() { loadDll(); }

    double executeFunction(double const& a, std::string const& funcName);

    mapFunction getFunction() { return Functions; };

private:
    std::string pluginsPath = "../../../dll/plugins/";

    mapFunction Functions;

    void loadDll();
    void loadFunc(std::string const& fileName);
};