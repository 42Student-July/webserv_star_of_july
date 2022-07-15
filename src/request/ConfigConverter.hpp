#ifndef CONFIGCONVERTER_HPP
# define CONFIGCONVERTER_HPP

#include <sstream>

#include "../config/ServerConfig.hpp"
#include "../response/ConfigDTO.hpp"

class ConfigConverter
{
    public:
        ConfigConverter();
        virtual ~ConfigConverter();
		ConfigDTO *toDTO(ServerConfig conf);

    private:
        ConfigConverter(ConfigConverter const &other);
        ConfigConverter &operator=(ConfigConverter const &other);
};

#endif
