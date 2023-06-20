/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-16 22:35:43
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-17 12:03:29
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\cgi\cgi-bin\single_estm_result.cpp
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */
#include <iostream>
#include <fstream>
#include <string>
#include "../include/cgi_header.h"
#include "../include/global.h"

int main()
{
  std::cerr << "result" << std::endl;
	std::ifstream file("static/single_estm_result.html"), pro_file("data/single_estm_profile.txt"), vcb_file("data/single_estm_vcb_size.txt");
	if(!file || !pro_file) {
		std::cerr << "Failed to open file." << std::endl;
	}
  // std::string Name, Cet_4 = 0, Cet_6 = 0, Vcb, Vcb_size;
  std::string Name, Cet_4, Cet_6, Vcb, Vcb_size;
  pro_file >> Name >> Cet_4 >> Cet_6 >> Vcb;
  if(Cet_4[0] == '-') Cet_4 = "无";
  if(Cet_6[0] == '-') Cet_6 = "无";
  vcb_file >> Vcb_size;
  std::cerr << Vcb_size << ' ' << Name << ' ' << Cet_4 << ' ' << Cet_6 << std::endl;
	std::string html_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close(); pro_file.close(); vcb_file.close();
  std::string modified_html = GB::replacePlaceholder(html_content, "{Vcb_size}", Vcb_size);
	modified_html = GB::replacePlaceholder(modified_html, "{Name}", Name);
	modified_html = GB::replacePlaceholder(modified_html, "{Cet_4}", Cet_4);
	modified_html = GB::replacePlaceholder(modified_html, "{Cet_6}", Cet_6);
  std::string temp = "{Check_op#}", __temp;
  for(int i=0; i<7; i++) {
    temp[9] = char(i+'0');
    __temp = (Vcb[i]=='1'? "checked" : "unckecked");
    modified_html = GB::replacePlaceholder(modified_html, temp, __temp);
  }
	std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n" << std::endl;
	std::cout << modified_html << std::endl;
  return 0;
}
