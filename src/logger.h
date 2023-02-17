
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <ostream>
#include <streambuf>

// null buffer for the null stream, does nothing on overflow request
class NullBuff : public std::streambuf
{
  public:
    int overflow(int c) final
    {
        return c;
    }
};

// ostream type object which does nothing
class nostream : public std::ostream
{
  public:
    nostream() : std::ostream(&nobuff) {}

  private:
    NullBuff nobuff;
};

class Logger
{
  public:
    enum
    {
        MSG,
        CRT,
        WRN,
        DBG
    };

    explicit Logger(int verbosity = 0, std::ostream& os = std::cout) : verbosity(verbosity), os(os) {}

    std::ostream& debug()
    {
        return verbosity >= DBG ? os : nos;
    }

    std::ostream& warning()
    {
        return verbosity >= WRN ? os : nos;
    }

    std::ostream& critical()
    {
        return verbosity >= CRT ? os : nos;
    }

    std::ostream& echo()
    {
        return verbosity >= MSG ? os : nos;
    }

  private:
    int           verbosity;
    std::ostream& os;
    nostream      nos;
};

#endif
