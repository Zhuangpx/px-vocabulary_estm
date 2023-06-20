/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-14 15:01:25
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-19 14:14:24
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\include\global.h
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */


#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
namespace GB {

template<typename T>
T rand_mt19937(T l, T r) {
  static std::mt19937_64 gen(std::chrono::steady_clock::now().time_since_epoch().count());
  return std::uniform_int_distribution<T>(l, r)(gen);
}

//  单次写文件 追加
template<typename Ty>
void file_write_app(std::string _file_path, Ty _data)
{
  std::ofstream file(_file_path, std::ios::app);
  if(file.is_open()) {
    file << _data;
    file.close();
  } else {
    std::cerr << "file open error" << std::endl;
  }
}

//  单次写文件 清空
template<typename Ty>
void file_rewrite(std::string _file_path, Ty _data)
{
  std::ofstream file(_file_path, std::ios::trunc);
  if(file.is_open()) {
    file << _data;
    file.close();
  } else {
    std::cerr << "file open error" << std::endl;
  }
}

//  单次读文件
template<typename Ty>
void file_read(std::string _file_path, Ty& _data)
{
  std::ifstream file(_file_path);
  if(file.is_open()) {
    file >> _data;
    file.close();
  } else {
    std::cerr << "file open error : " << _file_path << std::endl;
  }
}

//  完整读文件
template<typename Ty>
void file_read(std::string _file_path, std::vector<Ty>& _data)
{
  std::ifstream file(_file_path);
  if(file.is_open()) {
    Ty temp;
    while(file >> temp) _data.push_back(temp);
    file.close();
  } else {
    std::cerr << "file open error" << std::endl;
  }
}

//  string 内容替换
std::string replacePlaceholder(const std::string& html, const std::string placeholder, const std::string value) {
	std::string result = html;
	size_t pos = result.find(placeholder);
	while (pos != std::string::npos) {
		result.replace(pos, placeholder.length(), value);
		pos = result.find(placeholder, pos + value.length());
	}
	return result;
}

}

#endif
