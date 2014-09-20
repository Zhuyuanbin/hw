#include <QLibrary>
#include <QtQml>
#include <QDebug>

#include "hwengine.h"

extern "C" {
    RunEngine_t *RunEngine;
    registerIPCCallback_t *registerIPCCallback;
    ipcToEngine_t *ipcToEngine;
    flibInit_t *flibInit;
}
HWEngine::HWEngine(QObject *parent) :
    QObject(parent)
{
    QLibrary hwlib("./libhwengine.so");

    if(!hwlib.load())
        qWarning() << "Engine library not found" << hwlib.errorString();

    RunEngine = (RunEngine_t*) hwlib.resolve("RunEngine");
    registerIPCCallback = (registerIPCCallback_t*) hwlib.resolve("registerIPCCallback");
    ipcToEngine = (ipcToEngine_t*) hwlib.resolve("ipcToEngine");
    flibInit = (flibInit_t*) hwlib.resolve("flibInit");

    flibInit();
    registerIPCCallback(this, &engineMessageCallback);
}

HWEngine::~HWEngine()
{

}

void HWEngine::run()
{
    m_argsList.clear();
    m_argsList << "";
    m_argsList << "--internal";
    //m_argsList << "--user-prefix";
    //m_argsList << cfgdir->absolutePath();
    //m_argsList << "--prefix";
    //m_argsList << datadir->absolutePath();
    m_argsList << "--landpreview";

    m_args.resize(m_argsList.size());
    for(int i = m_argsList.size() - 1; i >=0; --i)
        m_args[i] = m_argsList[i].constData();

    RunEngine(m_args.size(), m_args.data());
    sendIPC("!");
}

static QObject *hwengine_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    HWEngine *hwengine = new HWEngine();
    return hwengine;
}

void HWEngine::exposeToQML()
{
    qDebug("HWEngine::exposeToQML");
    qmlRegisterSingletonType<HWEngine>("Hedgewars.Engine", 1, 0, "HWEngine", hwengine_singletontype_provider);
}

void HWEngine::sendIPC(const QByteArray & b)
{
    quint8 len = b.size() > 255 ? 255 : b.size();
    qDebug() << "sendIPC: len = " << len;

    ipcToEngine(len, b.constData());
}

void HWEngine::engineMessageCallback(void *context, quint8 len, const char *msg)
{
    QByteArray b = QByteArray::fromRawData(msg, len);

    qDebug() << "FLIPC in" << b;
}
