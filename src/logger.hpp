
#ifndef QUICKSIM_SIQAD_PLUGIN_LOGGER_HPP
#define QUICKSIM_SIQAD_PLUGIN_LOGGER_HPP

#include <iostream>
#include <ostream>
#include <streambuf>

// null buffer for the null stream, does nothing on overflow request
class null_buff : public std::streambuf
{
  public:
    int overflow(int c) final
    {
        return c;
    }
};

// ostream type object which does nothing
class no_stream : public std::ostream
{
  public:
    no_stream() : std::ostream(&nobuff) {}

  private:
    null_buff nobuff{};
};

class logger
{
  public:
    enum
    {
        MSG,
        CRT,
        WRN,
        DBG
    };

    explicit logger(int verbosity = 0, std::ostream& os = std::cout) : verbosity(verbosity), os(os) {}

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
    const int     verbosity{};
    std::ostream& os;
    no_stream     nos{};
};

#endif  // QUICKSIM_SIQAD_PLUGIN_LOGGER_HPP
