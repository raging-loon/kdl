#ifndef KDLC_INTERFACE_COMPILER_MESSAGE_H_
#define KDLC_INTERFACE_COMPILER_MESSAGE_H_

#include <string_view>

namespace kdl
{

class CompilerMessage
{
public:

    CompilerMessage();

    void setSource(const std::string_view str) { m_source = str; }

private:

    const std::string_view m_source;

};


} // kdl


#endif // KDLC_INTERFACE_COMPILER_MESSAGE_H_