// (c) 2018 Vladimir Zvezda
// Interface for logging  
#pragma once
#include <cstdarg>

namespace ylog {

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
class YLog
{
public:
   enum { kMaxRecordSize = 300 };

   virtual void Fmt(const char* fmt, ...) = 0;
   virtual void FmtI(void* instance, const char* fmt, ...) = 0;
   virtual void FmtV(const char* fmt, va_list args, const void* instance = nullptr) = 0;
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
class Defaults final
{
   // YLog that writes no nowhere
   class NowhereLogImpl: public YLog
   {
   public:
      virtual void Fmt(const char* fmt, ...) override final   {}
      virtual void FmtI(void* instance, const char* fmt, ...) override final {}
      virtual void FmtV(const char* fmt, va_list args, const void* instance) override final  {}
   };
public:
   static YLog* NowhereLog() noexcept
   {
      static NowhereLogImpl s_nowhereLog;
      return &s_nowhereLog;
   }

   static YLog* StdoutLog() noexcept;
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
template <class Owner>
class Loggable
{
public:
   Loggable(YLog* pLog = nullptr) noexcept
      : m_pLog(pLog == nullptr ? Defaults::NowhereLog() : pLog)
   {}

   void SetLogger(YLog* pLog)  {  m_pLog = pLog;  }
   YLog* GetLogger() { return m_pLog;  }

   void Log(const char* fmt, ...)
   {
      va_list list;
      va_start(list, fmt);
      m_pLog->FmtV(fmt, list, static_cast<const Owner*>(this));
      va_end(list);
   };

   void LogV(const char* fmt, va_list args)
   {
      m_pLog->FmtV(fmt, args, static_cast<const Owner*>(this));
   }

private:
   YLog* m_pLog;
};

}