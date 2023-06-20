#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mysql/mysql.h>


std::vector<std::string> read_file(const std::string &filename)
{
	std::vector<std::string> strings;
	std::ifstream file(filename);
	if(file.is_open()) {
		std::string line;
		while(file >> line) strings.push_back(line);
		file.close();
	}
	return strings;
}

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

int main()
{
  //  !MySQL connect
  MYSQL *mysql_conn = mysql_init(NULL);
  // Connect to the MySQL database
  if(!mysql_real_connect(mysql_conn, "localhost", "name", "password", "VE_db", 3306, NULL, 0)) {
    std::cerr << "MySQL Connection Error: " << mysql_error(mysql_conn) << std::endl;
    return 1;
  }
  std::cerr << "MySQL Connect to mysql: localhost name password VE_test 3306 NULL, 0" << std::endl;
  //  !MySQL create table & insert data
  for(std::size_t i=0; i<file_v.size(); i++) {
    std::string table_name = "table_" + name_v[i];
    std::vector<std::string> strs = read_file(file_v[i]);
    //  creat table table_i+1
    std::string create_query = "CREATE TABLE " + table_name + " (word VARCHAR(50)) engine=InnoDB;";
    if(mysql_query(mysql_conn, create_query.c_str())) {
      std::cerr << "MySQL Table Creation Error: " << mysql_error(mysql_conn) << std::endl;
      return 1;
    }
    //  Insert into table_i+1
    std::cerr << "x:" << strs[0] << std::endl;
    for(auto i:strs[0]) std::cerr << "i:" << i << std::endl;
    for(auto &x:strs) {
      std::string insert_query = "INSERT INTO " + table_name + " VALUES ('" + x + "');";
      // std::cerr << insert_query << std::endl;
      if(mysql_query(mysql_conn, insert_query.c_str())) {
        std::cerr << "MySQL Insertion Error: " << mysql_error(mysql_conn) << std::endl;
        return 1;
      }
    }
  }
  //  !MySQL set_op on table (high level weight removal)
  for(std::size_t i=1; i<name_v.size(); i++) {
    for(std::size_t j=0; j<i; j++) {
      // j < i
      std::string table_i = "table_" + name_v[i], table_j = "table_" + name_v[j];
      std::string op_query = "DELETE FROM " + table_i + " WHERE word IN ( SELECT word FROM " + table_j +" );" ;
      if(mysql_query(mysql_conn, op_query.c_str())) {
        std::cerr << "MySQL Deletion Error: " << mysql_error(mysql_conn) << std::endl;
        return 1;
      }
    }
  }
  for(std::size_t i=0; i<name_v.size(); i++) {
    std::cerr << name_v[i] << std::endl;
    int cnt = 0;
    std::string table_name = "table_" + name_v[i];
    std::string select_query = "select * from " + table_name + ";";
    std::vector<std::string> word_list;
    // Execute the SELECT query
    if(mysql_query(mysql_conn, select_query.c_str())) {
      std::cerr << "MySQL select Error: " << mysql_error(mysql_conn) << std::endl;
      exit(1);
    }
    // Get the result set
    MYSQL_RES *result = mysql_store_result(mysql_conn);
    if (!result) {
      std::cerr << "MySQL Store Result Error: " << mysql_error(mysql_conn) << std::endl;
      exit(1);
    }
    // Iterate over the rows and retrieve the data
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result))) {
      // Assuming the table has a single column, add it to the vector
      std::string rowData = row[0];
      word_list.push_back(rowData);
    }
    // Free the result set
    mysql_free_result(result);
    std::string file_name = "wordlist/" + name_v[i] + ".wordlist";
    std::ofstream file(file_name);
    if(file.is_open()) {
      for(const std::string &word : word_list) {
        file << word << "\n";
        cnt++;
      }
      file.close();
      std::cout << "Content written to file_wordlist successfully." << std::endl;
    }
    else {
      std::cerr << "Unable to open a.txt for writing." << std::endl;
    }
    std::cerr << cnt << std::endl;
  }
  mysql_close(mysql_conn);
  return 0;
}
