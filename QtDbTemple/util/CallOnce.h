#ifndef CALL_ONCE_H
#define CALL_ONCE_H

#include <QtGlobal>
#include <QAtomicInt>
#include <QMutex>
#include <QWaitCondition>
#include <QThreadStorage>
#include <QThread>

namespace CallOnce {
    enum ECallOnce {
        CO_Request,
        CO_InProgress,
        CO_Finished
    };
    //Q_GLOBAL_STATIC宏主要用于设置non-POD类型的全局静态变量
    //QThreadStorage用于按线程存储数据，所以对应的不同的线程会存储不同的once_flag值
    Q_GLOBAL_STATIC(QThreadStorage<QAtomicInt*>, once_flag)
}

template <class Function>
inline static void qCallOnce(Function func, QBasicAtomicInt& flag)
//flag是引用，当第一次调用结束后，flag对应的值就会被设为co_finished
{
    using namespace CallOnce;

#if QT_VERSION < 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag);
#elif QT_VERSION >= 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag.load());//将protectFlag初始化为flag
    //当第一次调用结束后，该值就会被设为co_finished
#endif

    if (protectFlag == CO_Finished)
    {
        //qDebug()<<"fi--"<<"flag: "<< flag<<"protectFlag: "<<protectFlag;
        return;
    }

    //如果flag的当前值等于protectFlag(CO_Request),则将CO_InProgress赋给flag并返回true
    if (protectFlag == CO_Request && flag.testAndSetRelaxed(protectFlag,
                                                           CO_InProgress)) {
        func();
        //qDebug()<<"re1--"<<"flag: "<< flag<<"protectFlag: "<<protectFlag;
        flag.fetchAndStoreRelease(CO_Finished);//将CO_Finished赋给flag
        //qDebug()<<"re2--"<<"flag: "<< flag<<"protectFlag: "<<protectFlag;
    }
    else {
        do {
            //将当前线程的执行权让给别的可执行线程,至于让给哪一个可执行线程，由操作系统决定
            //qDebug()<<"ot--"<<"flag: "<< flag<<"protectFlag: "<<protectFlag;
            QThread::yieldCurrentThread();
        }
        //如果flag不等于CO_Finished，则继续执行循环体
        while (!flag.testAndSetAcquire(CO_Finished, CO_Finished));
    }
}

template <class Function>
inline static void qCallOncePerThread(Function func)
{
    using namespace CallOnce;
    if (!once_flag()->hasLocalData()) {//如果once_flag没有在本线程被设置过的话则执行下面语句
        once_flag()->setLocalData(new QAtomicInt(CO_Request));
        qCallOnce(func, *once_flag()->localData());
    }
}

#endif // CALL_ONCE_H
