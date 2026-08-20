#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

/**
 * RebocapMapData 细节面板自定义 (为数据蓝图类默认值面板添加一键应用与 JSON 导入导出按钮)
 */
class FRebocapMapDataDetails : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance()
    {
        return MakeShareable(new FRebocapMapDataDetails);
    }

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
