/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-12 21:33:02
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-19 23:34:48
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\cpp\index.cpp
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */

#include <iostream>
#include "../include/cgi_header.h"
#include "../include/global.h"

int main()
{
  //  cgicc
  cgicc::Cgicc form_data;
  cgicc::form_iterator fite = form_data.getElement("estm_op");

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

  if(fite->isEmpty() || fite==(*form_data).end()) {
    //  没选 等一会然后返回
    std::cout << "<p>操作有误：至少选择一项</p>";
    std::cout << "<p>2秒后返回首页</p>";
    std::cout << "<meta http-equiv=\"refresh\" content=\"2; URL=../static/index.html\">\n";
  } else if(**fite == "Single") {
    //  单测
    std::cout << "<p>单测</p>";
    std::cout << "<p>做好准备 2秒后开始估算你的词汇量</p>";
    std::cout << "<meta http-equiv=\"refresh\" content=\"2; URL=../static/estm_profile.html\">\n";
    // std::cout << "<script>window.setTimeout(function() { window.location.href = 'single_estm.cgi'; }, 2000);</script>";
  } else if(**fite == "Batch") {
    //  批测
    std::cout << "<p>批测</p>";
    std::cout << "<p>2秒开始批处理测试</p>";
    std::cout << "<meta http-equiv=\"refresh\" content=\"2; URL=../static/batch_estm.html\">\n";
  } else {
    //  未知错误
    std::cout << "<p>未知错误</p>";
    std::cout << "<meta http-equiv=\"refresh\" content=\"2; URL=../static/error.html\">\n";
  }
  std::cout << "</head>\n";
  std::cout << "<body>\n";
  return 0;
}
