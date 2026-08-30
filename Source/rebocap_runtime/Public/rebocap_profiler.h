#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformTime.h"

struct FRebocapSamplePoint {
    float ElapsedTime = 0.0f;
    float RenderFPS = 0.0f;
    float RenderDeltaMs = 0.0f;
    float MocapHz = 0.0f;
    bool bConnected = false;
};

struct FRebocapBoneMappingInfo {
    FString MeshName;
    FString SkeletonName;
    int32 BoneCount = 0;
    int32 LODLevel = 0;
    FString RetargetAssetName;
    TMap<FString, FString> BoneMap; // e.g. "Pelvis" -> "pelvis"
};

struct FRebocapNodeConfigSnapshot {
    bool bAutoSkeleton = true;
    bool bAutoConnect = true;
    bool bEnableInterpolation = false;
    float InterpolationSpeed = 45.0f;
    bool bHoldPoseOnDropout = true;
    float DropoutTimeout = 2.0f;
    bool bThrottleSkeletonRegistration = false;
    bool bZeroAllocStaticSubject = false;
    bool bOptimizeFrameEvaluation = false;
};

class REBOCAP_RUNTIME_API FRebocapProfiler {
public:
    static FRebocapProfiler& Get();

    /** 启动诊断录制（默认 10.0 秒） */
    void StartRecording(float InDurationSeconds = 10.0f);

    /** 停止录制并生成报告 */
    void StopRecordingAndGenerateReport();

    /** 检查是否正在录制 */
    bool IsRecording() const { return bIsRecording; }

    /** 获取当前录制剩余时间（秒） */
    float GetRemainingTime() const;

    /** 获取录制进度百分比 0.0 ~ 1.0 */
    float GetProgress() const;

    /** 每帧更新采样（在动画或主线程 Tick 中调用） */
    void SampleFrame(float DeltaSeconds, float CurrentMocapHz, bool bIsConnected);

    /** 记录动捕数据包到达时刻（用于网络抖动深度分析） */
    void RecordPacketArrival(double Timestamp);

    /** 更新角色骨骼与重定向映射表上下文 */
    void UpdateSkeletalContext(const FRebocapBoneMappingInfo& InInfo);

    /** 更新动画节点运行配置快照 */
    void UpdateNodeConfig(const FRebocapNodeConfigSnapshot& InConfig);

    /** 获取最近一次生成的诊断报告文件绝对路径 */
    FString GetLastReportFilePath() const { return LastReportFilePath; }

private:
    FRebocapProfiler();
    ~FRebocapProfiler();

    FString GenerateReportString();

    bool bIsRecording = false;
    float TargetDuration = 10.0f;
    double RecordingStartTime = 0.0;
    double LastSampleTime = 0.0;

    TArray<FRebocapSamplePoint> Samples;
    TArray<double> PacketArrivalTimes;

    FRebocapBoneMappingInfo CachedBoneInfo;
    bool bHasBoneInfo = false;

    FRebocapNodeConfigSnapshot CachedNodeConfig;
    bool bHasNodeConfig = false;

    FString LastReportFilePath;
};
