#ifndef PATH_HPP
# define PATH_HPP

#include <iostream>
#include <vector>

class Path
{
    public:
        Path(const std::string &pathquery);
        virtual ~Path();
		//使わない
        Path(Path const &other);
        Path &operator=(Path const &other);

		const std::string & getRawPath() const;
		std::string getQuery() const;
		std::vector<std::string> getArgs() const;
		
		void splitPath();
		void createArgs();

    private:
		const std::string &pathquery_;
		std::string raw_path_;
		std::string query_;
		std::vector<std::string> args_;

};

#endif
