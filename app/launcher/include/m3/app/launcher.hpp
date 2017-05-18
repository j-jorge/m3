#pragma once

#include <claw/application.hpp>

#include <memory>

namespace bear
{
  namespace engine
  {
    class game;
  }
}

namespace m3
{
  namespace app
  {
    class launcher:
      public claw::application
    {
    public:
      launcher( int argc, char** argv );
      virtual ~launcher();

      virtual int run();

    private:
      void create_game( int argc, char** argv );
      void help() const;

    private:
      std::unique_ptr< bear::engine::game > m_game;

    };
  }
}

