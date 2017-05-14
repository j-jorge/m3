#include "m3/app/launcher.hpp"

#include "m3/app/create_level.hpp"

#include "engine/game.hpp"

#include <claw/logger.hpp>

#include <boost/preprocessor/stringize.hpp>

#include <libintl.h>

#ifdef __ANDROID__
#include "engine/resource_pool.hpp"
#include "engine/resource_pool/android_resource_pool.hpp"

#include <claw/android_logger.hpp>
#include <claw/log_stream_uniq.hpp>
#endif

#define m3_launch_argument( arg, value )        \
  arg BOOST_PP_STRINGIZE( value )

namespace m3
{
  namespace app
  {
    namespace detail
    {
      static void initialize_logger();
    }
  }
}

m3::app::launcher::launcher( int& argc, char** &argv )
  : claw::application(argc, argv)
{
  m_arguments.add
    ( "-h", "--help", gettext( "Print this help screen and exit." ),
      true );
  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool( "--help" ) )
    help();
  else
    create_game( argc, argv );
}

m3::app::launcher::~launcher() = default;

int m3::app::launcher::run()
{
  detail::initialize_logger();
  
  try
    {
      if ( m_game != nullptr )
        {
          m_game->set_waiting_level( create_level() );
          m_game->run();
          m_game.reset();
        }

      return EXIT_SUCCESS;
    }
  catch( const std::exception& e )
    {
      claw::logger << claw::log_error << "(exception) " << e.what()
                   << std::endl;

      m_game.reset();
      
      return EXIT_FAILURE;
    }
}

void m3::app::launcher::create_game( int& argc, char** &argv )
{
  const char* default_args[] =
    {
      "--game-name=M3",
      "--screen-width=720",
      "--screen-height=1280",
      "--active-area=250",

#ifdef __ANDROID__
      "--fullscreen",
#else
      m3_launch_argument( "--data-path=", M3_DATA_DIRECTORY ),
#endif

      "--auto-load-symbols",
      "--sync-render",
      nullptr
    };

  int default_argc(0);
  while ( default_args[ default_argc ] != nullptr )
    ++default_argc;

  int final_argc( argc + default_argc );
  const char** final_args = new const char*[ final_argc ];

  for ( int i=0; i!=default_argc; ++i )
    final_args[i] = default_args[i];

  for ( int i=0; i!=argc; ++i )
    final_args[ default_argc + i ] = argv[i];

  try
    {
      char** engine_args = const_cast< char** >(final_args);
      m_game.reset( new bear::engine::game( final_argc, engine_args ) );

#ifdef __ANDROID__
      bear::engine::resource_pool::get_instance().add_pool
        ( new bear::engine::android_resource_pool );                     
#endif
    }
  catch( const std::exception& e )
    {
      std::cerr << "Exception: " << e.what() << std::endl;
      help();
    }

  delete[] final_args;
}

void m3::app::launcher::help() const
{
  m_arguments.help( gettext("engine_options") );
  std::cout << '\n';
  bear::engine::game::print_help();
}

void m3::app::detail::initialize_logger()
{
#ifdef __ANDROID__
  claw::log_stream* const logger
    ( new claw::log_stream_uniq( new claw::android_logger( "M3" ) ) );
  
  claw::logger.set( logger );
  
#ifndef NDEBUG
  claw::logger.set_level( claw::log_verbose );
#else
  claw::logger.set_level( claw::log_error );
#endif

#endif
}
