#include "rebocap_source_editor.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "RebocapSourceEditor"

void SRebocapSourceEditor::Construct(const FArguments& Args) {
  on_rebocap_source_click_connect_ = Args._OnRebocapSourceClickConnect;
  ChildSlot[SNew(SBox).HeightOverride(200).WidthOverride(
      200)[SNew(SVerticalBox) +
           SVerticalBox::Slot().AutoHeight()
               [SNew(SHorizontalBox) +
                SHorizontalBox::Slot().HAlign(HAlign_Left).FillWidth(0.3f)[SNew(STextBlock).Text(LOCTEXT("Port", "Port"))] +
                SHorizontalBox::Slot()
                    .HAlign(HAlign_Fill)
                    .FillWidth(0.4f)[SNew(SNumericEntryBox<uint16>)
                                         .IsEnabled(true)
                                         .AllowSpin(true)
                                         .MinSliderValue(0)
                                         .MaxSliderValue(65535)
                                         .MinDesiredValueWidth(75)
                                         .Value_Raw(this, &SRebocapSourceEditor::get_connect_port)
                                         .OnValueChanged_Raw(this, &SRebocapSourceEditor::set_connect_port)] +
                SHorizontalBox::Slot()
                    .HAlign(HAlign_Fill)
                    .FillWidth(0.3f)[SNew(SHorizontalBox) +
                                     SHorizontalBox::Slot().FillWidth(
                                         0.5f)[SNew(SButton)
                                                   .Text(LOCTEXT("Connect", "Connect"))
                                                   .OnClicked_Raw(this, &SRebocapSourceEditor::on_rebocap_source_click_connect)]]]]];
}

FReply SRebocapSourceEditor::on_rebocap_source_click_connect() {
  on_rebocap_source_click_connect_.ExecuteIfBound(connect_port_);
  return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE