#include <qtrpc.h>

N::RPC::Client:: Client       ( QObject * parent )
               : QObject      (           parent )
               , QScriptable  (                  )
               , Thread       (                  )
               , TcpConnector (                  )
{
  setParameter ( "Role" , "Commando" ) ;
}

N::RPC::Client::~Client(void)
{
}

void N::RPC::Client::run(int Type,ThreadData * data)
{ Q_UNUSED     ( data              ) ;
  AtChannel    ( Type              ) ;
  setParameter ( "Running" , false ) ;
}

void N::RPC::Client::run(void)
{
  AtChannel    ( 3                 ) ;
  setParameter ( "Running" , false ) ;
}

void N::RPC::Client::Start(void)
{
  start ( ) ;
}

void N::RPC::Client::Start(int Type)
{
  start ( Type ) ;
}

void N::RPC::Client::SetParameter (QString key,QVariant value)
{
  setParameter ( key , value ) ;
}

QVariant & N::RPC::Client::Value(QString key,QVariant value)
{
  if ( ! Parameters . contains ( key ) ) {
    Parameters [ key ] = QVariant ( )    ;
  }                                      ;
  return Parameters [ key ]              ;
}

void N::RPC::Client::setAddress(QString address)
{
  Address = N::IpAddress ( address ) ;
}

void N::RPC::Client::Send(QString address,QString command)
{
  Address . setAddress ( address )                          ;
  if ( NULL == this -> Interpreter )                        {
    this -> Interpreter = new N::RPC::Interpreter ( NULL ) ;
  }                                                         ;
  this -> Interpreter -> Place ( command )                  ;
  this -> Interpreter -> setParameter ( "Method" , "Shot" ) ;
  setParameter ( "Block" , true )                           ;
  setParameter ( "State" , 0    )                           ;
  WriteChannel (                )                           ;
}

void N::RPC::Client::Emit(QString command)
{
  if ( NULL == this -> Interpreter )                        {
    this -> Interpreter = new N::RPC::Interpreter ( NULL ) ;
  }                                                         ;
  this -> Interpreter -> Place ( command )                  ;
}

QScriptValue N::RPC::ClientAttachement(QScriptContext * context,QScriptEngine * engine)
{
  N::RPC::Client      * client                 ;
  N::RPC::Interpreter * intp                   ;
  client  = new N::RPC::Client      ( engine ) ;
  intp    = new N::RPC::Interpreter ( NULL   ) ;
  return engine -> newQObject       ( client ) ;
}
