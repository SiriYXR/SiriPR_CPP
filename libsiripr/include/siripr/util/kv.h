#ifndef SIRIPR_UTIL_KV_H_
#define SIRIPR_UTIL_KV_H_

#include <map>
#include <string>

namespace siripr {

	class Kv {
	public:
		Kv();

		/*
		���ļ��г�ʼ������

		������
			file���ļ�·��

		����ֵ��
			��
		*/
		void load(const std::string &file);

		/*
		ͨ������ȡ��Ӧ��ֵ��

		������
			key�����ַ���

		����ֵ��
			��Ӧ��ֵ�ַ���
		*/
		std::string get(const std::string &key);

		/*
		���һ�Լ�ֵ�ԡ�

		������
			key�����ַ���
			value��ֵ�ַ���

		����ֵ��
			��
		*/
		void add(const std::string &key, const std::string &value);

		/*
		�Ƴ�������Ӧ��ֵ�ԡ�

		������
			key�����ַ���

		����ֵ��
			��
		*/
		void remove(const std::string &key);

		/*
		������м�ֵ�ԡ�

		������
			��

		����ֵ��
			��
		*/
		void clear();

	private:
		std::map<std::string, std::string> data_;
	};

}

#endif // SIRIPR_UTIL_KV_H_