/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef widget_gtk_CompositorWidgetParent_h
#define widget_gtk_CompositorWidgetParent_h

#include "X11CompositorWidget.h"
#include "mozilla/widget/PCompositorWidgetParent.h"

namespace mozilla {
namespace widget {

class CompositorWidgetParent final
#ifdef MOZ_X11
 : public PCompositorWidgetParent,
   public X11CompositorWidget
#else
 : public PCompositorWidgetParent,
   public CompositorWidget
#endif
{
public:
  explicit CompositorWidgetParent(const CompositorWidgetInitData& aInitData);
  ~CompositorWidgetParent() override;

  void ActorDestroy(ActorDestroyReason aWhy) override { }

#ifndef MOZ_X11
  LayoutDeviceIntSize GetClientSize() override
  {
    return LayoutDeviceIntSize();
  }
  nsIWidget* RealWidget() override { return nullptr; }
  void NotifyClientSizeChanged(const LayoutDeviceIntSize& aClientSize) { }
#endif

  void ObserveVsync(VsyncObserver* aObserver) override;
  RefPtr<VsyncObserver> GetVsyncObserver() const override;

  bool RecvNotifyClientSizeChanged(const LayoutDeviceIntSize& aClientSize) override;

private:
  RefPtr<VsyncObserver> mVsyncObserver;
};

} // namespace widget
} // namespace mozilla

#endif // widget_gtk_CompositorWidgetParent_h
