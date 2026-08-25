#include "AnimGraphNode_RebocapA2T.h"
#include "SceneManagement.h"

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

    // 开启镜像时，在拷贝前强制将左侧实时偏移镜像同步至右侧
    if (Node.bMirrorEdit)
    {
        Node.SyncMirrorLimbOffsets();
    }

    // 1. 复制所有旋转偏移数值（实时拖动滑块时生效）
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
    PreviewNode->bMirrorEdit = Node.bMirrorEdit;
    PreviewNode->bMirrorInvertRoll = Node.bMirrorInvertRoll;
    PreviewNode->bMirrorInvertPitch = Node.bMirrorInvertPitch;
    PreviewNode->bMirrorInvertYaw = Node.bMirrorInvertYaw;

    // 2. 仅在骨骼名称实际修改时才更新名称，切勿直接全量拷贝覆盖已初始化的 BoneIndex
    auto CopyBoneNameIfChanged = [](FBoneReference& Dst, const FBoneReference& Src)
    {
        if (Dst.BoneName != Src.BoneName)
        {
            Dst.BoneName = Src.BoneName;
            Dst.BoneIndex = INDEX_NONE;
        }
    };

    CopyBoneNameIfChanged(PreviewNode->LeftClavicle, Node.LeftClavicle);
    CopyBoneNameIfChanged(PreviewNode->LeftUpperArm, Node.LeftUpperArm);
    CopyBoneNameIfChanged(PreviewNode->LeftLowerArm, Node.LeftLowerArm);
    CopyBoneNameIfChanged(PreviewNode->LeftHand, Node.LeftHand);

    CopyBoneNameIfChanged(PreviewNode->RightClavicle, Node.RightClavicle);
    CopyBoneNameIfChanged(PreviewNode->RightUpperArm, Node.RightUpperArm);
    CopyBoneNameIfChanged(PreviewNode->RightLowerArm, Node.RightLowerArm);
    CopyBoneNameIfChanged(PreviewNode->RightHand, Node.RightHand);

    CopyBoneNameIfChanged(PreviewNode->LeftThigh, Node.LeftThigh);
    CopyBoneNameIfChanged(PreviewNode->LeftCalf, Node.LeftCalf);
    CopyBoneNameIfChanged(PreviewNode->LeftFoot, Node.LeftFoot);

    CopyBoneNameIfChanged(PreviewNode->RightThigh, Node.RightThigh);
    CopyBoneNameIfChanged(PreviewNode->RightCalf, Node.RightCalf);
    CopyBoneNameIfChanged(PreviewNode->RightFoot, Node.RightFoot);

    CopyBoneNameIfChanged(PreviewNode->Pelvis, Node.Pelvis);
    CopyBoneNameIfChanged(PreviewNode->Spine, Node.Spine);
    CopyBoneNameIfChanged(PreviewNode->Chest, Node.Chest);
    CopyBoneNameIfChanged(PreviewNode->UpChest, Node.UpChest);
    CopyBoneNameIfChanged(PreviewNode->Neck, Node.Neck);
    CopyBoneNameIfChanged(PreviewNode->Head, Node.Head);
}

void UAnimGraphNode_RebocapA2T::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    const FName MemberName = PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, PresetTemplate))
    {
        Node.ApplyPreset(Node.PresetTemplate);
    }
    else if (Node.bMirrorEdit)
    {
        Node.SyncMirrorLimbOffsets();
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
        TEXT("导出 Rebocap A2T 配置文件"),
        DefaultPath,
        DefaultFile,
        TEXT("JSON 文件 (*.json)|*.json"),
        EFileDialogFlags::None,
        OutFilenames);

    if (bSaved && OutFilenames.Num() > 0)
    {
        const FString SaveFilePath = OutFilenames[0];
        const FString JsonString = Node.ToJsonString();
        if (FFileHelper::SaveStringToFile(JsonString, *SaveFilePath))
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

// 辅助骨骼链绘制
static void DrawA2TBoneChain(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp, const TArray<FName>& BoneNames, const FLinearColor& JointColor, const FLinearColor& LinkColor, float JointRadius, float LinkThickness)
{
    if (!PDI || !SkelMeshComp) return;
    
    FVector PrevLoc = FVector::ZeroVector;
    bool bHasPrev = false;

    for (const FName& BoneName : BoneNames)
    {
        if (BoneName == NAME_None)
        {
            bHasPrev = false;
            continue;
        }

        int32 BoneIndex = SkelMeshComp->GetBoneIndex(BoneName);
        if (BoneIndex == INDEX_NONE)
        {
            bHasPrev = false;
            continue;
        }

        FVector BoneLoc = SkelMeshComp->GetBoneLocation(BoneName, EBoneSpaces::WorldSpace);
        
        // 绘制受控关节发光球体
        DrawWireSphere(PDI, BoneLoc, JointColor, JointRadius, 12, SDPG_Foreground, 1.0f);

        // 绘制骨骼父子连接线
        if (bHasPrev)
        {
            PDI->DrawLine(PrevLoc, BoneLoc, LinkColor, SDPG_Foreground, LinkThickness);
        }

        PrevLoc = BoneLoc;
        bHasPrev = true;
    }
}

// 辅助双骨骼连线
static void DrawA2TLink(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp, const FName& BoneA, const FName& BoneB, const FLinearColor& LinkColor, float LinkThickness)
{
    if (!PDI || !SkelMeshComp || BoneA == NAME_None || BoneB == NAME_None) return;
    if (SkelMeshComp->GetBoneIndex(BoneA) == INDEX_NONE || SkelMeshComp->GetBoneIndex(BoneB) == INDEX_NONE) return;

    FVector PosA = SkelMeshComp->GetBoneLocation(BoneA, EBoneSpaces::WorldSpace);
    FVector PosB = SkelMeshComp->GetBoneLocation(BoneB, EBoneSpaces::WorldSpace);
    PDI->DrawLine(PosA, PosB, LinkColor, SDPG_Foreground, LinkThickness);
}

void UAnimGraphNode_RebocapA2T::Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
    Super::Draw(PDI, PreviewSkelMeshComp);

    if (!bShowControlledBones || !PDI || !PreviewSkelMeshComp) return;

    // 1. 左上肢链 (Left Arm)
    TArray<FName> LeftArmChain;
    LeftArmChain.Add(Node.LeftClavicle.BoneName);
    LeftArmChain.Add(Node.LeftUpperArm.BoneName);
    LeftArmChain.Add(Node.LeftLowerArm.BoneName);
    LeftArmChain.Add(Node.LeftHand.BoneName);
    DrawA2TBoneChain(PDI, PreviewSkelMeshComp, LeftArmChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 2. 右上肢链 (Right Arm)
    TArray<FName> RightArmChain;
    RightArmChain.Add(Node.RightClavicle.BoneName);
    RightArmChain.Add(Node.RightUpperArm.BoneName);
    RightArmChain.Add(Node.RightLowerArm.BoneName);
    RightArmChain.Add(Node.RightHand.BoneName);
    DrawA2TBoneChain(PDI, PreviewSkelMeshComp, RightArmChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 3. 左下肢链 (Left Leg)
    TArray<FName> LeftLegChain;
    LeftLegChain.Add(Node.LeftThigh.BoneName);
    LeftLegChain.Add(Node.LeftCalf.BoneName);
    LeftLegChain.Add(Node.LeftFoot.BoneName);
    DrawA2TBoneChain(PDI, PreviewSkelMeshComp, LeftLegChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 4. 右下肢链 (Right Leg)
    TArray<FName> RightLegChain;
    RightLegChain.Add(Node.RightThigh.BoneName);
    RightLegChain.Add(Node.RightCalf.BoneName);
    RightLegChain.Add(Node.RightFoot.BoneName);
    DrawA2TBoneChain(PDI, PreviewSkelMeshComp, RightLegChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 5. 躯干与头部链 (Spine & Head)
    TArray<FName> TorsoChain;
    TorsoChain.Add(Node.Pelvis.BoneName);
    TorsoChain.Add(Node.Spine.BoneName);
    TorsoChain.Add(Node.Chest.BoneName);
    TorsoChain.Add(Node.UpChest.BoneName);
    TorsoChain.Add(Node.Neck.BoneName);
    TorsoChain.Add(Node.Head.BoneName);
    DrawA2TBoneChain(PDI, PreviewSkelMeshComp, TorsoChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 6. 肩部与胯部连接线 (Cross Links)
    FName UpperTorsoBone = (Node.UpChest.BoneName != NAME_None && PreviewSkelMeshComp->GetBoneIndex(Node.UpChest.BoneName) != INDEX_NONE) ? Node.UpChest.BoneName : Node.Chest.BoneName;
    DrawA2TLink(PDI, PreviewSkelMeshComp, UpperTorsoBone, Node.LeftClavicle.BoneName, LinkColor, LinkThickness);
    DrawA2TLink(PDI, PreviewSkelMeshComp, UpperTorsoBone, Node.RightClavicle.BoneName, LinkColor, LinkThickness);
    DrawA2TLink(PDI, PreviewSkelMeshComp, Node.Pelvis.BoneName, Node.LeftThigh.BoneName, LinkColor, LinkThickness);
    DrawA2TLink(PDI, PreviewSkelMeshComp, Node.Pelvis.BoneName, Node.RightThigh.BoneName, LinkColor, LinkThickness);
}

#undef LOCTEXT_NAMESPACE
