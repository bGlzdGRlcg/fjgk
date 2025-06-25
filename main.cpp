// curl -X POST -d
// "method=query&logname=登录名&pwd=密码的md5（32位小写）&ksh=考生号&check="
// https://gk.eeafj.cn/gkcj.shtml
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <openssl/md5.h>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     string *response) {
  size_t totalSize = size * nmemb;
  response->append((char *)contents, totalSize);
  return totalSize;
}

int main() {
  string logname, pwd_raw, pwd, ksh;
  cout << "请输入登录名: ";
  cin >> logname;
  cout << "请输入密码: ";
  cin >> pwd_raw;
  cout << "请输入考生号: ";
  cin >> ksh;

  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((unsigned char *)pwd_raw.c_str(), pwd_raw.length(), digest);
  char md5_str[33];
  for (int i = 0; i < 16; ++i) {
    sprintf(md5_str + i * 2, "%02x", digest[i]);
  }
  pwd = md5_str;

  CURL *curl;
  string response_data;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  string p = "method=query&logname=" + logname + "&pwd=" + pwd + "&ksh=" + ksh;

  curl_easy_setopt(curl, CURLOPT_URL, "https://gk.eeafj.cn/gkcj.shtml");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, p.c_str());
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  CURLcode res = curl_easy_perform(curl);

  if (res == CURLE_OK) {
    bool hasLogo = response_data.find("/images/k_logo.gif") != string::npos;
    bool hasLogoBg1 =
        response_data.find("/images/k_logo_bg_1.png") != string::npos;
    bool hasLogoBg =
        response_data.find("/images/k_logo_bg.gif") != string::npos;

    if (!hasLogo && !hasLogoBg1 && !hasLogoBg) {
      cout << "请求失败，请检查账号密码和考生号（注意，不是准考证号）" << endl;
      curl_easy_cleanup(curl);
      curl_global_cleanup();
      return 0;
    }

    size_t pos = 0;
    while ((pos = response_data.find("/images/k_logo.gif", pos)) !=
           string::npos) {
      response_data.replace(pos, 18, "images/k_logo.gif");
      pos += 19;
    }

    pos = 0;
    while ((pos = response_data.find("/images/k_logo_bg_1.png", pos)) !=
           string::npos) {
      response_data.replace(pos, 23, "images/k_logo_bg_1.png");
      pos += 24;
    }

    pos = 0;
    while ((pos = response_data.find("/images/k_logo_bg.gif", pos)) !=
           string::npos) {
      response_data.replace(pos, 21, "images/k_logo_bg.gif");
      pos += 22;
    }

    std::ofstream file("index.html");
    if (file.is_open()) {
      file << response_data;
      file.close();
      cout << "结果已保存到 index.html 文件中，请用浏览器打开" << endl;
    } else {
      cout << "无法创建 index.html 文件" << endl;
    }
  } else {
    cout << "请求失败: " << curl_easy_strerror(res) << endl;
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return 0;
}