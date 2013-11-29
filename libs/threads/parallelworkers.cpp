/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-01-13
 * Description : Multithreaded worker object
 *
 * Copyright (C) 2010-2012 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "parallelworkers.h"

// Qt includes

#include <QCoreApplication>
#include <QEvent>
#include <QMetaMethod>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

// KDE includes

#include <kdebug.h>

// Local includes

#include "threadmanager.h"

namespace Digikam
{

ParallelWorkers::ParallelWorkers()
    : m_currentIndex(0),
      m_replacementMetaObject(0)
{
}

ParallelWorkers::~ParallelWorkers()
{
    foreach(WorkerObject* const object, m_workers)
    {
        delete object;
    }
    delete m_replacementMetaObject;
}

int ParallelWorkers::optimalWorkerCount()
{
    return qMax(1, QThread::idealThreadCount());
}

bool ParallelWorkers::optimalWorkerCountReached() const
{
    return m_workers.size() >= optimalWorkerCount();
}

void ParallelWorkers::schedule()
{
    foreach(WorkerObject* const object, m_workers)
    {
        object->schedule();
    }
}

void ParallelWorkers::deactivate(WorkerObject::DeactivatingMode mode)
{
    foreach(WorkerObject* const object, m_workers)
    {
        object->deactivate(mode);
    }
}

void ParallelWorkers::wait()
{
    foreach(WorkerObject* const object, m_workers)
    {
        object->wait();
    }
}

void ParallelWorkers::setPriority(QThread::Priority priority)
{
    foreach(WorkerObject* const object, m_workers)
    {
        object->setPriority(priority);
    }
}

void ParallelWorkers::add(WorkerObject* const worker)
{
    /*
    if (!asQObject()->inherits(worker->metaObject()->className()))
    {
        kError() << "You need to derive the ParallelWorkers class from the WorkerObject you want to use";
        return;
    }

    QMetaObject* meta = asQObject()->metaObject();
    for (int i=0; i<meta->methodCount(); i++)
    {
        QMetaMethod method = meta->method(index);
        if (!method->methodType() == QMetaMethod::
    }
    */

    m_workers << worker;
}

/*bool ParallelWorkers::connect(const QObject* sender, const char* signal,
                              const char* method,
                              Qt::ConnectionType type) const
{
    foreach(WorkerObject* object, m_workers)
    {
        if (!WorkerObject::connect(sender, signal, object, method, type))
        {
            return false;
        }
    }
    return true;
}*/

bool ParallelWorkers::connect(const char* const signal,
                              const QObject* const receiver, const char* const method,
                              Qt::ConnectionType type) const
{
    foreach(WorkerObject* const object, m_workers)
    {
        if (!QObject::connect(object, signal, receiver, method, type))
        {
            return false;
        }
    }
    return true;
}

int ParallelWorkers::replacementQtMetacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WorkerObjectQtMetacall(_c, _id, _a);
    if (_id < 0)
        return _id;

    if (_c == QMetaObject::InvokeMetaMethod)
    {
        // This is the common ancestor's meta object, below WorkerObject
        const QMetaObject* mobj = mocMetaObject();
        const int properMethods = mobj->methodCount() - mobj->methodOffset();

        if (_id >= properMethods)
        {
            return _id - properMethods;
        }

        // Get the relevant meta method. I'm not quite sure if this is rock solid.
        QMetaMethod method = mobj->method(_id + mobj->methodOffset());

        // Copy the argument data - _a is going to be deleted in our current thread
        QList<QByteArray> types = method.parameterTypes();
        QVector<QGenericArgument> args(10);
        for (int i = 0; i<types.size(); i++)
        {
            int typeId = QMetaType::type(types[i]);
            if (!typeId && _a[i+1])
            {
                kWarning() << "Unable to handle unregistered datatype" << types[i] << "Dropping signal.";
                return _id - properMethods;
            }
            // we use QMetaType to copy the data. _a[0] is reserved for a return parameter.
            void* data = QMetaType::construct(typeId, _a[i+1]);
            args[i] = QGenericArgument(types[i], data);
        }

        // Find the object to be invoked
        WorkerObject* obj = m_workers.at(m_currentIndex);
        if (++m_currentIndex == m_workers.size())
        {
            m_currentIndex = 0;
        }
        obj->schedule();

        // Invoke across-thread
        method.invoke(obj, Qt::QueuedConnection,
                      args[0],
                      args[1],
                      args[2],
                      args[3],
                      args[4],
                      args[5],
                      args[6],
                      args[7],
                      args[8],
                      args[9]);

        _id -= properMethods;
    }
    return _id;
}

const QMetaObject *ParallelWorkers::replacementMetaObject() const
{
    if (!m_replacementMetaObject)
    {
        // We skip the extraData added in Qt 4.8, which gives the static_qt_metacall, bypassing
        // our overriding of the qt_metacall virtual
        QMetaObject *rmo = new QMetaObject(*mocMetaObject());
        rmo->d.extradata = 0;
        const_cast<ParallelWorkers*>(this)->m_replacementMetaObject = rmo;
    }
    return m_replacementMetaObject;
}

} // namespace Digikam