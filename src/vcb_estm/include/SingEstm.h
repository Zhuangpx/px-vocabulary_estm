/*
 * @Autor: violet apricity ( Zhuangpx )
 * @Date: 2023-06-14 15:01:25
 * @LastEditors: violet apricity ( Zhuangpx )
 * @LastEditTime: 2023-06-20 00:09:48
 * @FilePath: \px-vocabulary_estm\src\vcb_estm\include\SingEstm.h
 * @Description:  Zhuangpx : Violet && Apricity:/ The warmth of the sun in the winter /
 */


#ifndef SINGESTM_H
#define SINGESTM_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <mysql/mysql.h>

namespace SE {

template<typename T>
T rand_mt19937(T l, T r) {
  static std::mt19937_64 gen(std::chrono::steady_clock::now().time_since_epoch().count());
  return std::uniform_int_distribution<T>(l, r)(gen);
}

int Lower_bound[] = { 0, 905, 2124, 4124, 4440, 5500, 6000, 8000,       };
int Upper_bound[] = {    905, 2124, 4124, 4440, 5500, 6000, 8000, 13000 };

//  词汇表和阶梯对应情况
std::vector<std::string> Vcb_list = {
  "Elem_vcb",
  "Midd_vcb",
  "High_vcb",
  "Coll_vcb",
  "Cet4_vcb",
  "Cet6_vcb",
  "Tem4_vcb",
  "Tem6_vcb"
};

std::vector<std::string> file_v = {
  "vocabulary/elem.data",
  "vocabulary/midd.data",
  "vocabulary/high.data",
  "vocabulary/coll.data",
  "vocabulary/cet4.data",
  "vocabulary/cet6.data",
  "vocabulary/tem4.data",
  "vocabulary/tem8.data"
};

std::vector<std::string> file_wordlist_v = {
  "wordlist/elem.wordlist",
  "wordlist/midd.wordlist",
  "wordlist/high.wordlist",
  "wordlist/coll.wordlist",
  "wordlist/cet4.wordlist",
  "wordlist/cet6.wordlist",
  "wordlist/tem4.wordlist",
  "wordlist/tem8.wordlist"
};

std::vector<std::string> name_v = {
  "elem",
  "midd",
  "high",
  "coll",
  "cet4",
  "cet6",
  "tem4",
  "tem8"
};

// #define Elem_vcb 0
// #define Midd_vcb 1
// #define High_vcb 2
// #define Coll_vcb 3
// #define Cet4_vcb 4
// #define Cet6_vcb 5
// #define Tem4_vcb 6
// #define Tem6_vcb 7

/*
小学词汇 0
初中词汇 1
高中词汇 2
大学词汇 3
四级词汇 4
六级词汇 5
专四词汇 6
专八词汇 7
*/

std::vector<std::string> Test_word_list[] = {
  std::vector<std::string> {"a", "aa", "aaa", "aaaa", "aaaaa"},
  std::vector<std::string> {"b", "bb", "bbb", "bbbb", "bbbbb"},
  std::vector<std::string> {"c", "cc", "ccc", "cccc", "ccccc"},
  std::vector<std::string> {"d", "dd", "ddd", "dddd", "ddddd"},
  std::vector<std::string> {"e", "ee", "eee", "eeee", "eeeee"},
  std::vector<std::string> {"f", "ff", "fff", "ffff", "fffff"},
  std::vector<std::string> {"g", "gg", "ggg", "gggg", "ggggg"},
  std::vector<std::string> {"h", "hh", "hhh", "hhhh", "hhhhh"}
};

//  单例估算对象 维护当前估算信息
class SingEstm
{
public:
  //  初始化
  void Init(std::string _name, int _cet4, int _cet6, std::string _vcb) {
    sum_ = 0;
    name_ = _name;
    cet4_ = _cet4;
    cet6_ = _cet6;
    vcb_ = _vcb;
    for(int i=0; i<8; i++) if(_vcb[i] == '1') vcb_[i] = (_vcb[i]=='1'), id_list_.push_back(i);
    id_ = id_list_[id_list_pos_ = (id_list_.size()>>1)];
    Update_word_list();
    Update_word();
  }
  //  估算词汇量结果
  long long Cal_vcb_estm() {
    long long ans = 0;
    for(auto idx : id_list_) {
      //  枚举词汇表/阶梯
      auto identify = identify_map_[idx];
      //  当前阶梯准确率
      double rate = 1.0 * identify.relize_sum_ / identify.relize_sum_;
      //  当前阶梯词汇量 = L + Rate * (R-L+1)
      long long vcb_size = Lower_bound[idx] + (long long)((Upper_bound[idx] - Lower_bound[idx]) * rate);
      //  当前阶梯权重
      double weight = 1.0 * identify.sum_ / sum_;
      //  阶梯词汇量*阶梯权值 加权平均
      ans += (long long)(vcb_size * weight);
    }
    return ans;
  }
  //  上升一个阶梯
  void Push_up() {
    if(id_list_pos_+1 == (int)id_list_.size()) {
      //  顶
      Update_word();
    } else {
      id_ = id_list_[++id_list_pos_];
      Update_word_list();
      Update_word();
    }
  }
  //  下降一个阶梯
  void Push_down() {
    if(id_list_pos_ == 0) {
      //  底
      Update_word();
    } else {
      id_ = id_list_[--id_list_pos_];
      Update_word_list();
      Update_word();
    }
  }
  //  更新单词列表
  void Update_word_list() {
    //  TODO
    //  在这里切换word_list 需要从词汇表里拿出来 具体从file还是数据库 待考究
    //  现在先随便拿点来测试
    // word_list_ = Test_word_list[id_];
    //  !方案一 数据库
    // //  !MySQL connect
    // MYSQL *mysql_conn = mysql_init(NULL);
    // // Connect to the MySQL database
    // if(!mysql_real_connect(mysql_conn, "localhost", "root", "xin051606", "VE_db", 3306, NULL, 0)) {
    //   std::cerr << "MySQL Connection Error: " << mysql_error(mysql_conn) << std::endl;
    //   exit(1);
    // }
    // std::string table_name = "table_" + name_v[id_];
    // std::string select_query = "select * from " + table_name + ";";
    // word_list_.clear();
    // // Execute the SELECT query
    // if(mysql_query(mysql_conn, select_query.c_str())) {
    //   std::cerr << "MySQL select Error: " << mysql_error(mysql_conn) << std::endl;
    //   exit(1);
    // }
    // // Get the result set
    // MYSQL_RES *result = mysql_store_result(mysql_conn);
    // if (!result) {
    //   std::cerr << "MySQL Store Result Error: " << mysql_error(mysql_conn) << std::endl;
    //   exit(1);
    // }
    // // Iterate over the rows and retrieve the data
    // MYSQL_ROW row;
    // while((row = mysql_fetch_row(result))) {
    //   // Assuming the table has a single column, add it to the vector
    //   std::string rowData = row[0];
    //   word_list_.push_back(rowData);
    // }
    // // Free the result set
    // mysql_free_result(result);
    //  !方案二 文件
    word_list_.clear();
    GB::file_read(file_wordlist_v[id_], word_list_);
  }
  //  更新单词
  void Update_word() {
    do {
      word_ = word_list_[rand_mt19937(0, std::max(0, (int)word_list_.size()-1))];
    } while(relize_set_.find(word_) != relize_set_.end() || unrelize_set_.find(word_) != unrelize_set_.end());
  }
  //  文件取出
  void Load_form_file(const std::string& pro_file_path, const std::string& info_file_path) {
    std::ifstream pro_file(pro_file_path);
    pro_file >> name_ >> cet4_ >> cet6_ >> vcb_;
    std::ifstream info_file(info_file_path);
    info_file >> sum_ >> id_ >> word_;
    int word_list_n = 0;
    info_file >> word_list_n;
    word_list_.resize(word_list_n);
    for(auto &i:word_list_) info_file >> i;
    int relize_set_n = 0;
    info_file >> relize_set_n;
    relize_set_.clear();
    while(relize_set_n--) {
      std::string temp;
      info_file >> temp;
      relize_set_[temp] = 1;
    }
    int unrelize_set_n = 0;
    info_file >> unrelize_set_n;
    unrelize_set_.clear();
    while(unrelize_set_n--) {
      std::string temp;
      info_file >> temp;
      relize_set_[temp] = 1;
    }
    int id_list_n = 0;
    info_file >> id_list_n;
    id_list_.resize(id_list_n);
    for(auto &i:id_list_) info_file >> i;
    info_file >> id_list_pos_;
    for(int i=0; i<8; i++) info_file >> identify_map_[i].sum_ >> identify_map_[i].relize_sum_;
  }
  //  文件载入
  void Load_into_file(const std::string& pro_file_path, const std::string& info_file_path) {
    // std::ofstream pro_file(pro_file_path,std::ios::out | std::ios::trunc);
    // pro_file << name_ << '\n' << cet4_ << '\n' << cet6_ << '\n' << vcb_ << '\n';
    std::ofstream info_file(info_file_path, std::ios::out |std::ios::trunc);
    info_file << sum_ << '\n' << id_ << '\n' << word_ << '\n';
    int word_list_n = word_list_.size();
    info_file << word_list_n << '\n' ;
    for(auto i:word_list_) info_file << i << '\n';
    int relize_set_n = relize_set_.size();
    info_file << relize_set_n << '\n';
    for(auto x:relize_set_) if(x.second) info_file << x.first << '\n';
    int unrelize_set_n = unrelize_set_.size();
    info_file << unrelize_set_n << '\n';
    for(auto x:unrelize_set_) if(x.second) info_file << x.first << '\n';
    int id_list_n = id_list_.size();
    info_file << id_list_n << '\n';
    for(auto i:id_list_) info_file << i << '\n';
    info_file << id_list_pos_ << '\n';
    for(int i=0; i<8; i++) info_file << identify_map_[i].sum_ << '\n' << identify_map_[i].relize_sum_ << '\n';
  }
  // 序列化函数
  void Serialize(const std::string& file_path) {
    std::ofstream file(file_path, std::ios::binary);
    if(file) {
      file.write(reinterpret_cast<char*>(this), sizeof(*this));
      file.close();
    } else {
      std::cerr << "Failed to open file while serializing" << std::endl;
    }
  }
  //  反序列化函数
  void Deserialize(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if(file) {
      file.read(reinterpret_cast<char*>(this), sizeof(*this));
      file.close();
    } else {
      std::cerr << "Failed to open file while deserializing" << std::endl;
    }
  }

  //  profile
  std::string name_, vcb_;
  int cet4_, cet6_;
  //  info
  int sum_;                                  //  测试过的单词数目
  int id_;                                   //  当前阶梯
  std::string word_;                         //  当前单词
  std::vector<std::string> word_list_;       //  当前可选单词列表
  std::map<std::string, bool> relize_set_;   //  已识别正确的单词
  std::map<std::string, bool> unrelize_set_; //  已识别错误的单词
  // std::vector<int> vcb_vis_;           //  选择的词汇表 和阶梯对应
  std::vector<int> id_list_; //  选择的词汇表 和阶梯对应
  int id_list_pos_;          //  目前在id_list的位置
  struct IndentifyMap {
    int sum_;                          //  此阶梯测试过的单词数目
    int relize_sum_;                   //  此阶梯识别的单词数目
  } identify_map_[8];

  // static SingEstm* Istance()  { return Sing_estm; }  //  单例模式

// private:
  SingEstm() {
    sum_ = id_ = id_list_pos_ = cet4_ = cet6_ = 0;
    word_ = "word_";
    name_ = "name_";
    vcb_ = "00000000";
    word_list_ = Test_word_list[0];
    id_list_.push_back(0);
    for(int i=0; i<8; i++) identify_map_[i].sum_ = identify_map_[i].relize_sum_ = 0;
  }
  ~SingEstm() = default;
  SingEstm(const SingEstm &_sing_estm) = default;
  const SingEstm &operator= (const SingEstm &_sing_estm) = delete;
  //  单例对象指针
  // static SingEstm *Sing_estm;  //  单例模式
};

// SingEstm* SingEstm::Sing_estm = new (std::nothrow) SingEstm();  //  单例模式

}

#endif
