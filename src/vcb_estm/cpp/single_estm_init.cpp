/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-14 14:59:40
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-19 23:36:53
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\cpp\single_estm_init.cpp
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */
#include <iostream>
#include "../include/cgi_header.h"
#include "../include/global.h"

int main()
{
  // GB::SingEstm sing_estm; sing_estm.Serialize("SingEstm_data.bin");
  //  cgicc
  cgicc::Cgicc form_data;
  cgicc::form_iterator fite;
  std::string temp, Name, Vcb = "00000000";
  int Cet_4 = 0, Cet_6 = 0;
  //  Name Cet_4 Cet_6 Vcb
  Name = **form_data.getElement("Name");
  temp = **form_data.getElement("Cet_4");
  auto str_to_int = [](std::string s) {
    int res = 0;
    for(auto i:s) res = res*10 + (i-'0');
    return res;
  };
  if(temp.size() == 0) Cet_4 = -1;
  else Cet_4 = str_to_int(temp);
  temp = **form_data.getElement("Cet_6");
  if(temp.size() == 0) Cet_6 = -1;
  else Cet_6 = str_to_int(temp);
  if(**form_data.getElement("Check_box_1") == "Elem_vcb") Vcb[0] = '1';
  if(**form_data.getElement("Check_box_2") == "Midd_vcb") Vcb[1] = '1';
  if(**form_data.getElement("Check_box_3") == "High_vcb") Vcb[2] = '1';
  if(**form_data.getElement("Check_box_4") == "Coll_vcb") Vcb[3] = '1';
  if(**form_data.getElement("Check_box_5") == "Cet4_vcb") Vcb[4] = '1';
  if(**form_data.getElement("Check_box_6") == "Cet6_vcb") Vcb[5] = '1';
  if(**form_data.getElement("Check_box_7") == "Tem4_vcb") Vcb[6] = '1';
  if(**form_data.getElement("Check_box_8") == "Tem6_vcb") Vcb[7] = '1';
  //  data
  GB::file_rewrite("data/single_estm_profile.txt", Name+"\n");
  GB::file_write_app("data/single_estm_profile.txt", Cet_4);
  GB::file_write_app("data/single_estm_profile.txt", "\n");
  GB::file_write_app("data/single_estm_profile.txt", Cet_6);
  GB::file_write_app("data/single_estm_profile.txt", "\n");
  GB::file_write_app("data/single_estm_profile.txt", Vcb+"\n");
  int single_estm_cnt_now = 1;
  int single_estm_cnt_tot = 50;
	GB::file_rewrite("data/single_estm_cnt_now.txt", single_estm_cnt_now);
	GB::file_rewrite("data/single_estm_cnt_tot.txt", single_estm_cnt_tot);
  std::cerr << "init: " << Name << ' ' << Cet_4 << ' ' << Cet_6 << ' ' << Vcb << std::endl;
    //  html
  std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n";
  // std::cout << "<!DOCTYPE html>\n";
  // std::cout << "<html>\n";
  // std::cout << "<head>\n";
  // std::cout << "<title>词汇估算工具</title>\n";
  std::ifstream file("static/back.html");
	if(!file) {
		std::cerr << "Failed to open HTML file." << std::endl;
		return 1;
	}
	std::string htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
  std::cout << htmlContent << std::endl;
  // std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n";
  std::cout << "<!DOCTYPE html>\n";
  std::cout << "<html>\n";
  std::cout << "<head>\n";
  std::cout << "<title>词汇估算工具</title>\n";
  std::cout << "<p>正在为你准备数据 请稍等片刻</p>";
  std::cout << "<script>window.location.href = \"../cgi-bin/single_estm.cgi\";</script>\n";
  std::cout << "</head>\n";
  std::cout << "<body>\n";
  return 0;
}
