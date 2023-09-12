#ifndef __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTS_H_
#define __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTS_H_

namespace CppServer {
namespace core {
enum class Event { Readable, Writeable, Connected, Closeable, None };
}
} // namespace CppServer

#endif // __PROJECTS_CPP_SERVER_V2_SRC_CORE_EVENTS_H_