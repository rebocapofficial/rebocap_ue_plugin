#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// 这是一个空壳 UI 类，保留它是为了防止删除文件导致的其他链接错误
// 但里面已经没有任何功能了
class SRebocapLivelinkManagerDemoWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SRebocapLivelinkManagerDemoWidget)
    {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
};