/*
 * Copyright (C) by Claudio Cambra <claudio.cambra@nextcloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include "activitylistmodeltestutils.h"

#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FakeRemoteActivityStorage *FakeRemoteActivityStorage::_instance = nullptr;

FakeRemoteActivityStorage* FakeRemoteActivityStorage::instance()
{
    if (!_instance) {
        _instance = new FakeRemoteActivityStorage();
        _instance->init();
    }

    return _instance;
}

void FakeRemoteActivityStorage::destroy()
{
    if (_instance) {
        delete _instance;
    }

    _instance = nullptr;
}

void FakeRemoteActivityStorage::init()
{
    if (!_activityData.isEmpty()) {
        return;
    }

    _metaSuccess = {{QStringLiteral("status"), QStringLiteral("ok")}, {QStringLiteral("statuscode"), 200},
        {QStringLiteral("message"), QStringLiteral("OK")}};

    initActivityData();
}

void FakeRemoteActivityStorage::initActivityData()
{
    // Insert activity data
    for (quint32 i = 0; i <= _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("object_type"), "files");
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("type"), QStringLiteral("file"));
        activity.insert(QStringLiteral("subject"), QStringLiteral("You created %1.txt").arg(i));
        activity.insert(QStringLiteral("message"), QStringLiteral(""));
        activity.insert(QStringLiteral("object_name"), QStringLiteral("%1.txt").arg(i));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/apps/files/img/add-color.svg"));

        _activityData.push_back(activity);

        _startingId++;
    }

    // Insert notification data
    for (quint32 i = 0; i < _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("object_type"), "calendar");
        activity.insert(QStringLiteral("type"), QStringLiteral("calendar-event"));
        activity.insert(
            QStringLiteral("subject"), QStringLiteral("You created event %1 in calendar Events").arg(i));
        activity.insert(QStringLiteral("message"), QStringLiteral(""));
        activity.insert(QStringLiteral("object_name"), QStringLiteral(""));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/core/img/places/calendar.svg"));

        _activityData.push_back(activity);

        _startingId++;
    }

    // Insert notification data
    for (quint32 i = 0; i < _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("object_type"), "chat");
        activity.insert(QStringLiteral("type"), QStringLiteral("chat"));
        activity.insert(QStringLiteral("subject"), QStringLiteral("You have received %1's message").arg(i));
        activity.insert(QStringLiteral("message"), QStringLiteral(""));
        activity.insert(QStringLiteral("object_name"), QStringLiteral(""));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/core/img/places/talk.svg"));

        QJsonArray actionsArray;

        QJsonObject replyAction;
        replyAction.insert(QStringLiteral("label"), QStringLiteral("Reply"));
        replyAction.insert(QStringLiteral("link"), QStringLiteral(""));
        replyAction.insert(QStringLiteral("type"), QStringLiteral("REPLY"));
        replyAction.insert(QStringLiteral("primary"), true);
        actionsArray.push_back(replyAction);

        QJsonObject primaryAction;
        primaryAction.insert(QStringLiteral("label"), QStringLiteral("View chat"));
        primaryAction.insert(QStringLiteral("link"), QStringLiteral("http://cloud.example.de/call/9p4vjdzd"));
        primaryAction.insert(QStringLiteral("type"), QStringLiteral("WEB"));
        primaryAction.insert(QStringLiteral("primary"), false);
        actionsArray.push_back(primaryAction);

        QJsonObject additionalAction;
        additionalAction.insert(QStringLiteral("label"), QStringLiteral("Additional 1"));
        additionalAction.insert(QStringLiteral("link"), QStringLiteral("http://cloud.example.de/call/9p4vjdzd"));
        additionalAction.insert(QStringLiteral("type"), QStringLiteral("POST"));
        additionalAction.insert(QStringLiteral("primary"), false);
        actionsArray.push_back(additionalAction);
        additionalAction.insert(QStringLiteral("label"), QStringLiteral("Additional 2"));
        actionsArray.push_back(additionalAction);

        activity.insert(QStringLiteral("actions"), actionsArray);

        _activityData.push_back(activity);

        _startingId++;
    }

    // Insert notification data
    for (quint32 i = 0; i < _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("object_type"), "room");
        activity.insert(QStringLiteral("type"), QStringLiteral("room"));
        activity.insert(QStringLiteral("subject"), QStringLiteral("You have been invited into room%1").arg(i));
        activity.insert(QStringLiteral("message"), QStringLiteral(""));
        activity.insert(QStringLiteral("object_name"), QStringLiteral(""));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/core/img/places/talk.svg"));

        QJsonArray actionsArray;

        QJsonObject replyAction;
        replyAction.insert(QStringLiteral("label"), QStringLiteral("Reply"));
        replyAction.insert(QStringLiteral("link"), QStringLiteral(""));
        replyAction.insert(QStringLiteral("type"), QStringLiteral("REPLY"));
        replyAction.insert(QStringLiteral("primary"), true);
        actionsArray.push_back(replyAction);

        QJsonObject primaryAction;
        primaryAction.insert(QStringLiteral("label"), QStringLiteral("View chat"));
        primaryAction.insert(QStringLiteral("link"), QStringLiteral("http://cloud.example.de/call/9p4vjdzd"));
        primaryAction.insert(QStringLiteral("type"), QStringLiteral("WEB"));
        primaryAction.insert(QStringLiteral("primary"), false);
        actionsArray.push_back(primaryAction);

        activity.insert(QStringLiteral("actions"), actionsArray);

        _activityData.push_back(activity);

        _startingId++;
    }

    // Insert notification data
    for (quint32 i = 0; i < _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("object_type"), "call");
        activity.insert(QStringLiteral("type"), QStringLiteral("call"));
        activity.insert(QStringLiteral("subject"), QStringLiteral("You have missed a %1's call").arg(i));
        activity.insert(QStringLiteral("message"), QStringLiteral(""));
        activity.insert(QStringLiteral("object_name"), QStringLiteral(""));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/core/img/places/talk.svg"));

        QJsonArray actionsArray;

        QJsonObject primaryAction;
        primaryAction.insert(QStringLiteral("label"), QStringLiteral("Call back"));
        primaryAction.insert(QStringLiteral("link"), QStringLiteral("http://cloud.example.de/call/9p4vjdzd"));
        primaryAction.insert(QStringLiteral("type"), QStringLiteral("WEB"));
        primaryAction.insert(QStringLiteral("primary"), true);
        actionsArray.push_back(primaryAction);

        QJsonObject replyAction;
        replyAction.insert(QStringLiteral("label"), QStringLiteral("Reply"));
        replyAction.insert(QStringLiteral("link"), QStringLiteral(""));
        replyAction.insert(QStringLiteral("type"), QStringLiteral("REPLY"));
        replyAction.insert(QStringLiteral("primary"), false);
        actionsArray.push_back(replyAction);

        activity.insert(QStringLiteral("actions"), actionsArray);

        _activityData.push_back(activity);

        _startingId++;
    }

    // Insert notification data
    for (quint32 i = 0; i < _numItemsToInsert; i++) {
        QJsonObject activity;
        activity.insert(QStringLiteral("activity_id"), _startingId);
        activity.insert(QStringLiteral("object_type"), "2fa_id");
        activity.insert(QStringLiteral("subject"), QStringLiteral("Login attempt from 127.0.0.1"));
        activity.insert(QStringLiteral("message"), QStringLiteral("Please apporve or deny the login attempt."));
        activity.insert(QStringLiteral("object_name"), QStringLiteral(""));
        activity.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(Qt::ISODate));
        activity.insert(QStringLiteral("icon"), QStringLiteral("http://example.de/core/img/places/password.svg"));

        QJsonArray actionsArray;

        QJsonObject primaryAction;
        primaryAction.insert(QStringLiteral("label"), QStringLiteral("Approve"));
        primaryAction.insert(QStringLiteral("link"), QStringLiteral("/ocs/v2.php/apps/twofactor_nextcloud_notification/api/v1/attempt/39"));
        primaryAction.insert(QStringLiteral("type"), QStringLiteral("POST"));
        primaryAction.insert(QStringLiteral("primary"), true);
        actionsArray.push_back(primaryAction);

        QJsonObject secondaryAction;
        secondaryAction.insert(QStringLiteral("label"), QStringLiteral("Cancel"));
        secondaryAction.insert(QStringLiteral("link"),
            QString(QStringLiteral("/ocs/v2.php/apps/twofactor_nextcloud_notification/api/v1/attempt/39")));
        secondaryAction.insert(QStringLiteral("type"), QStringLiteral("DELETE"));
        secondaryAction.insert(QStringLiteral("primary"), false);
        actionsArray.push_back(secondaryAction);

        activity.insert(QStringLiteral("actions"), actionsArray);

        _activityData.push_back(activity);

        _startingId++;
    }

    _startingId--;
}

QByteArray FakeRemoteActivityStorage::activityJsonData(const int sinceId, const int limit)
{
    QJsonArray data;

    const auto itFound = std::find_if(
        std::cbegin(_activityData), std::cend(_activityData), [&sinceId](const QJsonValue &currentActivityValue) {
            const auto currentActivityId =
                currentActivityValue.toObject().value(QStringLiteral("activity_id")).toInt();
            return currentActivityId == sinceId;
        });

    const int startIndex = itFound != std::cend(_activityData)
        ? static_cast<int>(std::distance(std::cbegin(_activityData), itFound))
        : -1;

    if (startIndex > 0) {
        for (int dataIndex = startIndex, iteration = 0; dataIndex >= 0 && iteration < limit;
             --dataIndex, ++iteration) {
            if (_activityData[dataIndex].toObject().value(QStringLiteral("activity_id")).toInt()
                > sinceId - limit) {
                data.append(_activityData[dataIndex]);
            }
        }
    }

    QJsonObject root;
    QJsonObject ocs;
    ocs.insert(QStringLiteral("data"), data);
    root.insert(QStringLiteral("ocs"), ocs);

    return QJsonDocument(root).toJson();
}

QJsonValue FakeRemoteActivityStorage::activityById(const int id) const
{
    const auto itFound = std::find_if(
        std::cbegin(_activityData), std::cend(_activityData), [&id](const QJsonValue &currentActivityValue) {
            const auto currentActivityId =
                currentActivityValue.toObject().value(QStringLiteral("activity_id")).toInt();
            return currentActivityId == id;
        });

    if (itFound != std::cend(_activityData)) {
        return (*itFound);
    }

    return {};
}

int FakeRemoteActivityStorage::startingIdLast() const
{
    return _startingId;
}


void TestingALM::startFetchJob()
{
    auto *job = new OCC::JsonApiJob(
        accountState()->account(), QLatin1String("ocs/v2.php/apps/activity/api/v2/activity"), this);
    QObject::connect(this, &TestingALM::activityJobStatusCode, this, &TestingALM::slotProcessReceivedActivities);
    QObject::connect(job, &OCC::JsonApiJob::jsonReceived, this, &TestingALM::activitiesReceived);

    QUrlQuery params;
    params.addQueryItem(QLatin1String("since"), QString::number(currentItem()));
    params.addQueryItem(QLatin1String("limit"), QString::number(50));
    job->addQueryParams(params);

    setAndRefreshCurrentlyFetching(true);
    job->start();
}

void TestingALM::slotProcessReceivedActivities()
{
    if (rowCount() > _numRowsPrev) {
        auto finalListCopy = finalList();
        for (int i = _numRowsPrev; i < rowCount(); ++i) {
            const auto modelIndex = index(i, 0);
            auto activity = finalListCopy.at(modelIndex.row());
            if (activity._links.isEmpty()) {
                const auto activityJsonObject = FakeRemoteActivityStorage::instance()->activityById(activity._id);

                if (!activityJsonObject.isNull()) {
                    // because "_links" are normally populated within the notificationhandler.cpp, which we don't run as part of this unit test, we have to fill them here
                    // TODO: move the logic to populate "_links" to "activitylistmodel.cpp"
                    auto actions = activityJsonObject.toObject().value("actions").toArray();
                    foreach (auto action, actions) {
                        activity._links.append(OCC::ActivityLink::createFomJsonObject(action.toObject()));
                    }

                    finalListCopy[modelIndex.row()] = activity;
                }
            }
        }

        setFinalList(finalListCopy);
    }
    _numRowsPrev = rowCount();
    setAndRefreshCurrentlyFetching(false);
    emit activitiesProcessed();
}
