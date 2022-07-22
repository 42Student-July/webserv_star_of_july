#ifndef CONFIGVALIDATOR_HPP
# define CONFIGVALIDATOR_HPP

class ConfigValidator
{
    public:
        ConfigValidator();
        virtual ~ConfigValidator();
        ConfigValidator(ConfigValidator const &other);
        ConfigValidator &operator=(ConfigValidator const &other);

    private:
};

#endif
