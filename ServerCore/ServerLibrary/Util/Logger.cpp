#include "stdafx.h"
#include "Logger.h"
//-----------------------------------------------------------------//
LogPrintf::LogPrintf()
{
    printf("* Log create : printf log mode\n");
}

void LogPrintf::log(WCHAR *logStr)
{
    printf("%ws", logStr);
}

//-----------------------------------------------------------------//
LogFile::LogFile(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Log");
	xmlNode_t *elem = root->FirstChildElement("Path");

	array<WCHAR, _MAX_PATH> logFilePath;
	StrConvA2W((char *)elem->GetText(), logFilePath.data(), logFilePath.max_size());
	printf("* Log create : [%ws]file log mode\n", logFilePath.data());
	this->initialize(logFilePath.data());
}

LogFile::~LogFile()
{
    fs_.close();
    fs_.clear();

    std::size_t found = fileName_.find(L".log");
    if (found == wstr_t::npos){
        return;
    }

    //뒤에 로그파일이 붙으면 종료시, 종료시각을 파일이름 뒤에 붙여줍니다.
    wstr_t closeFileName = fileName_.substr(0, found);
    closeFileName += CLOCK.nowTime(L"_%Y%m%d-%H%M%S.log");
    _wrename(fileName_.c_str(), closeFileName.c_str());
}

void LogFile::initialize(WCHAR *logFileName)
{
    fileName_ = logFileName;
    fs_.open(logFileName, std::ios::out | std::ios::trunc);
    if (fs_.bad()) {
        printf("! logfile error, file open fail.\n");
        assert(false);
    }
}

void LogFile::log(WCHAR *logStr)
{
    printf("%ws", logStr);

    fs_ << logStr;
    fs_.flush();
}
//-----------------------------------------------------------------//

LogWriter::LogWriter()
{
    base_ = nullptr;
}

LogWriter::~LogWriter()
{
    prefix_.clear();

	SAFE_DELETE(base_);
};

void LogWriter::setLogger(LogBase *base, const WCHAR *logPrefix)
{
    prefix_.clear();     
    prefix_ = logPrefix;

    if (base_) {
        LogBase *old = base_;
        base_ = nullptr;
        old->unInitialize();
        
        SAFE_DELETE(old);
    }

    base_ = base;
    base_->initialize();
}

LogBase *LogWriter::logger()
{
    return base_;
}

void LogWriter::log(WCHAR *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	this->log(fmt, args);

	va_end(args);
}

void LogWriter::log(WCHAR *fmt, va_list args)
{
	wstr_t logMessage = CLOCK.nowTimeWithMilliSec();
	threadId_t threadId = GET_CURRENT_THREAD_ID();

	logMessage += L"\t";

	// 쓰레드 정보 넣기
	Thread *thread = ThreadManager::getInstance().at(threadId);
	if (thread) {
		logMessage += thread->name();
	}
	else {
		logMessage += prefix_;
	}
	//array<WCHAR, SIZE_8 * 2> threadIdStr;
	//snwprintf(threadIdStr, L"0x%X", (unsigned int) threadId);

	//logMessage += L":";
	//logMessage += threadIdStr.data();
	//logMessage += L"\t";

	array<WCHAR, SIZE_1024> logStr;

	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	logMessage += logStr.data();
	logMessage += L"\n";
    base_->log((WCHAR *)logMessage.c_str());
}
//-----------------------------------------------------------------//
SystemLog::SystemLog()
{
	xml_t config;
	if (!loadConfig(&config)) {
		printf("!!! have not config file\n");
		exit(0);
		return;
	}
	this->initialize(&config);
}

SystemLog::~SystemLog()
{
}

void SystemLog::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Log");
	if (!root) {
		printf("@ not exist log setting");
		LogBase *base = new LogPrintf();
		logWrite_.setLogger(base, L"testServer");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("Path");

	array<WCHAR, SIZE_256> tmp;
	elem = root->FirstChildElement("Prefix");
	StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
	wstr_t prefix = tmp.data();

	LogBase *base;
	elem = root->FirstChildElement("Type");
	const char *type = (char *)elem->GetText();
	if (!strcmp(type, "WithFile")) {
		base = new LogFile(config);
	}
	else {
		base = new LogPrintf();
	}

	logWrite_.setLogger(base, prefix.c_str());
}

void SystemLog::log(WCHAR *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
    logWrite_.log(fmt, args);
	va_end(args);
}