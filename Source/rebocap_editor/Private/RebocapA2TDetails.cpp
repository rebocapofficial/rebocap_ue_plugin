#include "RebocapA2TDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SHorizontalBox.h"
#include "AnimGraphNode_RebocapA2T.h"

#define LOCTEXT_NAMESPACE "RebocapA2TDetails"

void FRebocapA2TDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

    IDetailCategoryBuilder& PresetCategory = DetailBuilder.EditCategory(
        "0. Presets & Control",
        LOCTEXT("A2TPresetCategoryName", "0. Presets & Control"),
        ECategoryPriority::Important
    );

    // 添加操作按钮行：导出 JSON、导入 JSON
    PresetCategory.AddCustomRow(LOCTEXT("A2TPresetActionsFilter", "Preset Actions Export Import JSON A2T"))
    .WholeRowContent()
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(0.0f, 4.0f, 4.0f, 4.0f)
        [
            SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "Button")
            .ContentPadding(FMargin(10.0f, 4.0f))
            .ToolTipText(LOCTEXT("ExportA2TJsonTooltip", "Export current A2T calibration angles and bone mappings to JSON. / 导出当前 A2T 肢体偏转与骨骼配置为 JSON 文件。"))
            .OnClicked_Lambda([ObjectsBeingCustomized]() -> FReply {
                for (auto& WeakObj : ObjectsBeingCustomized)
                {
                    if (UAnimGraphNode_RebocapA2T* A2TNode = Cast<UAnimGraphNode_RebocapA2T>(WeakObj.Get()))
                    {
                        A2TNode->ExportToJson();
                        break;
                    }
                }
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ExportA2TJsonButton", "📤 Export to JSON"))
                .Font(FAppStyle::GetFontStyle(TEXT("NormalFontBold")))
            ]
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(4.0f, 4.0f, 0.0f, 4.0f)
        [
            SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "Button")
            .ContentPadding(FMargin(10.0f, 4.0f))
            .ToolTipText(LOCTEXT("ImportA2TJsonTooltip", "Import A2T calibration JSON configuration to auto-fill limb angles and bone mappings. / 从 JSON 文件导入 A2T 姿态配置。"))
            .OnClicked_Lambda([ObjectsBeingCustomized, &DetailBuilder]() -> FReply {
                for (auto& WeakObj : ObjectsBeingCustomized)
                {
                    if (UAnimGraphNode_RebocapA2T* A2TNode = Cast<UAnimGraphNode_RebocapA2T>(WeakObj.Get()))
                    {
                        A2TNode->ImportFromJson();
                    }
                }
                DetailBuilder.ForceRefreshDetails();
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ImportA2TJsonButton", "📥 Import from JSON"))
                .Font(FAppStyle::GetFontStyle(TEXT("NormalFontBold")))
            ]
        ]
    ];
}

#undef LOCTEXT_NAMESPACE
