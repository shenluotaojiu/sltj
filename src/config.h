#ifndef __SLTJ_CONFIG_H__
#define __SLTJ_CONFIG_H__

#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "log.h"

namespace sltj
{
    class ConfigVarBase
    {
    public:
        using ptr = std::shared_ptr<ConfigVarBase>;

        ConfigVarBase(const std::string &name, const std::string &description)
            : m_name(name), m_description(description) {}
        virtual ~ConfigVarBase() {}
        const std::string getName() const { return m_name; }
        const std::string getDescription() const { return m_description; }
        virtual std::string toString() = 0;
        virtual bool fromString(const std::string &val) = 0;

    private:
        std::string m_name;
        std::string m_description;
    };

    template <class T>
    class ConfigVar : public ConfigVarBase
    {
    public:
        using ptr = std::shared_ptr<ConfigVar>;
        ConfigVar(const std::string &name, const std::string &description, const T &default_val)
            : ConfigVarBase(name, description), m_val(default_val) {}

        std::string toString() override
        {
            try
            {
                return boost::lexical_cast<std::string>(m_val);
            }
            catch (std::exception &err)
            {
                SLTJ_LOG_ERROR(SLTJ_LOG_ROOT()) << err.what() << " convert : " << typeid(m_val).name() << "toString";
            }
            return "";
        }
        bool fromString(const std::string &val) override
        {
            try
            {
                m_val = boost::lexical_cast<T>(m_val);
                return true;
            }
            catch (std::exception &err)
            {
                SLTJ_LOG_ERROR(SLTJ_LOG_ROOT()) << err.what() << " convert : string to " << typeid(m_val).name();
            }
            return false;
        }
        const T getValue() const { return m_val; }
        void setValue(const T& val) { m_val = val; }
    private:
        T m_val;
    };

    class Config
    {
    public:
        using ConfigVarMap = std::map<std::string, ConfigVarBase::ptr>;

        template <class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string &name,
                                                 const T &default_val, const std::string &description)
        {
            auto tmp = Lookup<T>(name);
            if (tmp)
            {
                SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "Lookup name = " << name << " exist";
                return tmp;
            }

            if (name.find_first_not_of("qwertyuiopasdfghjklzxcvbnm1234567890._QWERTYUIOPASDFGHJKLZXCVBNM") != std::string::npos)
            {
                SLTJ_LOG_ERROR(SLTJ_LOG_ROOT()) << "Lookup name invaid(无效)" << name;
                throw std::invalid_argument(name);
            }

            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, description, default_val));
            m_datas[name] = v;
            return v;
        }

        template <class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string &name)
        {
            auto i = m_datas.find(name);
            // 找到后,把父类指针强转成子类指针
            return i == m_datas.end() ? nullptr : std::dynamic_pointer_cast<ConfigVar<T>>(i->second);
        }

    private:
        static ConfigVarMap m_datas;
    };
    
}

#endif