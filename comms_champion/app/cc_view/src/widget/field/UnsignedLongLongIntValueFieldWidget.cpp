//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "UnsignedLongLongIntValueFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <cmath>

#include "comms_champion/property/field.h"

namespace comms_champion
{

UnsignedLongLongIntValueFieldWidget::UnsignedLongLongIntValueFieldWidget(
    WrapperPtr wrapper,
    QWidget* parentObj)
  : Base(parentObj),
    m_wrapper(std::move(wrapper))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);
    m_ui.m_valueLineEdit->setText(QString("%1").arg(adjustRealToDisplayed(m_wrapper->getValue())));

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_wrapper->minWidth(), m_wrapper->maxWidth());

    connect(m_ui.m_valueLineEdit, SIGNAL(textEdited(const QString&)),
            this, SLOT(valueUpdated(const QString&)));

    connect(m_ui.m_serValueLineEdit, SIGNAL(textEdited(const QString&)),
            this, SLOT(serialisedValueUpdated(const QString&)));

    refresh();
}

UnsignedLongLongIntValueFieldWidget::~UnsignedLongLongIntValueFieldWidget() = default;

void UnsignedLongLongIntValueFieldWidget::refreshImpl()
{
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_wrapper->getSerialisedString());

    auto value = m_wrapper->getValue();
    assert(m_ui.m_valueLineEdit);
    if (adjustDisplayedToReal(getDisplayedValue(m_ui.m_valueLineEdit->text())) != value) {
        m_ui.m_valueLineEdit->setText(QString("%1").arg(adjustRealToDisplayed(value)));
    }

    bool valid = m_wrapper->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void UnsignedLongLongIntValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_valueLineEdit->setReadOnly(readonly);
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void UnsignedLongLongIntValueFieldWidget::updatePropertiesImpl(const QVariantMap& props)
{
    m_offset = property::field::IntValue(props).displayOffset();
    refresh();
}

void UnsignedLongLongIntValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_wrapper);
}

void UnsignedLongLongIntValueFieldWidget::valueUpdated(const QString& value)
{
    auto adjustedValue = adjustDisplayedToReal(getDisplayedValue(value));
    if (adjustedValue == m_wrapper->getValue()) {
        return;
    }

    assert(isEditEnabled());
    m_wrapper->setValue(adjustedValue);
    assert(m_wrapper->getValue() == adjustedValue);
    refresh();
    emitFieldUpdated();
}

UnsignedLongLongIntValueFieldWidget::UnderlyingType
UnsignedLongLongIntValueFieldWidget::adjustDisplayedToReal(DisplayedType val)
{
    return static_cast<UnderlyingType>(val - m_offset);
}

UnsignedLongLongIntValueFieldWidget::DisplayedType
UnsignedLongLongIntValueFieldWidget::adjustRealToDisplayed(UnderlyingType val)
{
    return static_cast<DisplayedType>(val + m_offset);
}

UnsignedLongLongIntValueFieldWidget::DisplayedType
UnsignedLongLongIntValueFieldWidget::getDisplayedValue(const QString& value)
{
    bool ok = false;
    auto val = value.toULongLong(&ok);
    return val;
}

}  // namespace comms_champion


