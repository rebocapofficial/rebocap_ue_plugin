#include "RebocapMapDataDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "rebocap_body_remap_asset.h"

void FRebocapMapDataDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

    IDetailCategoryBuilder& PresetCategory = DetailBuilder.EditCategory(
        "0. 快捷骨骼预设 (Preset Template)",
        FText::FromString(TEXT("0. 快捷骨骼预设 (Preset Template)")),
        ECategoryPriority::Important
    );

    // 添加操作按钮行：导出 JSON、导入 JSON
    PresetCategory.AddCustomRow(FText::FromString(TEXT("PresetActions")))
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
            .ToolTipText(FText::FromString(TEXT("将当前 24 根骨骼名称及 12 个脚底顶点导出为 JSON 文件（完全兼容 Blender 插件格式）。")))
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
                .Text(FText::FromString(TEXT("📤 导出为 JSON")))
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
            .ToolTipText(FText::FromString(TEXT("选择并导入 JSON 配置文件，自动填充骨骼名称及脚底顶点（完全兼容 Blender 插件格式）。")))
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
                .Text(FText::FromString(TEXT("📥 从 JSON 导入")))
                .Font(FAppStyle::GetFontStyle(TEXT("NormalFontBold")))
            ]
        ]
    ];
}
