// laba4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Threadpool.h"
#include <iostream>
#include <locale>
#include <windows.h>
#include <fstream>
#include <thread>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <list>

#define THREADCOUNT 10
std::vector<std::string> strings;
std::vector<std::list<std::string>> parts;
#define MAXSTRING "ZZZZZZZZZZZ"
void LoadFile(std::string filename)
{
    std::ifstream reader(filename.c_str());
    if (!reader.is_open()) {
        std::cout << "error opening file" << std::endl;
    }
    else {
        std::string buf;
        while (getline(reader, buf,'\n'))
        {
            strings.push_back(buf);
        }
        reader.close();
    }
}
void WriteStrFile(std::string filename)
{
    std::ifstream ostr(filename);
    if (!ostr.good()) {
        std::cout << "error writing to file" << std::endl;
    }
    else
    {
        std::ofstream stream(filename);
        for (std::string str : strings) {
            char* cstr = str.c_str();
            strcat(cstr, "\n")
            stream.write(str.c_str(), strlen(str.c_str()+1));
        }
        stream.close();
    }
}
void Split(float count)
{
    int size = ceil(strings.size() / count);
    for (int i = 0;i < strings.size();i ++)
    {
        parts[ceil(i / size)].push_back(strings[i]);
    }
}
bool comparator(std::pair<std::string, int> first, std::pair<std::string, int> second) { return (first.first.compare(second.first)<0); }
void MergeSortedParts()
{
    strings.clear();
    //std::list<std::string> strings;
    std::vector<std::pair<std::string, int>> buffer;
    for (int i = 0;i < THREADCOUNT;i++)
    {
        std::pair<std::string, int> tmp(parts[i].front(),i);
        buffer.push_back(tmp);
        parts[i].erase(parts[i].begin());
    }
    std::sort(buffer.begin(), buffer.end(),comparator);
    while (buffer[0].first.compare(MAXSTRING) < 0)
    {
        strings.push_back(buffer[0].first);
        int num = buffer[0].second;
        buffer.erase(buffer.begin());
        if (!parts[num].empty())
        {
            std::pair<std::string, int> tmp(parts[num].front(), num);
            buffer.push_back(tmp);
            parts[num].erase(parts[num].begin());
        }
        else
        {
            std::pair<std::string, int> tmp(MAXSTRING, num);
            buffer.push_back(tmp);
        }
        std::sort(buffer.begin(), buffer.end(), comparator);
    }
}
int main()
{
    LoadFile("D:\\University\\3course\\5semester\\OSiSP\\labs\\laba4\\Debug\\textfile.txt");
    parts.resize(THREADCOUNT);
    Split(THREADCOUNT);
    Threadpool* pool = new Threadpool(THREADCOUNT);
    HANDLE threadpoolfinish = pool->GetFinishHandle();
    for (int i = 0;i < THREADCOUNT;i++) {
        pool->QueueUserWorkItem([i]()
            {
                parts[i].sort();
            });
    }
    pool->Stop();
    WaitForSingleObject(threadpoolfinish, INFINITE);
    MergeSortedParts();
    WriteStrFile("D:\\University\\3course\\5semester\\OSiSP\\labs\\laba4\\Debug\\output.txt");
    std::cout << "Main thread " << GetCurrentThreadId() << " stops " << std::endl;
    system("pause");

}
