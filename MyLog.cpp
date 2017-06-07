#include "stdafx.h"
#include "MyLog.h"
#include <time.h>


MyLog::MyLog(FILE_TYPE type)
{
	flag = false;

	fileType = type;

	curFileCount = 0;

	if (fileType >= 0 && fileType < FILE_TYPE_TOTLA) {
		InitLogConfig();
		flag = true;
	}	
}

MyLog::~MyLog()
{
}

int MyLog::WriteLog(std::string msg)
{

	category->info(msg.c_str());

	//�����־�ļ�������С���������ļ���
	if (true)
	{
		curFileCount++;
		curFileName = GetFileName();

		appender = new log4cpp::FileAppender("default", curFileName.c_str());
		appender->setLayout(layout);
		category->addAppender(appender);
	}

	return 0;
}

int MyLog::WriteLog(const char * pMsg,...)
{
	va_list args;
	char log[1024];
	va_start(args, pMsg);
	vsnprintf_s(log,1024-1,pMsg,args);

	std::string strLog = "";
	strLog = log;
	WriteLog(strLog);

	return 0;
}

int MyLog::test()
{
	bool bRet = false;

	std::string dir = "log/20170603";

	dir = GetFileName();

	for (int i = 0;i < 10;i++) {
		curFileCount++;
		dir = GetFileName();
	}

	dir = "";
	dir = GetExeDir();

	bRet = IsDirExist(dir);
	if (!bRet) {
		CreateDir(dir);
	}

	return 0;
}

int MyLog::InitLogConfig()
{
	std::string name = log_file_name[fileType]; //"default"
	std::string filename = GetFileName();
	//std::string filename = "log/" + log_file_name[fileType];
	//filename += ".log";
	//curFileName = log_file_name[fileType] + int2string(curFileCount) +".log";
	std::string str_category = log_file_name[fileType] + "category";

	layout = new log4cpp::PatternLayout();
	/*
	%c category��
	%d ���ڣ����ڿ��Խ�һ�������ø�ʽ���û����Ű�Χ������%d{ %H:%M : %S,%l } ���� %d{ %d %m %Y %H:%M : %S,%l }����������þ������ڸ�ʽ��������Ĭ�ϸ�ʽ��ʹ�á�Wed Jan 02 02:03 : 55 1980�������ڵĸ�ʽ������ANSI C����strftime�е�һ�¡���������һ����ʽ����%l����ʾ���룬ռ����ʮ����λ��
	%m ��Ϣ��
	%n ���з��������ƽ̨�Ĳ�ͬ����ͬ���������û�͸����
	%p ���ȼ���
	%r �Դ�layout��������ĺ�������
	%R ��1970��1��1��0ʱ��ʼ��ĿǰΪֹ��������
	%u ���̿�ʼ��ĿǰΪֹ��ʱ����������
	%x NDC��
	*/
	layout->setConversionPattern("%m%n");
	//layout->setConversionPattern("");

	appender = new log4cpp::FileAppender(name.c_str(), filename.c_str());
	appender->setLayout(layout);

	category = &(log4cpp::Category::getInstance(str_category.c_str()));
	category->addAppender(appender);
	category->setPriority(log4cpp::Priority::INFO);

	return 0;
}

std::string MyLog::GetExeDir()
{
	std::string dir = "";

	TCHAR chpath[MAX_PATH];
	::GetModuleFileName(NULL,chpath, MAX_PATH);
	(_tcsrchr(chpath, _T('\\')))[1] = 0;
	dir = TCHAR2string(chpath);

	return dir;
}

bool MyLog::IsDirExist(std::string path)
{
	bool bRet = false;

	struct _stat fileStat;
	if ((_stat(path.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR))
	{
		bRet = true;
	}

	return bRet;
}

bool MyLog::CreateDir(std::string path)
{
	std::string cmd = "md " + path;
	system(cmd.c_str());//����һ���ļ��� 

	return true;
}

std::string MyLog::TCHAR2string(TCHAR * tch)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, tch, -1, NULL, 0, NULL, NULL);

	char* chRtn = new char[iLen * sizeof(char)];

	WideCharToMultiByte(CP_ACP, 0, tch, -1, chRtn, iLen, NULL, NULL);

	std::string str(chRtn);

	return str;
}

std::string MyLog::int2string(int value)
{
	return std::to_string(value);
}

long MyLog::GetFileSize()
{
	if (curFileName == "") {
		return 0;
	}

	struct _stat info;
	_stat(curFileName.c_str(), &info);
	long size = info.st_size;

	return size;
}

std::string MyLog::GetFileName()
{
	//��־�ļ���ʽΪ���� + ���� + ��׺��.log��
	std::string stringDate = "";
	char chDate[255];
	time_t today;
	tm* t = new tm;

	time(&today);
	//t = localtime(&today);
	localtime_s(t, &today);

	strftime(chDate, 255, "%Y%m%d", t);
	stringDate = chDate;

	curFileName = log_file_name[fileType] + "_" + stringDate + "_" + int2string(curFileCount) + ".log";

	if (t) {
		delete t;
	}

	return curFileName;
}
