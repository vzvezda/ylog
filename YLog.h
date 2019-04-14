//     
//   \|/ YLog
//   /|\ 2018.07.28 Vladimir Zvezda
//
//  The interface class for logging
#pragma once
#include "fmt/format.h"

namespace ylog {

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
struct Sink
{
   // Submits the log record to the log destination
   virtual void Write(const char* formattedString, const void* source = nullptr) noexcept = 0;
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
struct FmtLog: public Sink
{
   // Max size of the log record. App can issue several log records if it has to provide a really
   // long one.
   enum { kMaxRecordSize = 300 };

   // Write log record using fmt library
   template <typename ...Args>
   void Fmt(const char* fmtSyntax, Args... args) noexcept 
   {
      FmtS(nullptr, fmtSyntax, std::forward<Args>(args)...);
   }

   // Write log record using fmt library
   template <typename ...Args>
   void FmtS(const void* source, const char* fmtSyntax, Args... args) noexcept
   {
      try
      {
         char buffer[kMaxRecordSize + 1];

         auto res = fmt::format_to_n(buffer, 
            kMaxRecordSize, 
            fmtSyntax, 
            std::forward<Args>(args)...);

         *res.out = 0;
         Write(buffer, source);
      }
      catch (...)
      {
         Write("YLog: failed to format log message", source);
      }
   }
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
class Defaults final
{
   // YLog that writes to nowhere, used when 
   class NowhereLogImpl final: public FmtLog
   {
   protected:
      void Write(const char* formattedString, const void* source) noexcept final {}
   };
public:
   static FmtLog* Nowhere() noexcept
   {
      static NowhereLogImpl s_nowhereLog;
      return &s_nowhereLog;
   }

   static FmtLog* Stdout() noexcept;
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
enum class TraceMode { On, Off };

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
template <class Owner, TraceMode TraceModeP = TraceMode::Off>
class Loggable
{
public:
   Loggable(FmtLog* pLog = nullptr) noexcept
      : m_pLog(pLog == nullptr ? Defaults::Nowhere() : pLog)
   {}

   void SetLogger(FmtLog* pLog)  noexcept {  m_pLog = pLog; }
   FmtLog* GetLogger() noexcept { return m_pLog;  }

   template <typename ...Args>
   void Log(const char* fmtSyntax, Args... args)
   {
      m_pLog->FmtS(static_cast<const Owner*>(this), fmtSyntax, std::forward<Args>(args)...);
   };

private:
   FmtLog* m_pLog;
};

}
