/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-12 23:33:37
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-19 23:51:05
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\cpp\single_estm.cpp
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */

#include <iostream>
#include <fstream>
#include <string>
#include "../include/cgi_header.h"
#include "../include/global.h"
#include "../include/SingEstm.h"

int main()
{
	SE::SingEstm sing_estm;
	cgicc::Cgicc form_data;
	int single_estm_cnt_tot = 0, single_estm_cnt_now = 0;
	GB::file_read("data/single_estm_cnt_tot.txt", single_estm_cnt_tot);
	GB::file_read("data/single_estm_cnt_now.txt", single_estm_cnt_now);
	bool end_fl = (single_estm_cnt_now == single_estm_cnt_tot);

	//	动态强制类型转换   //  单例模式
	// GB::SingEstm &sing_estm = dynamic_cast<GB::SingEstm&>(*GB::SingEstm::Istance());

	if(single_estm_cnt_now == 1) {
		//	首次 初始化
		std::ifstream pro_file("data/single_estm_profile.txt");
		std::string name, vcb;
		int cet4, cet6;
		pro_file >> name >> cet4 >> cet6 >> vcb;
		std::cerr << "Inin info: " << name << ' ' << cet4 << ' ' << cet6 << ' ' << vcb << std::endl;
		sing_estm.Init(name, cet4, cet6, vcb);
		std::cerr << "Init finish" << std::endl;
		// sing_estm.Serialize("SingEstm_data.bin");
		sing_estm.Load_into_file("data/single_estm_profile.txt", "data/single_estm_info.txt");
	} else {
		//	非首次 需要处理
		// sing_estm.Deserialize("SingEstm_data.bin");
		sing_estm.Load_form_file("data/single_estm_profile.txt", "data/single_estm_info.txt");
		sing_estm.sum_++;
		if(**form_data.getElement("Word_op") == "Yes") {
			// 认识
			auto &identify = sing_estm.identify_map_[sing_estm.id_];
			identify.relize_sum_++;
			identify.sum_++;
			// if(sing_estm.sum_ == 50) End();	//	答完了 结束了
			// sing_estm.relize_set_.insert(sing_estm.word_);
			sing_estm.relize_set_[sing_estm.word_] = 1;
			// 计算当前识别率
			double rate = 1.0 * identify.relize_sum_ / identify.sum_;
			if(rate >= 0.8 && identify.sum_ >= 5) {
				sing_estm.Push_up();
			} else {
				sing_estm.Update_word();
			}
		} else {
			//	不认识
			auto &identify = sing_estm.identify_map_[sing_estm.id_];
			identify.sum_++;
			// if(sing_estm.sum_ == 50) End();	//	答完了 结束了
			// sing_estm.unrelize_set_.insert(sing_estm.word_);
			sing_estm.unrelize_set_[sing_estm.word_] = 1;
			// 计算当前识别率
			double rate = 1.0 * identify.relize_sum_ / identify.sum_;
			if(rate <= 0.2 && identify.sum_ >= 5) {
				sing_estm.Push_down();
			} else {
				sing_estm.Update_word();
			}
		}
		// sing_estm.Serialize("SingEstm_data.bin");
		sing_estm.Load_into_file("data/single_estm_profile.txt", "data/single_estm_info.txt");
	}


	std::cerr << "sing_estm.sum_:" << sing_estm.sum_ << std::endl;
	std::cerr << "single_estm_cnt_now:" << single_estm_cnt_now << std::endl;

	if(single_estm_cnt_now > single_estm_cnt_tot) {
		//	结束了 此时是从结束点过来的
		// sing_estm.sum_++;
		// auto &identify = sing_estm.identify_map_[sing_estm.id_];
		// identify.sum_++;
		// identify.relize_sum_ += (int)(**form_data.getElement("Word_op") == "Yes");
		int Vcb_size = sing_estm.Cal_vcb_estm();
		GB::file_rewrite("data/single_estm_vcb_size.txt", Vcb_size);
		std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n";
		std::ifstream file("static/back.html");
		if(!file) {
			std::cerr << "Failed to open HTML file." << std::endl;
			return 1;
		}
		std::string htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		std::cout << htmlContent << std::endl;
		std::cout << "<!DOCTYPE html>\n";
		std::cout << "<html>\n";
		std::cout << "<head>\n";
		std::cout << "<title>词汇估算工具</title>\n";
		std::cout << "<p>正在为你估算结果 请稍等片刻</p>";
		// std::cout << "<script>window.location.href = \"./single_estm_result.cgi\";</script>\n";
		std::cout << "<script>window.setTimeout(function() { window.location.href = 'single_estm_result.cgi'; }, 2000);</script>";
		std::cout << "</head>\n";
		std::cout << "<body>\n";
		return 0;
	}




	//	html
	int Cnt_tot = single_estm_cnt_tot, Cnt_now = single_estm_cnt_now;
	std::ifstream file("static/single_estm.html");
	if(!file) {
		std::cerr << "Failed to open HTML file." << std::endl;
		return 1;
	}
	std::string htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	std::string Word = sing_estm.word_;
	// std::string Word = "测试";
	std::string Submit_op = end_fl? "结束" : "下一题";
	std::string modifiedHtml = GB::replacePlaceholder(htmlContent, "{Word}", Word);
	modifiedHtml = GB::replacePlaceholder(modifiedHtml, "{Submit_op}", Submit_op);
	modifiedHtml = GB::replacePlaceholder(modifiedHtml, "{Cnt_now}", std::to_string(Cnt_now));
	modifiedHtml = GB::replacePlaceholder(modifiedHtml, "{Cnt_tot}", std::to_string(Cnt_tot));
	std::cout << "Content-type:text/html;charset=utf-8\r\n\r\n" << std::endl;
	std::cout << modifiedHtml << std::endl;
	single_estm_cnt_now++;
	GB::file_rewrite("data/single_estm_cnt_now.txt", single_estm_cnt_now);

	return 0;
}
