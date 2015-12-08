/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DINPUT_QABSTRACTPHYSICALDEVICEBACKENDNODE_P_H
#define QT3DINPUT_QABSTRACTPHYSICALDEVICEBACKENDNODE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DCore/private/qbackendnode_p.h>
#include <Qt3DCore/qnodeid.h>
#include <QtCore/qvector.h>
#include <Qt3DInput/private/qt3dinput_global_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

class QAxisSetting;
class QInputAspect;

namespace Input {

struct AxisIdSetting
{
    int m_axisIdentifier;
    Qt3DCore::QNodeId m_axisSettingsId;
};

class AxisSetting;

}

class QT3DINPUTSHARED_PRIVATE_EXPORT QAbstractPhysicalDeviceBackendNodePrivate : public Qt3DCore::QBackendNodePrivate
{
public:
    explicit QAbstractPhysicalDeviceBackendNodePrivate(Qt3DCore::QBackendNode::Mode mode = Qt3DCore::QBackendNode::ReadOnly);

    Q_DECLARE_PUBLIC(QAbstractPhysicalDeviceBackendNode)

    void addAxisSetting(int axisIdentifier, Qt3DCore::QNodeId axisSettingId);
    void removeAxisSetting(Qt3DCore::QNodeId axisSettingsId);

    Input::AxisSetting *getAxisSetting(Qt3DCore::QNodeId axisSettingId) const;

    QVector<Input::AxisIdSetting> m_axisSettings;
    QInputAspect *m_inputAspect;
    bool m_enabled;
};

}

QT_END_NAMESPACE

#endif // QT3DINPUT_QABSTRACTPHYSICALDEVICEBACKENDNODE_P_H

