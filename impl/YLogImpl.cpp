#include "YLog.h"
#include <cstdio>

namespace { // hide class name in anonymouse namespace

// YLog that writes via printf
class StdoutLogImpl : public ylog::FmtLog
{
protected:
   void Write(const char* formattedString, const void* source) noexcept final
   {
      printf(formattedString);
      printf("\n");
   }
};

} // namespace {

namespace ylog {

FmtLog* Defaults::Stdout() noexcept
{
   static StdoutLogImpl s_stdoutLog;
   return &s_stdoutLog;
}

}
