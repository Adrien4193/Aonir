#pragma once

#include <exception>
#include <string>

namespace Aonir
{
    class Exception : public std::exception
    {
    private:
        std::string m_message;

    public:
        AONIR_CORE_API explicit Exception(std::string message);

        [[nodiscard]] AONIR_CORE_API auto what() const -> const char * override;
    };
}
