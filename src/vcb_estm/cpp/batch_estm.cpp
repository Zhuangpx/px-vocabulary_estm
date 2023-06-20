/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-19 20:13:54
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-20 00:25:00
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\cpp\batch_estm.cpp
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */
#include <iostream>
#include "../include/cgi_header.h"
#include "../include/global.h"
#include "../include/SingEstm.h"

int main()
{
  std::map<std::string, bool> mp;
  //  !get Batch_file
  cgicc::Cgicc form_data;
  cgicc::const_file_iterator fite = form_data.getFile("Batch_file");
  if(fite != form_data.getFiles().end()) {
    const std::string& file_name = fite->getFilename();
    std::ifstream file(file_name);
    if(file.is_open()) {
      std::string str;
      while(file >> str) mp[str] = 1;
      file.close();
    } else {
      std::cerr << "Error: Failed to open the Batch_file" << std::endl;
    }
  } else {
    std::cerr << "Error: Failed to retrieve the file" << std::endl;
  }
  int real_vcb = mp.size();
  long long estm_vcb = 0;
  //  !estm
  SE::SingEstm sing_estm;
  sing_estm.Init("batch", -1, -1, "11111111");
  for(int i=1; i<=100; i++) {
    sing_estm.sum_++;
    std::string word = sing_estm.word_;
    if(mp.count(word) != 0) {
      //  认识
      auto &identify = sing_estm.identify_map_[sing_estm.id_];
			identify.relize_sum_++;
			identify.sum_++;
			sing_estm.relize_set_[sing_estm.word_] = 1;
			// 计算当前识别率
			double rate = 1.0 * identify.relize_sum_ / identify.sum_;
			if(rate >= 0.8 && identify.sum_ >= 5) {
				sing_estm.Push_up();
			} else {
				sing_estm.Update_word();
			}
    } else {
      //  不认识
      auto &identify = sing_estm.identify_map_[sing_estm.id_];
			identify.sum_++;
			sing_estm.unrelize_set_[sing_estm.word_] = 1;
			// 计算当前识别率
			double rate = 1.0 * identify.relize_sum_ / identify.sum_;
			if(rate <= 0.2 && identify.sum_ >= 5) {
				sing_estm.Push_down();
			} else {
				sing_estm.Update_word();
			}
    }
  }
  estm_vcb = sing_estm.Cal_vcb_estm();
  //  !result
  std::ifstream file("static/batch_estm_result.html");
  if(!file) {
		std::cerr << "Failed to open file." << std::endl;
    return 1;
	}
  std::string html_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
  std::string str_real_vcb = std::to_string(real_vcb);
  std::string str_estm_vcb = std::to_string(estm_vcb);
  std::string modified_html = GB::replacePlaceholder(html_content, "{Barch_real_vcb}", str_real_vcb);
	modified_html = GB::replacePlaceholder(modified_html, "{Barch_estm_vcb}", str_estm_vcb);
	std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n" << std::endl;
	std::cout << modified_html << std::endl;
}
