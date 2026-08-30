#include "AnimGraphNode_RebocapPose.h"
#include "SceneManagement.h"
#include "rebocap_body_remap_asset.h"
#include "rebocap_skeleton_data.h"
#include "rebocap_source.h"

#define LOCTEXT_NAMESPACE "RebocapAnimGraphNode"

FText UAnimGraphNode_RebocapPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (TitleType == ENodeTitleType::ListView)
    {
        return LOCTEXT("RebocapPoseNodeTitle", "Rebocap Body Pose");
    }

    auto Source = FRebocapSource::GetInstance();
    if (Source.IsValid() && Source->IsPortOpen())
    {
        float Hz = Source->GetMocapHz();
        if (Hz > 0.0f)
        {
            return FText::FromString(FString::Printf(TEXT("Rebocap Body Pose [%.1f Hz]"), Hz));
        }
        return LOCTEXT("RebocapPoseNodeTitleActive", "Rebocap Body Pose [已连接]");
    }
    return LOCTEXT("RebocapPoseNodeTitle", "Rebocap Body Pose");
}

FText UAnimGraphNode_RebocapPose::GetTooltipText() const
{
    auto Source = FRebocapSource::GetInstance();
    if (Source.IsValid() && Source->IsPortOpen())
    {
        return FText::FromString(FString::Printf(TEXT("Rebocap 动捕驱动节点\n● 当前状态: 已连接 (Active)\n● 动捕接收频率: %.1f Hz\n● 骨骼自动回传: %s\n● 帧平滑插值: %s"),
            Source->GetMocapHz(),
            Node.bAutoSkeleton ? TEXT("开启") : TEXT("关闭"),
            Node.bEnableInterpolation ? TEXT("开启") : TEXT("关闭")));
    }
    return LOCTEXT("RebocapPoseNodeTooltip", "Rebocap 动捕驱动节点\n○ 当前状态: 未连接 (Disconnected)\n通过骨骼重定向映射表驱动角色的 LiveLink 动作捕捉数据。");
}

FText UAnimGraphNode_RebocapPose::GetMenuCategory() const
{
    return LOCTEXT("RebocapPoseCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapPose::GetKeywords() const
{
    return LOCTEXT("RebocapPoseKeywords", "rebocap, mocap, live link, pose, skeleton, motion capture");
}

static void DrawPoseBoneChain(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp, const TArray<FName>& BoneNames, const FLinearColor& JointColor, const FLinearColor& LinkColor, float JointRadius, float LinkThickness)
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

static void DrawPoseLink(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp, const FName& BoneA, const FName& BoneB, const FLinearColor& LinkColor, float LinkThickness)
{
    if (!PDI || !SkelMeshComp || BoneA == NAME_None || BoneB == NAME_None) return;
    if (SkelMeshComp->GetBoneIndex(BoneA) == INDEX_NONE || SkelMeshComp->GetBoneIndex(BoneB) == INDEX_NONE) return;

    FVector PosA = SkelMeshComp->GetBoneLocation(BoneA, EBoneSpaces::WorldSpace);
    FVector PosB = SkelMeshComp->GetBoneLocation(BoneB, EBoneSpaces::WorldSpace);
    PDI->DrawLine(PosA, PosB, LinkColor, SDPG_Foreground, LinkThickness);
}

void UAnimGraphNode_RebocapPose::Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
    Super::Draw(PDI, PreviewSkelMeshComp);

    if (!bShowControlledBones || !PDI || !PreviewSkelMeshComp) return;

    // 解析当前生效的骨骼映射表
    FName PelvisName = Node.bone_map_.Pelvis.BoneName;
    FName Spine1Name = Node.bone_map_.Spine1.BoneName;
    FName Spine2Name = Node.bone_map_.Spine2.BoneName;
    FName Spine3Name = Node.bone_map_.Spine3.BoneName;
    FName NeckName = Node.bone_map_.Neck.BoneName;
    FName HeadName = Node.bone_map_.Head.BoneName;

    FName L_CollarName = Node.bone_map_.L_Collar.BoneName;
    FName L_ShoulderName = Node.bone_map_.L_Shoulder.BoneName;
    FName L_ElbowName = Node.bone_map_.L_Elbow.BoneName;
    FName L_WristName = Node.bone_map_.L_Wrist.BoneName;
    FName L_HandName = Node.bone_map_.L_Hand.BoneName;

    FName R_CollarName = Node.bone_map_.R_Collar.BoneName;
    FName R_ShoulderName = Node.bone_map_.R_Shoulder.BoneName;
    FName R_ElbowName = Node.bone_map_.R_Elbow.BoneName;
    FName R_WristName = Node.bone_map_.R_Wrist.BoneName;
    FName R_HandName = Node.bone_map_.R_Hand.BoneName;

    FName L_HipName = Node.bone_map_.L_Hip.BoneName;
    FName L_KneeName = Node.bone_map_.L_Knee.BoneName;
    FName L_AnkleName = Node.bone_map_.L_Ankle.BoneName;
    FName L_FootName = Node.bone_map_.L_Foot.BoneName;

    FName R_HipName = Node.bone_map_.R_Hip.BoneName;
    FName R_KneeName = Node.bone_map_.R_Knee.BoneName;
    FName R_AnkleName = Node.bone_map_.R_Ankle.BoneName;
    FName R_FootName = Node.bone_map_.R_Foot.BoneName;

    if (Node.retarget_asset_)
    {
        URebocapMapData* MapData = Node.retarget_asset_->GetDefaultObject<URebocapMapData>();
        if (MapData)
        {
            PelvisName = MapData->GetRemappedBoneName(rebocap_bones::pelvis);
            Spine1Name = MapData->GetRemappedBoneName(rebocap_bones::spine1);
            Spine2Name = MapData->GetRemappedBoneName(rebocap_bones::spine2);
            Spine3Name = MapData->GetRemappedBoneName(rebocap_bones::spine3);
            NeckName = MapData->GetRemappedBoneName(rebocap_bones::neck);
            HeadName = MapData->GetRemappedBoneName(rebocap_bones::head);

            L_CollarName = MapData->GetRemappedBoneName(rebocap_bones::l_collar);
            L_ShoulderName = MapData->GetRemappedBoneName(rebocap_bones::l_shoulder);
            L_ElbowName = MapData->GetRemappedBoneName(rebocap_bones::l_elbow);
            L_WristName = MapData->GetRemappedBoneName(rebocap_bones::l_wrist);
            L_HandName = MapData->GetRemappedBoneName(rebocap_bones::l_hand);

            R_CollarName = MapData->GetRemappedBoneName(rebocap_bones::r_collar);
            R_ShoulderName = MapData->GetRemappedBoneName(rebocap_bones::r_shoulder);
            R_ElbowName = MapData->GetRemappedBoneName(rebocap_bones::r_elbow);
            R_WristName = MapData->GetRemappedBoneName(rebocap_bones::r_wrist);
            R_HandName = MapData->GetRemappedBoneName(rebocap_bones::r_hand);

            L_HipName = MapData->GetRemappedBoneName(rebocap_bones::l_hip);
            L_KneeName = MapData->GetRemappedBoneName(rebocap_bones::l_knee);
            L_AnkleName = MapData->GetRemappedBoneName(rebocap_bones::l_ankle);
            L_FootName = MapData->GetRemappedBoneName(rebocap_bones::l_foot);

            R_HipName = MapData->GetRemappedBoneName(rebocap_bones::r_hip);
            R_KneeName = MapData->GetRemappedBoneName(rebocap_bones::r_knee);
            R_AnkleName = MapData->GetRemappedBoneName(rebocap_bones::r_ankle);
            R_FootName = MapData->GetRemappedBoneName(rebocap_bones::r_foot);
        }
    }

    // 1. 左上肢链 (Left Arm)
    TArray<FName> LeftArmChain;
    LeftArmChain.Add(L_CollarName);
    LeftArmChain.Add(L_ShoulderName);
    LeftArmChain.Add(L_ElbowName);
    LeftArmChain.Add(L_WristName);
    LeftArmChain.Add(L_HandName);
    DrawPoseBoneChain(PDI, PreviewSkelMeshComp, LeftArmChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 2. 右上肢链 (Right Arm)
    TArray<FName> RightArmChain;
    RightArmChain.Add(R_CollarName);
    RightArmChain.Add(R_ShoulderName);
    RightArmChain.Add(R_ElbowName);
    RightArmChain.Add(R_WristName);
    RightArmChain.Add(R_HandName);
    DrawPoseBoneChain(PDI, PreviewSkelMeshComp, RightArmChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 3. 左下肢链 (Left Leg)
    TArray<FName> LeftLegChain;
    LeftLegChain.Add(L_HipName);
    LeftLegChain.Add(L_KneeName);
    LeftLegChain.Add(L_AnkleName);
    LeftLegChain.Add(L_FootName);
    DrawPoseBoneChain(PDI, PreviewSkelMeshComp, LeftLegChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 4. 右下肢链 (Right Leg)
    TArray<FName> RightLegChain;
    RightLegChain.Add(R_HipName);
    RightLegChain.Add(R_KneeName);
    RightLegChain.Add(R_AnkleName);
    RightLegChain.Add(R_FootName);
    DrawPoseBoneChain(PDI, PreviewSkelMeshComp, RightLegChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 5. 躯干与头部链 (Spine & Head)
    TArray<FName> TorsoChain;
    TorsoChain.Add(PelvisName);
    TorsoChain.Add(Spine1Name);
    TorsoChain.Add(Spine2Name);
    TorsoChain.Add(Spine3Name);
    TorsoChain.Add(NeckName);
    TorsoChain.Add(HeadName);
    DrawPoseBoneChain(PDI, PreviewSkelMeshComp, TorsoChain, JointColor, LinkColor, JointRadius, LinkThickness);

    // 6. 肩部与胯部连接线 (Cross Links)
    FName UpperTorsoBone = (Spine3Name != NAME_None && PreviewSkelMeshComp->GetBoneIndex(Spine3Name) != INDEX_NONE) ? Spine3Name : ((Spine2Name != NAME_None && PreviewSkelMeshComp->GetBoneIndex(Spine2Name) != INDEX_NONE) ? Spine2Name : Spine1Name);
    DrawPoseLink(PDI, PreviewSkelMeshComp, UpperTorsoBone, L_CollarName, LinkColor, LinkThickness);
    DrawPoseLink(PDI, PreviewSkelMeshComp, UpperTorsoBone, R_CollarName, LinkColor, LinkThickness);
    DrawPoseLink(PDI, PreviewSkelMeshComp, PelvisName, L_HipName, LinkColor, LinkThickness);
    DrawPoseLink(PDI, PreviewSkelMeshComp, PelvisName, R_HipName, LinkColor, LinkThickness);
}

#undef LOCTEXT_NAMESPACE
