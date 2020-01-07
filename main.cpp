/*
 * main.cpp
 * Copyright 2019, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
 *
 * This file is part of the QtWidgets QML module.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "qmlboxlayout.h"
#include "qmlmainwindow.h"
#include "qmlwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include <qboxlayout.h>

static void registerTypes()
{
    qmlRegisterUncreatableType<QWidget>("QtWidgets", 5, 12, "QWidget", "Use Widget instead");
    qmlRegisterType<QmlWidget>("QtWidgets", 5, 12, "Widget");
    qmlRegisterUncreatableType<QLayout>("QtWidgets", 5, 12, "Layout", "Abstract class");
    qmlRegisterType<QmlVBoxLayout>("QtWidgets", 5, 12, "VBoxLayout");
    qmlRegisterType<QmlHBoxLayout>("QtWidgets", 5, 12, "HBoxLayout");
    qmlRegisterType<QmlMainWindow>("QtWidgets", 5, 12, "MainWindow");
    qmlRegisterType<QPushButton>("QtWidgets", 5, 12, "PushButton");
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    registerTypes();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
