#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

enum priv { SuperUser, Admin, NormalUser };

struct User {
  string userName;
  string password;
  priv identity;
};

struct Record {
  string InvoiceNo;
  string StockCode;
  string Quantity;
  string UnitPrice;
  string CustomerID;
  string Country;
  bool deleted = false;
  bool unselected = false;
};

struct Table {
  string table_name;
  int version;
  bool locked;
  Record* title;
  vector<Record*> records;
};

map<string, User*> user_list;
User* cur_user;

string error_msg;
bool error_hap;

Table* t = new Table{"E-Commerce", 1, false, new Record{}};
Record* temp_record;
string key[10], value[10], temp_value[10];

int cur, changed;
vector<Record*> rset, temp;

long long temp_time;

void filter_noindex() {
  cur = 0;
  while (true) {
    if (cur > 6) {
      error_msg = "Error: Too much condition added.\n";
      error_hap = true;
      return;
    }
    cin >> key[cur];
    if (key[cur] == "-1") {
      break;
    }
    cin >> value[cur];
    cur++;
  }
  for (int j = 0; j < cur; j++) {
    if (key[j] == "InvoiceNo" || key[j] == "invoiceno") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->InvoiceNo != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->InvoiceNo == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else if (key[j] == "StockCode" || key[j] == "stockcode") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->StockCode != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->StockCode == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else if (key[j] == "Quantity" || key[j] == "quantity") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->Quantity != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->Quantity == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else if (key[j] == "UnitPrice" || key[j] == "unitprice") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->UnitPrice != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->UnitPrice == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else if (key[j] == "CustomerID" || key[j] == "customerid") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->CustomerID != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->CustomerID == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else if (key[j] == "Country" || key[j] == "country") {
      if (rset.size()) {
        for (int i = 0; i < rset.size(); i++) {
          if (rset[i]->Country != value[j]) {
            rset[i]->unselected = true;
          }
        }
      } else {
        for (int i = 0; i < t->records.size(); i++) {
          if (t->records[i]->Country == value[j]) {
            rset.push_back(t->records[i]);
          }
        }
      }
    } else {
      error_msg = "Error: Invalid column name '" + key[j] + "'.\n";
      error_hap = true;
      return;
    }
  }
};

void update() {
  cur = 0;
  while (true) {
    if (cur > 6) {
      error_msg = "Error: Too much column selected.\n";
      error_hap = true;
      return;
    }
    cin >> key[cur];
    if (key[cur] == "-1") {
      break;
    }
    cin >> value[cur];
    cur++;
  }
  for (int j = 0; j < cur; j++) {
    if (key[j] == "InvoiceNo" || key[j] == "invoiceno") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->InvoiceNo = value[j];
        }
      }
    } else if (key[j] == "StockCode" || key[j] == "stockcode") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->StockCode = value[j];
        }
      }
    } else if (key[j] == "Quantity" || key[j] == "quantity") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->Quantity = value[j];
        }
      }
    } else if (key[j] == "UnitPrice" || key[j] == "unitprice") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->UnitPrice = value[j];
        }
      }
    } else if (key[j] == "CustomerID" || key[j] == "customerid") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->CustomerID = value[j];
        }
      }
    } else if (key[j] == "Country" || key[j] == "country") {
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected) {
          rset[i]->Country = value[j];
        }
      }
    } else {
      error_msg = "Error: Invalid column name '" + key[j] + "'.\n";
      error_hap = true;
      return;
    }
  }
};

void insert() {
  for (int i = 0; i < 6; i++) {
    temp_value[i] = "";
  }
  cur = 0;
  while (true) {
    if (cur > 6) {
      error_msg = "> Error: Too much input column.\n";
      error_hap = true;
      return;
    }
    cin >> key[cur];
    if (key[cur] == "-1") {
      break;
    }
    cin >> value[cur];
    cur++;
  }
  for (int j = 0; j < cur; j++) {
    if (key[j] == "InvoiceNo" || key[j] == "invoiceno") {
      temp_value[0] = value[j];
    } else if (key[j] == "StockCode" || key[j] == "stockcode") {
      temp_value[1] = value[j];
    } else if (key[j] == "Quantity" || key[j] == "quantity") {
      temp_value[2] = value[j];
    } else if (key[j] == "UnitPrice" || key[j] == "unitprice") {
      temp_value[3] = value[j];
    } else if (key[j] == "CustomerID" || key[j] == "customerid") {
      temp_value[4] = value[j];
    } else if (key[j] == "Country" || key[j] == "country") {
      temp_value[5] = value[j];
    } else {
      error_msg = "> Error: Invalid column name '" + key[j] + "'.\n";
      error_hap = true;
      return;
    }
  }
  temp_record = new Record{temp_value[0], temp_value[1], temp_value[2],
                           temp_value[3], temp_value[4], temp_value[5]};
  t->records.push_back(temp_record);
};

void load_table() {
  std::chrono::steady_clock::time_point st1 = std::chrono::steady_clock::now();
  ifstream is("./data.csv", ios::in);
  string line_str;
  stringstream ss;
  getline(is, line_str);
  ss << line_str;
  getline(ss, t->title->InvoiceNo, ',');
  getline(ss, t->title->StockCode, ',');
  getline(ss, t->title->Quantity, ',');
  getline(ss, t->title->UnitPrice, ',');
  getline(ss, t->title->CustomerID, ',');
  getline(ss, t->title->Country, '\x0D');
  ss.clear();
  while (getline(is, line_str)) {
    temp_record = new Record{};
    ss << line_str;
    getline(ss, temp_record->InvoiceNo, ',');
    getline(ss, temp_record->StockCode, ',');
    getline(ss, temp_record->Quantity, ',');
    getline(ss, temp_record->UnitPrice, ',');
    getline(ss, temp_record->CustomerID, ',');
    getline(ss, temp_record->Country, '\x0D');
    t->records.push_back(temp_record);
    ss.clear();
  }
  std::chrono::steady_clock::time_point ed1 = std::chrono::steady_clock::now();
  temp_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(ed1 - st1).count();
}

inline void print_select(Record* rcd) {
  cout
      << "+----------+----------+----------+----------+----------+-------------"
         "-------+\n";
  cout << "|" << setw(10) << rcd->InvoiceNo << "|" << setw(10) << rcd->StockCode
       << "|" << setw(10) << rcd->Quantity << "|" << setw(10) << rcd->UnitPrice
       << "|" << setw(10) << rcd->CustomerID << "|" << setw(20) << rcd->Country
       << "|\n";
};

void transaction(Table* tb){};

int main() {
  thread load(load_table);
  string temp_username, temp_password, temp_user, temp_identity;
  user_list.insert(
      pair<string, User*>("root", new User{"root", "root", priv::SuperUser}));
  while (true) {
    cout << "Please enter your Username: ";
    cin >> temp_username;
    if (user_list.count(temp_username)) {
      cout << "Password: ";
      cin >> temp_password;
      if (temp_password == user_list.at(temp_username)->password) {
        cout << "Login successfully.\n\n";
        cur_user = user_list.at(temp_username);
        break;
      } else {
        cout << "Wrong password. Try again.\n";
        continue;
      }
    } else {
      cout << "No existing user found. Input again.\n";
    }
  }
  load.join();
  cout << "> Table '" << t->table_name
       << "' Loaded Successfully. [ExecTime: " << temp_time
       << " ms]\n> Enter the instructions (End with -1): \n";
  std::chrono::steady_clock::time_point st2, ed2;
  string op;
  int changed;
  while (true) {
    cout << cur_user->userName << "@" << t->table_name << "# ";
    changed = 0;
    error_hap = false;
    while (!rset.empty()) {
      rset.pop_back();
    }
    cin >> op;
    st2 = std::chrono::steady_clock::now();
    if (op == "insert") {
      if (cur_user->identity == priv::NormalUser) {
        cout << "> Error: NormalUser are not permitted to change tables.";
        cin.ignore(INT64_MAX, '\n');
      } else {
        insert();
        if (error_hap) {
          cout << error_msg;
          continue;
        }
        cout << "> Query Succeded. New row inserted.";
      }
    } else if (op == "delete") {
      if (cur_user->identity == priv::NormalUser) {
        cout << "> Error: NormalUser are not permitted to change tables.";
        cin.ignore(INT64_MAX, '\n');
      } else {
        filter_noindex();
        if (error_hap) {
          cout << error_msg;
          continue;
        }
        for (int i = 0; i < rset.size(); i++) {
          if (!rset[i]->unselected && !rset[i]->deleted) {
            rset[i]->deleted = true;
            changed++;
          }
          rset[i]->unselected = false;
        }
        cout << "> Query Succeded. " << changed << " Rows deleted.";
      }
    } else if (op == "update") {
      if (cur_user->identity == priv::NormalUser) {
        cout << "> Error: NormalUser are not permitted to change tables.";
        cin.ignore(INT64_MAX, '\n');
      } else {
        filter_noindex();
        update();
        for (int i = 0; i < rset.size(); i++) {
          if (!rset[i]->unselected && !rset[i]->deleted) {
            changed++;
          }
          rset[i]->unselected = false;
        }
        cout << "> Query Succeded. " << changed << " Rows updated.";
      }
    } else if (op == "select") {
      filter_noindex();
      if (error_hap) {
        cout << error_msg;
        continue;
      }
      print_select(t->title);
      for (int i = 0; i < rset.size(); i++) {
        if (!rset[i]->unselected && !rset[i]->deleted) {
          print_select(rset[i]);
          changed++;
        }
        rset[i]->unselected = false;
      }
      cout << "+----------+----------+----------+----------+----------+--------"
              "-----"
              "-------+\n";
      cout << "> Query Succeded. " << changed << " Rows selected.";
    } else if (op == "givepriv") {
      if (cur_user->identity == priv::SuperUser) {
        cin >> temp_user;
        if (user_list.count(temp_user)) {
          user_list.at(temp_user)->identity = priv::Admin;
          cout << "> Succeeded. User '" << temp_user << "' is Admin now.";
        } else {
          cout << "> Error: No existing user '" << temp_user << "' found.";
        }
      } else {
        cout << "> Error: You are not the SuperUser. ";
        cin.ignore(INT64_MAX, '\n');
      }
    } else if (op == "createuser") {
      if (cur_user->identity == priv::SuperUser) {
        cin >> temp_user >> temp_password;
        if (user_list.count(temp_user)) {
          cout << "> Error: User '" << temp_user << "' already exists.";
        } else {
          user_list.insert(pair<string, User*>(
              temp_user, new User{temp_user, temp_password, priv::NormalUser}));
          cout << "> Succeeded. User '" << temp_user << "' is created.";
        }
      } else {
        cout << "> Error: You are not the SuperUser. ";
        cin.ignore(INT64_MAX, '\n');
      }
    } else if (op == "changeuser") {
      cin >> temp_username >> temp_password;
      if (user_list.count(temp_username)) {
        if (temp_password == user_list.at(temp_username)->password) {
          cout << "> Successfully changed to user '" << temp_username << "'.";
          cur_user = user_list.at(temp_username);
        } else {
          cout << "> Error: Wrong password.";
        }
      } else {
        cout << "> Error: No existing user found.";
      }
    } else if (op == "checkpriv") {
      if (cur_user->identity == priv::SuperUser) {
        cin >> temp_user;
        if (user_list.count(temp_user)) {
          switch (user_list.at(temp_user)->identity) {
            case 0:
              temp_identity = "SuperUser";
              break;
            case 1:
              temp_identity = "Admin";
              break;
            case 2:
              temp_identity = "NormalUser";
              break;
          }
          cout << "> User '" << temp_user << "' is '" << temp_identity << "'.";
        } else {
          cout << "> Error: No existing user '" << temp_user << "' found.";
        }
      } else {
        cout << "> Error: You are not the SuperUser.";
        cin.ignore(INT64_MAX, '\n');
      }
    } else {
      cout << "> Cannot recognize command '" << op
           << "'. Please input again.\n";
      cin.ignore(INT64_MAX, '\n');
      continue;
    }
    ed2 = std::chrono::steady_clock::now();
    cout << "    [ExecTime: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(ed2 - st2)
                .count()
         << " ms]\n\n";
  }
}