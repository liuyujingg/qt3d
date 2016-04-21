/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "layerfilternode_p.h"
#include "qlayerfilter.h"
#include <Qt3DRender/private/qlayerfilter_p.h>
#include <Qt3DCore/qnodepropertychange.h>
#include <Qt3DRender/private/stringtoint_p.h>

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {
namespace Render {

LayerFilterNode::LayerFilterNode()
    : FrameGraphNode(FrameGraphNode::LayerFilter)
{
}

void LayerFilterNode::updateFromPeer(Qt3DCore::QNode *peer)
{
    QLayerFilter *layerFilter = static_cast<QLayerFilter *>(peer);
    setLayers(layerFilter->layers());
}

void LayerFilterNode::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{
    FrameGraphNode::initializeFromPeer(change);
    const auto typedChange = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<QLayerFilterData>>(change);
    const auto &data = typedChange->data;
    setLayers(data.layers);
}

void LayerFilterNode::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    if (e->type() == NodeUpdated) {
        QNodePropertyChangePtr propertyChange = qSharedPointerCast<QNodePropertyChange>(e);
        if (propertyChange->propertyName() == QByteArrayLiteral("layers"))
            setLayers(propertyChange->value().value<QStringList>());
        markDirty(AbstractRenderer::AllDirty);
    }
    FrameGraphNode::sceneChangeEvent(e);
}

QStringList LayerFilterNode::layers() const
{
    return m_layers;
}

void LayerFilterNode::setLayers(const QStringList &list)
{
    m_layers = list;
    m_layerIds.clear();
    m_layerIds.reserve(m_layers.size());
    for (const QString &name : list)
        m_layerIds.push_back(StringToInt::lookupId(name));
}

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE
