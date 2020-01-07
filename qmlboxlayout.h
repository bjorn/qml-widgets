/*
 * qmlboxlayout.h
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

#pragma once

#include <QBoxLayout>
#include <QQmlListProperty>
#include <QWidget>
#include <QDebug>

class QmlBoxLayout : public QBoxLayout
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "data")

public:
    using QBoxLayout::QBoxLayout;

    QQmlListProperty<QObject> data()
    {
        return QQmlListProperty<QObject>(this, nullptr, QmlBoxLayout::data_append,
                                         QmlBoxLayout::data_count,
                                         QmlBoxLayout::data_at,
                                         QmlBoxLayout::data_clear);
    }

private:
    static void data_append(QQmlListProperty<QObject> *property, QObject *object)
    {
        QmlBoxLayout *self = static_cast<QmlBoxLayout*>(property->object);

        //
        // The QML engine assigns the widget as the parent of the layout
        // before assigning it to the layout property. This causes problems in
        // QLayout::parentWidget because a layout is not supposed to have a
        // widget as a parent unless it is marked as topLevel.
        //
        // Here we work around this by setting the layout on the widget before
        // adding any child widgets or layouts.
        //
        if (QWidget *parentWidget = qobject_cast<QWidget*>(self->parent()))
            if (parentWidget->layout() != self)
                parentWidget->setLayout(self);

        if (QWidget *widget = qobject_cast<QWidget*>(object)) {
            self->addWidget(widget);
        } else if (QLayout *layout = qobject_cast<QLayout*>(object)) {
            //
            // Need to unset the parent that was set by the QML engine to avoid
            // "layout already has a parent" in QLayout::addChildLayout.
            //
            layout->setParent(nullptr);

            self->addLayout(layout);
        } else {
            object->setParent(self);
        }
    }

    static int data_count(QQmlListProperty<QObject> *property)
    {
        QmlBoxLayout *self = static_cast<QmlBoxLayout*>(property->object);
        return self->children().count();
    }

    static QObject *data_at(QQmlListProperty<QObject> *property, int index)
    {
        QmlBoxLayout *self = static_cast<QmlBoxLayout*>(property->object);
        return self->children().at(index);
    }

    static void data_clear(QQmlListProperty<QObject> *property)
    {
        QmlBoxLayout *self = static_cast<QmlBoxLayout*>(property->object);
        while (!self->children().isEmpty())
            self->children().first()->setParent(nullptr);
    }
};

class QmlVBoxLayout : public QmlBoxLayout
{
    Q_OBJECT
public:
    QmlVBoxLayout() : QmlBoxLayout(TopToBottom) {}
};

class QmlHBoxLayout : public QmlBoxLayout
{
    Q_OBJECT
public:
    QmlHBoxLayout() : QmlBoxLayout(LeftToRight) {}
};
