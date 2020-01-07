/*
 * qmlwidget.h
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

#include <QLayout>
#include <QQmlListProperty>
#include <QWidget>

class QmlWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QLayout* layout READ layout WRITE setLayout)
    Q_PROPERTY(QWidget *parent READ parentWidget WRITE setParent DESIGNABLE false FINAL)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "data")

public:
    QQmlListProperty<QObject> data()
    {
        return QQmlListProperty<QObject>(this, nullptr, QmlWidget::data_append,
                                         QmlWidget::data_count,
                                         QmlWidget::data_at,
                                         QmlWidget::data_clear);
    }

private:
    static void data_append(QQmlListProperty<QObject> *property, QObject *object)
    {
        QmlWidget *self = static_cast<QmlWidget*>(property->object);
        if (QWidget *widget = qobject_cast<QWidget*>(object)) {
            if (QLayout *layout = self->layout())
                layout->addWidget(widget);
            else
                widget->setParent(self);
        } else {
            object->setParent(self);
        }
    }

    static int data_count(QQmlListProperty<QObject> *property)
    {
        QmlWidget *self = static_cast<QmlWidget*>(property->object);
        return self->children().count();
    }

    static QObject *data_at(QQmlListProperty<QObject> *property, int index)
    {
        QmlWidget *self = static_cast<QmlWidget*>(property->object);
        return self->children().at(index);
    }

    static void data_clear(QQmlListProperty<QObject> *property)
    {
        QmlWidget *self = static_cast<QmlWidget*>(property->object);
        while (!self->children().isEmpty())
            self->children().first()->setParent(nullptr);
    }
};
