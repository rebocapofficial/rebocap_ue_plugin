#pragma once

#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "rebocap_body_remap_asset.generated.h"

/** Rebocap 骨骼映射资产，用于将标准 Rebocap 24 骨节点映射到当前骨骼的对应骨骼名称。 */
UCLASS(BlueprintType, meta = (ToolTip = "Rebocap 骨骼映射资产，用于将标准 Rebocap 24 骨节点映射到当前骨骼的对应骨骼名称。"))
class REBOCAP_RUNTIME_API URebocapMapData : public ULiveLinkRemapAsset {
  GENERATED_BODY()
  URebocapMapData();

 public:
  FName GetRemappedBoneName_Implementation(FName CurveName) const final;

  void Initialize() final { InitializeTMap(); }

  UFUNCTION(BlueprintCallable, Category = BoneRemapping)
  void InitializeTMap();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  TMap<FName, FName> name_mapping_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName rebocap_pelvis_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName spine1_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName spine2_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName spine3_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_foot_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_foot_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName neck_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName head_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName l_hand_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
  FName r_hand_;
};