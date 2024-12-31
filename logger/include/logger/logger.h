#ifndef __lplog__
#define __lplog__

#include <fstream>
#include <string>
#include <sstream>

enum log_level
{
	level_trace = 0,
	level_debug,
	level_info,
	level_warn,
	level_error,
	level_fatal,
};

class logger 
{
public:
	logger();
	~logger();

	bool start(/*const std::string& module, const lplog_conf::option& conf*/);
	void release();

	template<typename ... Args>	inline void trace(Args const& ... args) { log(level_trace, args...); }
	template<typename ... Args>	inline void debug(Args const& ... args) { log(level_debug, args...); }
	template<typename ... Args>	inline void info(Args const& ... args) { log(level_info, args...); }
	template<typename ... Args>	inline void warn(Args const& ... args) { log(level_warn, args...); }
	template<typename ... Args>	inline void error(Args const& ... args) { log(level_error, args...); }
	template<typename ... Args>	inline void fatal(Args const& ... args) { log(level_fatal, args...); }

	template<typename ... Args>
	void log(log_level level, Args const& ... args)
	{
		std::ostringstream stream;
		using List = int[];
		(void)List {
			0, ((void)(stream << args), 0) ...
		};

		log_line(level, std::move(stream.str()));
	}

	void log_line(log_level level, std::string& message);
	bool flush(int timeout_ms);

private:
	class Implement;
	std::shared_ptr<Implement> impl_;
};

#endif
