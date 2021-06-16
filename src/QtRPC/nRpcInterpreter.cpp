#include <qtrpc.h>

N::RPC::Interpreter:: Interpreter ( QObject * parent )
                    : QObject     (           parent )
                    , QScriptable (                  )
                    , NetProtocol (                  )
                    , Parser      ( NULL             )
                    , engine      ( NULL             )
{
  setParameter ( "Parser" , "Signal" )              ;
  setParameter ( "State"  , 2        )              ;
  nConnect ( this , SIGNAL(pCommand(int,QString))   ,
             this , SLOT  (eCommand(int,QString)) ) ;
}

N::RPC::Interpreter::~Interpreter(void)
{
}

int N::RPC::Interpreter::type(void) const
{
  return 37712439 ;
}

bool N::RPC::Interpreter::In(int size,char * data)
{
  Buffers [ Pending ] . append ( data , size ) ;
  return Analysis ( )                          ;
}

bool N::RPC::Interpreter::In(QByteArray & data)
{
  Buffers [ Pending ] . append ( data ) ;
  return Analysis ( )                   ;
}

bool N::RPC::Interpreter::Out(QByteArray & data)
{
  Buffers [ Output ] . append ( data ) ;
  return Analysis ( )                  ;
}

bool N::RPC::Interpreter::Actions(void)
{
  if ( ! Parameters [ "Running" ] . toBool ( ) ) return false ;
  Analysis ( )                                                ;
  return Parameters [ "Running" ] . toBool ( )                ;
}

bool N::RPC::Interpreter::Analysis(void)
{
  if ( Buffers [ Pending ] . size () > 0 )               {
    Buffers [ Input   ] . append ( Buffers [ Pending ] ) ;
    Buffers [ Pending ] . clear  (                     ) ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  QByteArray LINE                                        ;
  int        code = 0                                    ;
  if ( ( Buffers [ Input  ] . size ( ) == 0 )           &&
       ( Buffers [ Output ] . size ( ) == 0 )          ) {
    Interpret ( code , LINE )                            ;
  }                                                      ;
  while ( ( code = Response ( ) ) > 0 )                  {
    Take      ( LINE        )                            ;
    Tail      ( LINE        )                            ;
    Interpret ( code , LINE )                            ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  return true                                            ;
}

bool N::RPC::Interpreter::Interpret(int code,QByteArray & line)
{
  QString method                                        ;
  if ( Parameters.contains("Parser") )                  {
    method = Parameters["Parser"].toString()            ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  setParameter ( "State" , 2 )                          ;
  if ( code == 0 )                                      {
    if ( method == "Shot" )                             {
      setParameter ( "State" , 0 )                      ;
    }                                                   ;
    return true                                         ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  QString s = QString::fromUtf8(line)                   ;
  ///////////////////////////////////////////////////////
  if ( ( method == "Function" ) && ( NULL != Parser ) ) {
    QString answer                                      ;
    if ( Parser(code,s,answer) )                        {
      if ( answer . length ( ) > 0 ) Place ( answer )   ;
      return true                                       ;
    } else return false                                 ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  if ( method == "Signal" )                             {
    emit pCommand ( code , s )                          ;
    return true                                         ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  if ( method == "Engine" )                             {
    QString answer = ScriptEngine ( code , s )          ;
    if ( answer . length ( ) > 0 ) Place ( answer )     ;
    return true                                         ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  return true                                           ;
}

void N::RPC::Interpreter::setEngine(QScriptEngine * e)
{
  engine = e ;
}

void N::RPC::Interpreter::setParser(CommandParser parser)
{
  setParameter ( "Parser" , "Function" ) ;
  Parser = parser                        ;
}

void N::RPC::Interpreter::eCommand(int code,QString command)
{
  emit Commanding ( code , command ) ;
}

QString N::RPC::Interpreter::ScriptEngine(int code,QString command)
{
  if ( NULL == engine ) return ""                             ;
  if ( ! Parameters . contains ( "Callback" ) ) return ""     ;
  QString callback = Parameters [ "Callback" ] . toString ( ) ;
  if ( callback . length ( ) <= 0 ) return ""                 ;
  QScriptValue global = engine  -> globalObject (          )  ;
  QScriptValue eval   = global   . property     ( callback )  ;
  QString      answer = ""                                    ;
  if ( eval . isFunction ( ) )                                {
    QScriptValue     r                                        ;
    QScriptValueList args                                     ;
    args  << QScriptValue     ( code                    )     ;
    args  << QScriptValue     ( command                 )     ;
    r       = eval . call     ( QScriptValue ( ) , args )     ;
    answer  = r    . toString (                         )     ;
    engine -> collectGarbage  (                         )     ;
  }                                                           ;
  return answer                                               ;
}
