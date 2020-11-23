/*************************************************************************
 * DeepTags, Markdown Notes Manager
 * Copyright (C) 2020  Zineddine Saibi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/
#include "tagitem.h"
#include <QApplication>
#include "settings.h"

#define USER_TYPE_NUM 1500

TagItem::TagItem(const QString& label, QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent, QStringList(label), USER_TYPE_NUM),
      m_elements(new QVector<Element*>()), m_special(false), m_pinned(false) {}

TagItem::TagItem(const QString& label, QTreeWidget* parent)
    : QTreeWidgetItem(parent, QStringList(label), USER_TYPE_NUM),
      m_elements(new QVector<Element*>()), m_special(false), m_pinned(false) {}

TagItem::TagItem(const QString& label, const QString& icon, bool special, QTreeWidget* parent)
    : QTreeWidgetItem(parent, QStringList(label), USER_TYPE_NUM),
      m_elements(new QVector<Element*>()), m_special(special), m_pinned(false) {
    if (!icon.isEmpty()) setIcon(0, QIcon(icon));
}

TagItem::TagItem(const TagItem& other) : QTreeWidgetItem(other) {
    m_special  = other.m_special;
    m_pinned   = other.m_pinned;
    m_elements = new QVector<Element*>(*other.m_elements);
}

TagItem::TagItem(TagItem&& other) : QTreeWidgetItem(other) {
    m_special        = other.m_special;
    m_pinned         = other.m_pinned;
    m_elements       = other.m_elements;
    other.m_elements = nullptr;
}

TagItem& TagItem::operator=(TagItem& other) {
    m_special  = other.m_special;
    m_pinned   = other.m_pinned;
    m_elements = other.m_elements;
    return *this;
}

TagItem& TagItem::operator=(const TagItem& other) {
    m_special  = other.m_special;
    m_pinned   = other.m_pinned;
    m_elements = other.m_elements;
    return *this;
}

TagItem& TagItem::operator=(TagItem&& other) {
    m_special        = other.m_special;
    m_pinned         = other.m_pinned;
    m_elements       = other.m_elements;
    other.m_elements = nullptr;
    return *this;
}

TagItem::~TagItem() {
    if (m_elements) delete m_elements;
}


void TagItem::removeElement(Element* element) {
    for (int i = 0; i < m_elements->size(); i++)
        if (*m_elements->at(i) == *element) m_elements->remove(i);
}


bool TagItem::empty() const {
    if (!m_elements->empty()) return false;
    auto els = std::make_unique<QVector<Element*>>(*allElements());
    return els->empty();
}


bool TagItem::contains(Element* e) {
    auto els = std::make_unique<QVector<Element*>>(*allElements());
    return allElements()->contains(e);
}


void TagItem::setColor(const QString& color) {
    if (color.isEmpty())
        setForeground(0, qApp->palette().text());
    else {
        QBrush brush(foreground(0));
        brush.setColor(color);
        setForeground(0, brush);
    }
    Settings::setTagItemColor(label(), color);
}

void TagItem::setPinned(bool pinned) {
    m_pinned = pinned;
    QFont f  = font(0);
    f.setBold(pinned);
    setFont(0, f);
    if (pinned)
        Settings::setTagPinned(label());
    else
        Settings::setTagUnpinned(label());
}


QVector<Element*>* TagItem::allElements() const {
    auto res           = new QVector<Element*>(*m_elements);
    auto elsInChildren = [](TagItem* ti) -> QVector<Element*> {   // return the elements of children
        QVector<Element*> qv;
        if (ti->hasChildren()) qv += *ti->allElements();
        return qv;
    };
    for (auto& c : children())
        for (auto& e : elsInChildren(c))
            if (!res->contains(e)) res->push_back(e);
    return res;
}

QVector<TagItem*> TagItem::children() const {
    QVector<TagItem*> v;
    for (int i = 0; i < childCount(); i++) v.push_back(static_cast<TagItem*>(child(i)));
    return v;
}
