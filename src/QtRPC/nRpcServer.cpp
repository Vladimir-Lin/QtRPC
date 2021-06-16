#include <qtrpc.h>

N::RPC::Server:: Server      ( QObject * parent )
               : QObject     (           parent )
               , QScriptable (                  )
               , TcpService  (                  )
{
}

N::RPC::Server::~Server(void)
{
  if ( NULL != this -> Finder ) {
    delete this -> Finder       ;
    this -> Finder = NULL       ;
  }                             ;
}

int N::RPC::Server::type(void) const
{
  return 37712439 ;
}

void N::RPC::Server::Start(void)
{
  start ( ) ;
}

void N::RPC::Server::Start(int Type)
{
  start ( Type ) ;
}

void N::RPC::Server::SetParameter (QString key,QVariant value)
{
  setParameter ( key , value ) ;
}

QVariant & N::RPC::Server::Value(QString key,QVariant value)
{
  if ( ! Parameters . contains ( key ) ) {
    Parameters [ key ] = QVariant ( )    ;
  }                                      ;
  return Parameters [ key ]              ;
}

void N::RPC::Server::setCallback(QString callback)
{
  N::RPC::Finder * find = (N::RPC::Finder *)NetServer::Finder ;
  find -> Callback = callback                                 ;
}

void N::RPC::Server::setWelcome(QString welcome)
{
  N::RPC::Finder * find = (N::RPC::Finder *)NetServer::Finder ;
  find -> Welcome = welcome                                   ;
}

void N::RPC::Server::setAddress(QString address)
{
  Address = N::IpAddress ( address ) ;
}

QScriptValue N::RPC::ServerAttachement(QScriptContext * context,QScriptEngine * engine)
{
  N::RPC::Server * server                 ;
  N::RPC::Finder * finder                 ;
  server  = new N::RPC::Server ( engine ) ;
  finder  = new N::RPC::Finder (        ) ;
  server -> setFinder          ( finder ) ;
  finder -> Engine = engine               ;
  return engine -> newQObject  ( server ) ;
}
