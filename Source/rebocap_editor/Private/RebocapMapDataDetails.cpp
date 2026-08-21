#include "RebocapMapDataDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "rebocap_body_remap_asset.h"

#define LOCTEXT_NAMESPACE "RebocapMapDataDetails"

void FRebocapMapDataDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

    IDetailCategoryBuilder& PresetCategory = DetailBuilder.EditCategory(
        "0. Presets & Actions",
        LOCTEXT("PresetCategoryName", "0. Presets & Actions"),
        ECategoryPriority::Important
    );

    // 添加操作按钮行：导出 JSON、导入 JSON
    PresetCategory.AddCustomRow(LOCTEXT("PresetActionsFilter", "Preset Actions Export Import JSON"))
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
            .ToolTipText(LOCTEXT("ExportJsonTooltip", "Export current 24 bone names and foot vertices to JSON (fully compatible with Blender plugin format). / 导出当前骨骼配置为 JSON 文件。"))
            .OnClicked_Lambda([ObjectsBeingCustomized]() -> FReply {
                for (auto& WeakObj : ObjectsBeingCustomized)
                {
                    if (URebocapMapData* MapData = Cast<URebocapMapData>(WeakObj.Get()))
                    {
                        MapData->ExportToJson();
                        break;
                    }
                }
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ExportJsonButton", "📤 Export to JSON"))
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
            .ToolTipText(LOCTEXT("ImportJsonTooltip", "Select and import JSON configuration to auto-fill bone names and foot vertices (fully compatible with Blender plugin format). / 从 JSON 文件导入骨骼配置。"))
            .OnClicked_Lambda([ObjectsBeingCustomized, &DetailBuilder]() -> FReply {
                for (auto& WeakObj : ObjectsBeingCustomized)
                {
                    if (URebocapMapData* MapData = Cast<URebocapMapData>(WeakObj.Get()))
                    {
                        MapData->ImportFromJson();
                        MapData->MarkPackageDirty();
                    }
                }
                DetailBuilder.ForceRefreshDetails();
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ImportJsonButton", "📥 Import from JSON"))
                .Font(FAppStyle::GetFontStyle(TEXT("NormalFontBold")))
            ]
        ]
    ];
}

#undef LOCTEXT_NAMESPACE
