#pragma once
#pragma once

#include <Windows.h>


#include <string>
#include <memory>

#include <log4cpp\Category.hh>
#include <log4cpp\Appender.hh>
#include <log4cpp\FileAppender.hh>
#include <log4cpp\OstreamAppender.hh>
#include <log4cpp\Layout.hh>
#include <log4cpp\SimpleLayout.hh>
#include <log4cpp\BasicLayout.hh>
#include <log4cpp\PatternLayout.hh>
#include <log4cpp\Priority.hh>

#include <log4cpp\RollingFileAppender.hh>
#include <log4cpp\DailyRollingFileAppender.hh>

enum FILE_TYPE {
	RAW_S = 0,
	DIST_S,
	JSON_S,
	SQL_S,
	SYS_S,
	KALMAN_S,
	FILE_TYPE_TOTLA
};

const std::string log_file_name[] = {
	"RAW_S",
	"DIST_S",
	"JSON_S",
	"SQL_S",
	"SYS_S",
	"Kalman_S"
};

class MyLog
{
public:
	MyLog(FILE_TYPE type);
	~MyLog();
public:
	int WriteLog(std::string msg);
	int WriteLog(const char* pMsg,...);

	int test();

public:
	bool flag;

private:
	//以下函数考虑移出本类
	std::string GetExeDir();		//获得exe程序所在目录
	bool IsDirExist(std::string path);		//检查目录是否存在
	bool CreateDir(std::string path);		//创建目录
	std::string TCHAR2string(TCHAR* tch);
	std::string int2string(int value);

private:
	int InitLogConfig();

	long GetFileSize();

	std::string GetFileName();

private:
	FILE_TYPE fileType;

	std::string curFileName;
	int curFileCount;

	log4cpp::Appender* appender;
	log4cpp::PatternLayout* layout;
	log4cpp::Category* category;
};



