/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#ifndef JSONDBPARTITION_H
#define JSONDBPARTITION_H


#include <QObject>
#include <QScopedPointer>
#include <QMap>
#include <QPointer>
#include <QJSValue>
#include <QJSEngine>

#include "jsondb-client.h"

class JsonDatabase;
class JsonDbNotify;
class JsonDbPartitionPrivate;
class JsonDbQueryObject;
class JsonDbChangesSinceObject;

Q_USE_JSONDB_NAMESPACE

class JsonDbPartition: public QObject
{
    Q_OBJECT
public:
    JsonDbPartition(const QString &partitionName=QString(), QObject *parent=0);
    ~JsonDbPartition();

    Q_PROPERTY(QString name READ name WRITE setName)

    Q_INVOKABLE int create(const QJSValue &object,
                           const QJSValue &options = QJSValue(),
                           const QJSValue &callback = QJSValue());

    Q_INVOKABLE int update(const QJSValue &object,
                           const QJSValue &options = QJSValue(),
                           const QJSValue &callback = QJSValue());

    Q_INVOKABLE int remove(const QJSValue &object,
                           const QJSValue &options = QJSValue(),
                           const QJSValue &callback = QJSValue());

    Q_INVOKABLE int find(const QJSValue &query,
                         const QJSValue &options = QJSValue(),
                         const QJSValue &callback = QJSValue());

    Q_INVOKABLE int changesSince(int stateNumber,
                                 const QJSValue &types = QJSValue(),
                                 const QJSValue &callback = QJSValue());

    Q_INVOKABLE JsonDbNotify* createNotification(const QJSValue &query,
                                                 const QJSValue &actions,
                                                 QObject *parentItem);

    Q_INVOKABLE JsonDbQueryObject* createQuery(const QJSValue &query,
                                               int offset,
                                               int limit,
                                               QObject *parentItem);

    Q_INVOKABLE JsonDbChangesSinceObject* createChangesSince(int stateNumber,
                                                             const QStringList &types,
                                                             QObject *parentItem);

    QString name() const;
    void setName(const QString &partitionName);

Q_SIGNALS:
    void nameChanged(const QString &partitionName);

private:
    QString _name;
    QString _file;
    QString _uuid;
    JsonDbClient jsonDb;

    QMap<int, QJSValue> createCallbacks;
    QMap<int, QJSValue> updateCallbacks;
    QMap<int, QJSValue> removeCallbacks;
    QMap<int, QJSValue> findCallbacks;
    QMap<int, QJSValue> changesCallbacks;
    QMap<int, QPointer<JsonDbNotify> > newNotifications;
    QMap<int, QPointer<JsonDbNotify> > notificationsToRemove;
    QMap<QString, QPointer<JsonDbNotify> > notifications;

    void updateNotification(JsonDbNotify *notify);
    void removeNotification(JsonDbNotify *notify);

    void call(QMap<int, QJSValue> &callbacks, int id, const QVariant &result);
    void callChangesSince(QMap<int, QJSValue> &callbacks, int id, const QVariant &result);
    void callFindCallback(QMap<int, QJSValue> &callbacks, int id, const QVariant &result);
    void callErrorCallback(QMap<int, QJSValue> &callbacks, int id, int code, const QString &message);

private Q_SLOTS:
    void dbResponse(int id, const QVariant &result);
    void dbErrorResponse(int id, int code, const QString &message);
    void dbNotified(const QString& currentNotifyUuid,const QVariant &v,
                       const QString &action);

    friend class JsonDatabase;
    friend class JsonDbNotify;
    friend class JsonDbQueryObject;
    friend class JsonDbChangesSinceObject;
};

#endif