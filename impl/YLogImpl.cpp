#include "YLog.h"
#include <cstdio>

namespace { // hide class name in anonymouse namespace

// YLog that writes via printf
class StdoutLogImpl : public ylog::YLog
{
public:
   virtual void Fmt(const char* fmt, ...) override final
   {
      va_list list;
      va_start(list, fmt);
      FmtV(fmt, list);
      va_end(list);
   }

   virtual void FmtI(void* instance, const char* fmt, ...) override final
   {
      va_list list;
      va_start(list, fmt);
      FmtV(fmt, list, instance);
      va_end(list);
   }

   virtual void FmtV(const char* fmt, va_list args, const void* instance = nullptr) override final
   {
      vprintf(fmt, args);
      printf("\n");
   }
};

} // namespace {

namespace ylog {

YLog* Defaults::StdoutLog() noexcept
{
   static StdoutLogImpl s_stdoutLog;
   return &s_stdoutLog;
}

}
