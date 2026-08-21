#include "AnimGraphNode_RebocapA2T.h"

#if WITH_EDITOR
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/MessageDialog.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#endif

#define LOCTEXT_NAMESPACE "RebocapA2TAnimGraphNode"

FText UAnimGraphNode_RebocapA2T::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RebocapA2TNodeTitle", "Rebocap A-Pose to T-Pose");
}

FText UAnimGraphNode_RebocapA2T::GetTooltipText() const
{
    return LOCTEXT("RebocapA2TNodeTooltip", "Calibrates A-Pose characters into standard T-Pose for Rebocap motion capture.");
}

FText UAnimGraphNode_RebocapA2T::GetMenuCategory() const
{
    return LOCTEXT("RebocapA2TCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapA2T::GetKeywords() const
{
    return LOCTEXT("RebocapA2TKeywords", "rebocap, a2t, tpose, a-pose, t-pose, calibrate, limb, mocap, live link");
}

void UAnimGraphNode_RebocapA2T::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode)
{
    FAnimNode_RebocapA2T* PreviewNode = static_cast<FAnimNode_RebocapA2T*>(InPreviewNode);

    PreviewNode->LeftClavicleOffset = Node.LeftClavicleOffset;
    PreviewNode->LeftUpperArmOffset = Node.LeftUpperArmOffset;
    PreviewNode->LeftLowerArmOffset = Node.LeftLowerArmOffset;
    PreviewNode->LeftHandOffset = Node.LeftHandOffset;

    PreviewNode->RightClavicleOffset = Node.RightClavicleOffset;
    PreviewNode->RightUpperArmOffset = Node.RightUpperArmOffset;
    PreviewNode->RightLowerArmOffset = Node.RightLowerArmOffset;
    PreviewNode->RightHandOffset = Node.RightHandOffset;

    PreviewNode->LeftThighOffset = Node.LeftThighOffset;
    PreviewNode->LeftCalfOffset = Node.LeftCalfOffset;
    PreviewNode->LeftFootOffset = Node.LeftFootOffset;

    PreviewNode->RightThighOffset = Node.RightThighOffset;
    PreviewNode->RightCalfOffset = Node.RightCalfOffset;
    PreviewNode->RightFootOffset = Node.RightFootOffset;

    PreviewNode->PelvisOffset = Node.PelvisOffset;
    PreviewNode->SpineOffset = Node.SpineOffset;
    PreviewNode->ChestOffset = Node.ChestOffset;
    PreviewNode->UpChestOffset = Node.UpChestOffset;
    PreviewNode->NeckOffset = Node.NeckOffset;
    PreviewNode->HeadOffset = Node.HeadOffset;

    PreviewNode->Alpha = Node.Alpha;
}

void UAnimGraphNode_RebocapA2T::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // 1. 预设切换：自动应用对应角色的 A-Pose 旋转数据
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, PresetTemplate))
    {
        if (Node.PresetTemplate != ERebocapA2TPreset::Custom)
        {
            Node.ApplyPreset(Node.PresetTemplate);
        }
        return;
    }

    // 2. 对称修改：只要开启 bMirrorEdit，根据用户勾选的 XYZ 正负轴向规则自动镜像同步到右侧
    if (Node.bMirrorEdit)
    {
        const double MultRoll  = Node.bMirrorInvertRoll  ? -1.0 : 1.0;
        const double MultPitch = Node.bMirrorInvertPitch ? -1.0 : 1.0;
        const double MultYaw   = Node.bMirrorInvertYaw   ? -1.0 : 1.0;

        auto MirrorRot = [&](const FRotator& InRot) -> FRotator
        {
            return FRotator(InRot.Pitch * MultPitch, InRot.Yaw * MultYaw, InRot.Roll * MultRoll);
        };

        Node.RightClavicleOffset = MirrorRot(Node.LeftClavicleOffset);
        Node.RightUpperArmOffset = MirrorRot(Node.LeftUpperArmOffset);
        Node.RightLowerArmOffset = MirrorRot(Node.LeftLowerArmOffset);
        Node.RightHandOffset     = MirrorRot(Node.LeftHandOffset);

        Node.RightThighOffset    = MirrorRot(Node.LeftThighOffset);
        Node.RightCalfOffset     = MirrorRot(Node.LeftCalfOffset);
        Node.RightFootOffset     = MirrorRot(Node.LeftFootOffset);
    }
}

void UAnimGraphNode_RebocapA2T::ExportToJson()
{
#if WITH_EDITOR
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return;

    const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    const FString DefaultPath = FPaths::ProjectSavedDir();
    const FString DefaultFile = TEXT("Rebocap_A2T_Config.json");
    TArray<FString> OutFilenames;

    const bool bSaved = DesktopPlatform->SaveFileDialog(
        ParentWindowHandle,
        TEXT("导出 Rebocap A2T 姿态校准为 JSON 文件"),
        DefaultPath,
        DefaultFile,
        TEXT("JSON 文件 (*.json)|*.json"),
        EFileDialogFlags::None,
        OutFilenames);

    if (bSaved && OutFilenames.Num() > 0)
    {
        const FString SaveFilePath = OutFilenames[0];
        const FString JsonContent = Node.ToJsonString();
        if (FFileHelper::SaveStringToFile(JsonContent, *SaveFilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
        {
            UE_LOG(LogTemp, Display, TEXT("成功导出 A2T 配置至: %s"), *SaveFilePath);
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("成功导出 A2T 配置至：\n%s"), *SaveFilePath)));
        }
    }
#endif
}

void UAnimGraphNode_RebocapA2T::ImportFromJson()
{
#if WITH_EDITOR
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return;

    const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    const FString DefaultPath = FPaths::ProjectSavedDir();
    TArray<FString> OutFilenames;

    const bool bOpened = DesktopPlatform->OpenFileDialog(
        ParentWindowHandle,
        TEXT("选择要导入的 Rebocap A2T 配置文件"),
        DefaultPath,
        TEXT(""),
        TEXT("JSON 文件 (*.json)|*.json"),
        EFileDialogFlags::None,
        OutFilenames);

    if (bOpened && OutFilenames.Num() > 0)
    {
        const FString OpenFilePath = OutFilenames[0];
        FString JsonString;
        if (FFileHelper::LoadFileToString(JsonString, *OpenFilePath))
        {
            if (Node.FromJsonString(JsonString))
            {
                MarkPackageDirty();
                UE_LOG(LogTemp, Display, TEXT("成功导入 A2T 配置: %s"), *OpenFilePath);
                FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("成功导入 A2T 配置！\n已还原所有肢体旋转与骨骼绑定。"))));
            }
            else
            {
                FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("导入失败：JSON 格式不符合 A2T 规范！")));
            }
        }
    }
#endif
}

#undef LOCTEXT_NAMESPACE
