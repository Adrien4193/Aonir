#include "Exception.hpp"

#include <string>
#include <utility>

namespace Aonir
{
    Exception::Exception(std::string message):
        m_message(std::move(message))
    {
    }

    auto Exception::what() const -> const char *
    {
        return m_message.c_str();
    }
}
