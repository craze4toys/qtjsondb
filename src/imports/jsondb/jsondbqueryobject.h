/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtAddOn.JsonDb module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef JSONDBQUERYOBJECT_H
#define JSONDBQUERYOBJECT_H

#include <QObject>
#include <QVariant>
#include <QPointer>
#include <QJSValue>
#include <QDeclarativeParserStatus>
#include <QDeclarativeListProperty>
#include "jsondb-client.h"

class JsonDbPartition;
class JsonDbPartitionPrivate;

QT_USE_NAMESPACE_JSONDB

class JsonDbQueryObject : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)

public:
    Q_ENUMS(Status)
    enum Status { Null, Loading, Ready, Error };

    Q_PROPERTY(QString query READ query WRITE setQuery)
    Q_PROPERTY(JsonDbPartition* partition READ partition WRITE setPartition)
    Q_PROPERTY(quint32 stateNumber READ stateNumber)

    Q_PROPERTY(int limit READ limit WRITE setLimit)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantMap error READ error NOTIFY errorChanged)
    Q_PROPERTY(QVariantMap bindings READ bindings WRITE setBindings)

    JsonDbQueryObject(QObject *parent = 0);
    ~JsonDbQueryObject();

    QString query();
    void setQuery(const QString &newQuery);

    JsonDbPartition* partition();
    void setPartition(JsonDbPartition* newPartition);

    quint32 stateNumber() const;

    int limit();
    void setLimit(int newLimit);

    JsonDbQueryObject::Status status() const;
    QVariantMap error() const;

    QVariantMap bindings() const;
    void setBindings(const QVariantMap &newBindings);

    void classBegin() {}
    void componentComplete();

    Q_INVOKABLE int start();
    Q_INVOKABLE QVariantList takeResults();

Q_SIGNALS:
    void resultsReady(int resultsAvailable);
    void finished();
    void statusChanged(JsonDbQueryObject::Status newStatus);
    void errorChanged(const QVariantMap &newError);

private Q_SLOTS:
    void setError(QtAddOn::JsonDb::JsonDbError::ErrorCode code, const QString& message);
    void setReadyStatus();

private:
    bool completed;
    int queryLimit;
    QString queryString;
    QVariantList results;
    QPointer<JsonDbPartition> partitionObject;
    QPointer<JsonDbPartition> defaultPartitionObject;
    QPointer<JsonDbQuery> jsondbQuery;
    int errorCode;
    QString errorString;
    Status objectStatus;
    QVariantMap queryBindings;

    void clearError();
    inline bool parametersReady();
    void checkForReadyStatus();
    friend class JsonDbPartition;
    friend class JsonDbPartitionPrivate;
};

#endif //JSONDBQUERYOBJECT_H
