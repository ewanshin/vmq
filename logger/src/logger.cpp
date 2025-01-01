#include "stdafx.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "logger/logger.h"

class logger::Implement
{
public:
    Implement() 
    {
        // 로그 시스템 초기화 코드
        console_ = spdlog::stdout_color_mt("console");
        file_ = spdlog::daily_logger_format_mt("file", "logs.txt");
    }

    ~Implement() 
    {
        // 리소스 정리 및 로그 시스템 종료
        spdlog::shutdown();
    }

    bool start() {
        // 로그 시작 관련 코드
        return true;
    }

    void release() 
    {
        // 로그 해제 관련 코드
    }

    template<typename ... Args>
    void log(int level, Args const& ... args)
    {
        // 로그 레벨에 따른 로그 출력
        switch (level) 
         {
        case level_trace:
        {
            console_->trace(args...);
            file_->trace(args...);
        }
        break;
        case level_debug:
        {
            console_->debug(args...);
            file_->debug(args...);
        }
        break;
        case level_info:
        {
            console_->info(args...);
            file_->info(args...);
        }
        break;
        case level_warn:
        {
            console_->warn(args...);
            file_->warn(args...);
        }
        break;
        case level_error:
        {
            console_->error(args...);
            file_->error(args...);
        }
        break;
        case level_fatal:
        {
            console_->critical(args...);
            file_->critical(args...);
        }
        break;
        }
    }

    bool flush(int timeout_ms) 
    {
        // 로그 대기열 전송
        return true;
    }

    void log_line(log_level level, std::string& message)
    {
		std::ostringstream oss;
		
		switch (level)
		{
		case level_trace:
			console_->trace(message);
			file_->trace(message);
			break;
		case level_debug:
			console_->debug(message);
			file_->debug(message);
			break;
		case level_info:
			console_->info(message);
			file_->info(message);
			break;
		case level_warn:
			console_->warn(message);
			file_->warn(message);
			break;
		case level_error:
			console_->error(message);
			file_->error(message);
			break;
		case level_fatal:
			console_->critical(message);
			file_->critical(message);
			break;
		}
    }

private:
    std::shared_ptr<spdlog::logger> console_;
    std::shared_ptr<spdlog::logger> file_;
};

// 실제 logger 클래스 구현
logger::logger() 
    : impl_(std::make_shared<Implement>()) 
{
}

logger::~logger() = default;

bool logger::start()
{
    return impl_->start();
}

void logger::release()
{
    impl_->release();
}

bool logger::flush(int timeout_ms)
{
    return impl_->flush(timeout_ms);
}

void logger::log_line(log_level level, std::string& message)
{
    impl_->log_line(level, message);
}
