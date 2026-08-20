#pragma once

#include "Widgets/SCompoundWidget.h"

class SRebocapSourceEditor final : public SCompoundWidget {
 public:
  DECLARE_DELEGATE_OneParam(FOnRebocapSourceClickConnect, uint16_t);

  SLATE_BEGIN_ARGS(SRebocapSourceEditor) {}
  SLATE_EVENT(FOnRebocapSourceClickConnect, OnRebocapSourceClickConnect)
  SLATE_END_ARGS()

  ~SRebocapSourceEditor() final = default;

  void Construct(const FArguments& Args);

 private:
  uint16_t connect_port_{7690};

  TOptional<uint16> get_connect_port() const { return connect_port_; }

  void set_connect_port(uint16 value) { connect_port_ = value; }

  FOnRebocapSourceClickConnect on_rebocap_source_click_connect_;

  FReply on_rebocap_source_click_connect();
};