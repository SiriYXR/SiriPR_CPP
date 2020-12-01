#ifndef SIRIPR_UTIL_KV_H_
#define SIRIPR_UTIL_KV_H_

#include <map>
#include <string>

namespace siripr {

	class Kv {
	public:
		Kv();

		/*
		从文件中初始化对象。

		参数：
			file：文件路径

		返回值：
			空
		*/
		void load(const std::string &file);

		/*
		通过键获取相应的值。

		参数：
			key：键字符串

		返回值：
			对应的值字符串
		*/
		std::string get(const std::string &key);

		/*
		添加一对键值对。

		参数：
			key：键字符串
			value：值字符串

		返回值：
			空
		*/
		void add(const std::string &key, const std::string &value);

		/*
		移除键所对应键值对。

		参数：
			key：键字符串

		返回值：
			空
		*/
		void remove(const std::string &key);

		/*
		清空所有键值对。

		参数：
			空

		返回值：
			空
		*/
		void clear();

	private:
		std::map<std::string, std::string> data_;
	};

}

#endif // SIRIPR_UTIL_KV_H_