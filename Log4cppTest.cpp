// Log4cppTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp\FileAppender.hh>
#include <log4cpp\PatternLayout.hh>
#include <log4cpp\RollingFileAppender.hh>

#include "MyLog.h"
#include <thread>

using namespace std;

#define TEST_COUNTS 10000

long getFileSize2(const char* strFileName)
{
	struct _stat info;
	_stat(strFileName, &info);
	long size = info.st_size;
	return size;
}


int log4cpp_test(std::string content)
{
	log4cpp::Appender *appender = new log4cpp::FileAppender("default", "log/test.log");
	//appender->setLayout(new log4cpp::BasicLayout());
	//appender->setLayout(new log4cpp::SimpleLayout());
	log4cpp::PatternLayout *pLayout = new log4cpp::PatternLayout();
	pLayout->setConversionPattern("%d::%m%n");
	appender->setLayout(pLayout);

	log4cpp::Category& category = log4cpp::Category::getInstance(std::string("category"));
	category.addAppender(appender);

	category.setPriority(log4cpp::Priority::INFO);

	category.info(content.c_str());

	return 0;
}

int log4cpp_roll_test(std::string value)
{
	log4cpp::Appender *appender1 = new log4cpp::FileAppender("default", "log/fa.log");
	log4cpp::PatternLayout *pLayout1 = new log4cpp::PatternLayout();
	pLayout1->setConversionPattern("%d::%m%n");
	appender1->setLayout(pLayout1);

	log4cpp::Category& category1 = log4cpp::Category::getInstance(std::string("category1"));
	category1.addAppender(appender1);
	category1.info("bbbbbbbbb");

	cout<<getFileSize2("log/fa.log")<<endl;

	DWORD start = ::GetTickCount();
	getFileSize2("log/fa.log");
	DWORD end = ::GetTickCount();
	cout << "1.calc time: "<<end-start<<" ms"<< endl;

	log4cpp::Appender *appender = new log4cpp::RollingFileAppender("roll","log/rfa.log",1024,1);
	log4cpp::PatternLayout *pLayout = new log4cpp::PatternLayout();
	pLayout->setConversionPattern("%d::%m%n");
	appender->setLayout(pLayout);

	log4cpp::Category& category = log4cpp::Category::getInstance(std::string("category2"));
	category.addAppender(appender);

	category.setPriority(log4cpp::Priority::INFO);

	category.info(value.c_str());

	cout << getFileSize2("log/rfa.log") << endl;

	DWORD start1 = ::GetTickCount();
	getFileSize2("log/rfa.log");
	DWORD end1 = ::GetTickCount();
	cout << "2.calc time: " << end1 - start1 << " ms" << endl;

	DWORD start2 = ::GetTickCount();
	for (int i = 0;i<10000;i++) {
		//getFileSize2("log/fa.log");
		getFileSize2("log/rfa.log");
	}
	DWORD end2= ::GetTickCount();
	cout << "3.calc time: " << end2 - start2 << " ms" << endl;

	return 0;
}

int mylog_test()
{
	MyLog log(FILE_TYPE::SYS_S);
	log.test();
	log.WriteLog("s:aaaaaaaaaaaaaaaaaaa");
	log.WriteLog("s:bbbbbbbbbbbbbbbbbbb");
	log.WriteLog("s:ccccccccccccccccccc");
	log.WriteLog("s:ddddddddddddddddddd");

	MyLog log1(FILE_TYPE::RAW_S);
	log1.WriteLog("r:aaaaaaaaaaaaaaaaaaa");
	log1.WriteLog("r:bbbbbbbbbbbbbbbbbbb");
	log1.WriteLog("r:ccccccccccccccccccc");
	log1.WriteLog("r:ddddddddddddddddddd");

	return 0;
}

void thread_func1(LPVOID lParam)
{
	MyLog* log = (MyLog*)lParam;
	std::string a = "a thread";
	/*for (size_t i = 0; i < TEST_COUNTS; i++)
	{
		log->WriteLog(a);
	}*/
	while (true) {
		log->WriteLog(a);
		Sleep(100);
	}
}

void thread_func2(LPVOID lParam)
{
	MyLog* log = (MyLog*)lParam;
	std::string a = "b thread";
	/*for (size_t i = 0; i < TEST_COUNTS; i++)
	{
		log->WriteLog(a);
	}*/
	while (true) {
		log->WriteLog(a);
		Sleep(100);
	}
}

int thread_test()
{
	MyLog log(FILE_TYPE::SYS_S);

	std::thread t1(&thread_func1,&log);
	//t1.join();
	t1.detach();

	std::thread t2(&thread_func2,&log);
	//t2.join();
	t2.detach();

	return 0;
}

int main()
{
	//thread_test();

	mylog_test();

	/*log4cpp::OstreamAppender* osAppender = new log4cpp::OstreamAppender("osAppender", &cout);

	osAppender->setLayout(new log4cpp::BasicLayout());
	log4cpp::Category& root = log4cpp::Category::getRoot();

	root.addAppender(osAppender);

	root.setPriority(log4cpp::Priority::DEBUG);

	root.error("Hello log4cpp in aError Message!");

	root.warn("Hello log4cpp in aWarning Message!");

	log4cpp::Category::shutdown();*/

	//log4cpp_test("tttttttttttt");

	//log4cpp_roll_test("aaaaaaaaaaaa");

	system("pause");

    return 0;
}

