/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_RPC_H
#define QT_RPC_H

#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <Essentials>
#include <NetProtocol>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTRPC_LIB)
#      define Q_RPC_EXPORT Q_DECL_EXPORT
#    else
#      define Q_RPC_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_RPC_EXPORT
#endif

namespace N   {
namespace RPC {

class Q_RPC_EXPORT Finder      ;
class Q_RPC_EXPORT Client      ;
class Q_RPC_EXPORT Server      ;
class Q_RPC_EXPORT Interpreter ;

typedef bool (*CommandParser)(int code,QString line,QString & response) ;

class Q_RPC_EXPORT Finder : public FindConnector
{
  public:

    QString         Welcome  ;
    QString         Callback ;
    QScriptEngine * Engine   ;

    explicit               Finder    (void) ;
    virtual               ~Finder    (void) ;

    virtual NetConnector * Allocator (void) ;
    virtual NetConnector * Connector (void) ;
    virtual NetProtocol  * Protocol  (void) ;

  protected:

  private:

};

class Q_RPC_EXPORT Client : public QObject
                          , public QScriptable
                          , public Thread
                          , public TcpConnector
{
  Q_OBJECT
  public:

    explicit           Client       (QObject * parent = NULL) ;
    virtual           ~Client       (void) ;

  protected:

    virtual void       run          (int Type,ThreadData * Data) ;
    virtual void       run          (void) ;

  private:

  public slots:

    virtual void       Start        (void) ;
    virtual void       Start        (int Type) ;
    virtual void       SetParameter (QString key,QVariant value) ;
    virtual QVariant & Value        (QString key,QVariant value) ;

    virtual void       setAddress   (QString address) ;
    virtual void       Send         (QString address,QString command) ;
    virtual void       Emit         (QString command) ;

  protected slots:

  private slots:

  signals:

};

class Q_RPC_EXPORT Server : public QObject
                          , public QScriptable
                          , public TcpService
{
  Q_OBJECT
  public:

    explicit           Server       (QObject * parent = NULL) ;
    virtual           ~Server       (void) ;

    virtual int        type         (void) const ;

  protected:

  private:

  public slots:

    virtual void       Start        (void) ;
    virtual void       Start        (int Type) ;
    virtual void       SetParameter (QString key,QVariant value) ;
    virtual QVariant & Value        (QString key,QVariant value) ;
    virtual void       setCallback  (QString callback) ;
    virtual void       setWelcome   (QString welcome) ;
    virtual void       setAddress   (QString address) ;

  protected slots:

  private slots:

  signals:

};

class Q_RPC_EXPORT Interpreter : public QObject
                               , public QScriptable
                               , public NetProtocol
{
  Q_OBJECT
  public:

    explicit     Interpreter (QObject * parent = NULL) ;
    virtual     ~Interpreter (void) ;

    virtual int  type        (void) const ;

    virtual bool In          (int size,char * data) ;
    virtual bool In          (QByteArray & data) ;
    virtual bool Out         (QByteArray & data) ;

    virtual bool Actions     (void) ;
    virtual bool Interpret   (int code,QByteArray & line) ;

    void         setEngine   (QScriptEngine * engine) ;

  protected:

    CommandParser   Parser ;
    QScriptEngine * engine ;

    virtual bool Analysis    (void) ;

  private:

    QString ScriptEngine     (int code,QString command) ;

  public slots:

    void         setParser   (CommandParser parser) ;

  protected slots:

  private slots:

    void         eCommand    (int code,QString command) ;

  signals:

    void         pCommand    (int code,QString command) ;
    void         Commanding  (int code,QString command) ;

};

Q_RPC_EXPORT QScriptValue ServerAttachement(QScriptContext * context,QScriptEngine * engine) ;
Q_RPC_EXPORT QScriptValue ClientAttachement(QScriptContext * context,QScriptEngine * engine) ;

}
}

Q_DECLARE_METATYPE(N::RPC::Finder)

QT_END_NAMESPACE

#endif
