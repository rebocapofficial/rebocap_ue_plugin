#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

/**
 * UAnimGraphNode_RebocapA2T 节点细节面板自定义 (添加 JSON 导出/导入按钮)
 */
class FRebocapA2TDetails : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance()
    {
        return MakeShareable(new FRebocapA2TDetails);
    }

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
