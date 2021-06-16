#include <qtrpc.h>

N::RPC::Finder:: Finder        (void)
               : FindConnector (    )
               , Welcome       ( "" )
               , Callback      ( "" )
               , Engine        (NULL)
{
}

N::RPC::Finder::~Finder(void)
{
}

N::NetConnector * N::RPC::Finder::Connector(void)
{
  Client * client                                         ;
  client  = new Client   ( NULL                         ) ;
  client -> setParameter ( "Role"    , "Accepter"       ) ;
  client -> setParameter ( "Forever" , true             ) ;
  client -> setParameter ( "Timeout" , 15 * 1000 * 1000 ) ;
  return client                                           ;
}

N::NetProtocol  * N::RPC::Finder::Protocol(void)
{
  Interpreter * intp                               ;
  intp  = new Interpreter ( NULL )                 ;
  if ( Welcome . length ( ) > 0 )                  {
    intp ->Place ( Welcome )                       ;
  }                                                ;
  if ( Callback . length ( ) > 0 )                 {
    intp -> setEngine    ( Engine                ) ;
    intp -> setParameter ( "Callback" , Callback ) ;
    intp -> setParameter ( "Parser"   , "Engine" ) ;
  }                                                ;
  return intp                                      ;
}

N::NetConnector * N::RPC::Finder::Allocator (void)
{
  N::NetConnector * client              ;
  client                = Connector ( ) ;
  client -> Interpreter = Protocol  ( ) ;
  return client                         ;
}
