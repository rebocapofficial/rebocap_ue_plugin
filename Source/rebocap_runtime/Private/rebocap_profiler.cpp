#include "rebocap_profiler.h"
#include "rebocap_source.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformMemory.h"
#include "HAL/PlatformMisc.h"
#include "HAL/IConsoleManager.h"
#include "Scalability.h"
#include "Runtime/Launch/Resources/Version.h"

FRebocapProfiler& FRebocapProfiler::Get() {
    static FRebocapProfiler Instance;
    return Instance;
}

FRebocapProfiler::FRebocapProfiler() {
}

FRebocapProfiler::~FRebocapProfiler() {
}

void FRebocapProfiler::StartRecording(float InDurationSeconds) {
    TargetDuration = FMath::Max(1.0f, InDurationSeconds);
    RecordingStartTime = FPlatformTime::Seconds();
    LastSampleTime = RecordingStartTime;
    Samples.Empty();
    Samples.Reserve(static_cast<int32>(TargetDuration * 120.0f));
    PacketArrivalTimes.Empty();
    PacketArrivalTimes.Reserve(static_cast<int32>(TargetDuration * 120.0f));
    bIsRecording = true;
    UE_LOG(LogTemp, Display, TEXT("[Rebocap Profiler] Started deep diagnostic recording for %.1f seconds..."), TargetDuration);
}

void FRebocapProfiler::SampleFrame(float DeltaSeconds, float CurrentMocapHz, bool bIsConnected) {
    if (!bIsRecording) return;

    double CurrentTime = FPlatformTime::Seconds();
    float Elapsed = static_cast<float>(CurrentTime - RecordingStartTime);

    if (Elapsed >= TargetDuration) {
        StopRecordingAndGenerateReport();
        return;
    }

    FRebocapSamplePoint Point;
    Point.ElapsedTime = Elapsed;
    Point.RenderDeltaMs = DeltaSeconds * 1000.0f;
    Point.RenderFPS = DeltaSeconds > 0.0f ? (1.0f / DeltaSeconds) : 0.0f;
    Point.MocapHz = CurrentMocapHz;
    Point.bConnected = bIsConnected;

    Samples.Add(Point);
}

void FRebocapProfiler::RecordPacketArrival(double Timestamp) {
    if (bIsRecording) {
        PacketArrivalTimes.Add(Timestamp);
    }
}

void FRebocapProfiler::UpdateSkeletalContext(const FRebocapBoneMappingInfo& InInfo) {
    CachedBoneInfo = InInfo;
    bHasBoneInfo = true;
}

void FRebocapProfiler::UpdateNodeConfig(const FRebocapNodeConfigSnapshot& InConfig) {
    CachedNodeConfig = InConfig;
    bHasNodeConfig = true;
}

float FRebocapProfiler::GetRemainingTime() const {
    if (!bIsRecording) return 0.0f;
    double CurrentTime = FPlatformTime::Seconds();
    float Elapsed = static_cast<float>(CurrentTime - RecordingStartTime);
    return FMath::Max(0.0f, TargetDuration - Elapsed);
}

float FRebocapProfiler::GetProgress() const {
    if (!bIsRecording || TargetDuration <= 0.0f) return 0.0f;
    double CurrentTime = FPlatformTime::Seconds();
    float Elapsed = static_cast<float>(CurrentTime - RecordingStartTime);
    return FMath::Clamp(Elapsed / TargetDuration, 0.0f, 1.0f);
}

void FRebocapProfiler::StopRecordingAndGenerateReport() {
    if (!bIsRecording && Samples.Num() == 0) return;
    bIsRecording = false;

    FString ReportContent = GenerateReportString();

    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Logs");
    IFileManager::Get().MakeDirectory(*LogDir, true);

    FString TimeStamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    FString FileName = FString::Printf(TEXT("Rebocap_Diagnostic_Report_%s.txt"), *TimeStamp);
    LastReportFilePath = LogDir / FileName;

    bool bSaved = FFileHelper::SaveStringToFile(ReportContent, *LastReportFilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
    if (bSaved) {
        UE_LOG(LogTemp, Display, TEXT("[Rebocap Profiler] Deep diagnostic report successfully saved to: %s"), *LastReportFilePath);
        FPlatformProcess::ExploreFolder(*LastReportFilePath);
    } else {
        UE_LOG(LogTemp, Error, TEXT("[Rebocap Profiler] Failed to save diagnostic report to: %s"), *LastReportFilePath);
    }
}

static FString GetQualityName(int32 Level) {
    switch (Level) {
        case 0: return TEXT("低 (Low)");
        case 1: return TEXT("中 (Medium)");
        case 2: return TEXT("高 (High)");
        case 3: return TEXT("史诗 (Epic)");
        case 4: return TEXT("影视级 (Cinematic)");
        default: return FString::Printf(TEXT("Custom (%d)"), Level);
    }
}

FString FRebocapProfiler::GenerateReportString() {
    FString Report;
    Report += TEXT("================================================================================\n");
    Report += TEXT("        REBOCAP UE 插件 深度运行与性能全景诊断报告 (AI 专用分析版)\n");
    Report += TEXT("================================================================================\n");
    Report += FString::Printf(TEXT("生成时间: %s\n"), *FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S")));
    Report += TEXT("插件版本: V2.0-beta10 (Build 30)\n");
    Report += FString::Printf(TEXT("虚幻引擎版本: %d.%d.%d\n"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION);

#if WITH_EDITOR
    Report += TEXT("运行模式: Unreal Editor (编辑器模式)\n");
#else
    Report += TEXT("运行模式: Standalone / Packaged Game (独立打包游戏模式)\n");
#endif

    // 1. 硬件与系统环境
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【1. 硬件与系统环境】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");
    Report += FString::Printf(TEXT("- 操作系统: %s\n"), *FPlatformMisc::GetOSVersion());
    Report += FString::Printf(TEXT("- 处理器 (CPU): %s (物理核心: %d, 逻辑线程: %d)\n"),
        *FPlatformMisc::GetCPUBrand(),
        FPlatformMisc::NumberOfCores(),
        FPlatformMisc::NumberOfCoresIncludingHyperthreads());

    FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
    Report += FString::Printf(TEXT("- 物理内存 (RAM): %llu MB (可用: %llu MB)\n"),
        MemStats.TotalPhysical / (1024 * 1024),
        MemStats.AvailablePhysical / (1024 * 1024));

    Report += FString::Printf(TEXT("- 显卡 (GPU): %s\n"), *FPlatformMisc::GetPrimaryGPUBrand());

    // 2. 引擎视口与渲染管线配置
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【2. 引擎视口与渲染画质配置】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");

    IConsoleVariable* CVarMaxFPS = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
    float MaxFPS = CVarMaxFPS ? CVarMaxFPS->GetFloat() : 0.0f;
    Report += FString::Printf(TEXT("- 帧率上限 (t.MaxFPS): %.1f %s\n"), MaxFPS, MaxFPS <= 0.0f ? TEXT("(无限制)") : TEXT(""));

    IConsoleVariable* CVarVSync = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
    int32 VSync = CVarVSync ? CVarVSync->GetInt() : 0;
    Report += FString::Printf(TEXT("- 垂直同步 (r.VSync): %s\n"), VSync > 0 ? TEXT("开启 (Enabled)") : TEXT("关闭 (Disabled)"));

    IConsoleVariable* CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
    float ScreenPercentage = CVarScreenPercentage ? CVarScreenPercentage->GetFloat() : 100.0f;
    Report += FString::Printf(TEXT("- 屏幕渲染百分比: %.1f%%\n"), ScreenPercentage);

    IConsoleVariable* CVarLumenGI = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.DiffuseIndirect.Allow"));
    int32 LumenGI = CVarLumenGI ? CVarLumenGI->GetInt() : 1;
    Report += FString::Printf(TEXT("- Lumen 全局光照: %s\n"), LumenGI > 0 ? TEXT("开启 (Enabled)") : TEXT("关闭 (Disabled)"));

    IConsoleVariable* CVarNanite = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite"));
    int32 Nanite = CVarNanite ? CVarNanite->GetInt() : 1;
    Report += FString::Printf(TEXT("- Nanite 虚拟几何体: %s\n"), Nanite > 0 ? TEXT("开启 (Enabled)") : TEXT("关闭 (Disabled)"));

    Scalability::FQualityLevels QualityLevels = Scalability::GetQualityLevels();
    Report += FString::Printf(TEXT("- 全局画质等级:\n"));
    Report += FString::Printf(TEXT("  * 视距: %s | 抗锯齿: %s | 阴影: %s | 后期处理: %s\n"),
        *GetQualityName(QualityLevels.ViewDistanceQuality),
        *GetQualityName(QualityLevels.AntiAliasingQuality),
        *GetQualityName(QualityLevels.ShadowQuality),
        *GetQualityName(QualityLevels.PostProcessQuality));
#if ENGINE_MAJOR_VERSION >= 5
    Report += FString::Printf(TEXT("  * 纹理: %s | 特效: %s | 全局光照: %s | 反射: %s\n"),
        *GetQualityName(QualityLevels.TextureQuality),
        *GetQualityName(QualityLevels.EffectsQuality),
        *GetQualityName(QualityLevels.GlobalIlluminationQuality),
        *GetQualityName(QualityLevels.ReflectionQuality));
#else
    Report += FString::Printf(TEXT("  * 纹理: %s | 特效: %s | 植被: %s\n"),
        *GetQualityName(QualityLevels.TextureQuality),
        *GetQualityName(QualityLevels.EffectsQuality),
        *GetQualityName(QualityLevels.FoliageQuality));
#endif

    // 3. 角色模型与 24 根骨骼重定向映射表
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【3. 角色模型与 24 根骨骼重定向映射表】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");
    if (bHasBoneInfo) {
        Report += FString::Printf(TEXT("- 骨骼网格体 (Skeletal Mesh): %s\n"), *CachedBoneInfo.MeshName);
        Report += FString::Printf(TEXT("- 骨骼资产 (Skeleton): %s\n"), *CachedBoneInfo.SkeletonName);
        Report += FString::Printf(TEXT("- 骨骼总数量: %d 根 | 当前 LOD 等级: %d\n"), CachedBoneInfo.BoneCount, CachedBoneInfo.LODLevel);
        Report += FString::Printf(TEXT("- 骨骼重定向资产 (Retarget Asset): %s\n"), *CachedBoneInfo.RetargetAssetName);
        Report += TEXT("- 24 根核心骨骼映射详情:\n");

        int32 UnmappedCount = 0;
        for (const auto& Pair : CachedBoneInfo.BoneMap) {
            bool bIsNone = (Pair.Value.IsEmpty() || Pair.Value.Equals(TEXT("None"), ESearchCase::IgnoreCase));
            if (bIsNone) {
                UnmappedCount++;
                Report += FString::Printf(TEXT("  ❌ %-16s -> %-24s [警告: 未映射 / None]\n"), *Pair.Key, *Pair.Value);
            } else {
                Report += FString::Printf(TEXT("  ✓  %-16s -> %-24s\n"), *Pair.Key, *Pair.Value);
            }
        }
        if (UnmappedCount > 0) {
            Report += FString::Printf(TEXT("⚠️ 发现 %d 根骨骼未映射！可能导致对应部位无法动捕驱动。\n"), UnmappedCount);
        } else {
            Report += TEXT("✓ 24 根骨骼已全部成功映射绑定。\n");
        }
    } else {
        Report += TEXT("○ 当前视口或动画蓝图未激活 Rebocap Body Pose 节点，未捕获到角色骨骼绑定信息。\n");
    }

    // 4. 动画节点当前运行配置快照
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【4. 动画节点当前运行配置快照】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");
    if (bHasNodeConfig) {
        Report += FString::Printf(TEXT("- 动捕自动连接 (Connect Rebocap): %s\n"), CachedNodeConfig.bAutoConnect ? TEXT("开启") : TEXT("关闭"));
        Report += FString::Printf(TEXT("- 自动骨骼回传 (Auto submit skeleton data): %s\n"), CachedNodeConfig.bAutoSkeleton ? TEXT("开启") : TEXT("关闭"));
        Report += FString::Printf(TEXT("- 帧平滑插值 (Enable Frame Interpolation): %s (速率: %.1f)\n"),
            CachedNodeConfig.bEnableInterpolation ? TEXT("开启") : TEXT("关闭"),
            CachedNodeConfig.InterpolationSpeed);
        Report += FString::Printf(TEXT("- 丢包防闪保护 (Hold Pose On Dropout): %s (超时: %.1fs)\n"),
            CachedNodeConfig.bHoldPoseOnDropout ? TEXT("开启") : TEXT("关闭"),
            CachedNodeConfig.DropoutTimeout);
        Report += FString::Printf(TEXT("- 实验性开关: 骨骼节流保护=%s | 零内存分配=%s | 线程锁优化=%s\n"),
            CachedNodeConfig.bThrottleSkeletonRegistration ? TEXT("开启") : TEXT("关闭"),
            CachedNodeConfig.bZeroAllocStaticSubject ? TEXT("开启") : TEXT("关闭"),
            CachedNodeConfig.bOptimizeFrameEvaluation ? TEXT("开启") : TEXT("关闭"));
    } else {
        Report += TEXT("○ 节点配置未就绪。\n");
    }

    // 5. 动捕网络数据包深度抖动与时序分析
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【5. 动捕网络数据包深度抖动与到达时序分析】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");
    auto Source = FRebocapSource::GetInstance();
    bool bConnected = Source.IsValid() && Source->IsPortOpen();
    Report += FString::Printf(TEXT("- WebSocket 端口: 7690 | 链路状态: %s\n"), bConnected ? TEXT("● 正常推流中") : TEXT("○ 未连接"));

    int32 TotalPackets = PacketArrivalTimes.Num();
    Report += FString::Printf(TEXT("- 10 秒采样期总接收动捕包: %d 个\n"), TotalPackets);

    if (TotalPackets > 1) {
        int32 NormalIntervals = 0;   // 12ms ~ 20ms (around 60Hz)
        int32 DelayedIntervals = 0;  // 20ms ~ 50ms
        int32 DropoutIntervals = 0;  // > 50ms
        double MaxGapSeconds = 0.0;
        double SumInterval = 0.0;

        for (int32 i = 1; i < TotalPackets; ++i) {
            double Interval = PacketArrivalTimes[i] - PacketArrivalTimes[i - 1];
            double IntervalMs = Interval * 1000.0;
            SumInterval += Interval;
            MaxGapSeconds = FMath::Max(MaxGapSeconds, Interval);

            if (IntervalMs >= 12.0 && IntervalMs <= 20.0) {
                NormalIntervals++;
            } else if (IntervalMs > 50.0) {
                DropoutIntervals++;
            } else {
                DelayedIntervals++;
            }
        }

        int32 TotalIntervals = TotalPackets - 1;
        float NormalPct = (static_cast<float>(NormalIntervals) / TotalIntervals) * 100.0f;
        float AvgIntervalMs = static_cast<float>((SumInterval / TotalIntervals) * 1000.0);

        Report += FString::Printf(TEXT("- 平均数据包到达间隔: %.2f ms (理论 60Hz: 16.67 ms)\n"), AvgIntervalMs);
        Report += FString::Printf(TEXT("- 正常 60Hz 到达率 (12~20ms): %.1f%% (%d/%d)\n"), NormalPct, NormalIntervals, TotalIntervals);
        Report += FString::Printf(TEXT("- 轻微网络延迟 (20~50ms): %d 次\n"), DelayedIntervals);
        Report += FString::Printf(TEXT("- 严重卡顿丢包 (>50ms): %d 次\n"), DropoutIntervals);
        Report += FString::Printf(TEXT("- 最大无数据间隔 (Max Gap): %.1f ms\n"), MaxGapSeconds * 1000.0);
    }

    // 6. 视口渲染帧率采样统计
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【6. 视口渲染帧率采样统计与波形】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");

    if (Samples.Num() > 0) {
        float SumFPS = 0.0f;
        float MinFPS = 999999.0f;
        float MaxRenderFPS = 0.0f;
        float SumHz = 0.0f;
        float MinHz = 999999.0f;
        float MaxHz = 0.0f;
        int32 ConnectedSamples = 0;

        for (const auto& Pt : Samples) {
            SumFPS += Pt.RenderFPS;
            MinFPS = FMath::Min(MinFPS, Pt.RenderFPS);
            MaxRenderFPS = FMath::Max(MaxRenderFPS, Pt.RenderFPS);

            if (Pt.bConnected && Pt.MocapHz > 0.0f) {
                SumHz += Pt.MocapHz;
                MinHz = FMath::Min(MinHz, Pt.MocapHz);
                MaxHz = FMath::Max(MaxHz, Pt.MocapHz);
                ConnectedSamples++;
            }
        }

        float AvgFPS = SumFPS / static_cast<float>(Samples.Num());
        float AvgHz = ConnectedSamples > 0 ? (SumHz / static_cast<float>(ConnectedSamples)) : 0.0f;
        if (ConnectedSamples == 0) MinHz = 0.0f;

        Report += FString::Printf(TEXT("- 视口渲染帧率: 平均 %.1f FPS | 最低 %.1f FPS | 最高 %.1f FPS\n"), AvgFPS, MinFPS, MaxRenderFPS);
        Report += FString::Printf(TEXT("- 动捕接收频率: 平均 %.1f Hz  | 最低 %.1f Hz  | 最高 %.1f Hz\n"), AvgHz, MinHz, MaxHz);

        Report += TEXT("\n--- 采样时序波形 (每 0.25s 汇总) ---\n");
        float BucketStart = 0.0f;
        float BucketFPS = 0.0f;
        float BucketHz = 0.0f;
        int32 BucketCount = 0;

        for (int32 i = 0; i < Samples.Num(); ++i) {
            const auto& Pt = Samples[i];
            BucketFPS += Pt.RenderFPS;
            BucketHz += Pt.MocapHz;
            BucketCount++;

            if (Pt.ElapsedTime - BucketStart >= 0.25f || i == Samples.Num() - 1) {
                float AvgBucketFPS = BucketCount > 0 ? (BucketFPS / BucketCount) : 0.0f;
                float AvgBucketHz = BucketCount > 0 ? (BucketHz / BucketCount) : 0.0f;
                Report += FString::Printf(TEXT("  [%5.2fs ~ %5.2fs] 视口渲染: %5.1f FPS | 动捕接收: %5.1f Hz %s\n"),
                    BucketStart, Pt.ElapsedTime, AvgBucketFPS, AvgBucketHz,
                    AvgBucketHz < 45.0f && bConnected ? TEXT("<< [上位机丢帧/波动]") : TEXT(""));

                BucketStart = Pt.ElapsedTime;
                BucketFPS = 0.0f;
                BucketHz = 0.0f;
                BucketCount = 0;
            }
        }
    }

    // 7. 智能诊断分析结论与排查建议
    Report += TEXT("\n--------------------------------------------------------------------------------\n");
    Report += TEXT("【7. 智能诊断分析结论与排查建议】\n");
    Report += TEXT("--------------------------------------------------------------------------------\n");

    if (!bConnected) {
        Report += TEXT("❌ [未连通动捕] 当前未连接到 Rebocap 上位机 (端口 7690)。请确保 Rebocap 软件已开启并正在推流。\n");
    } else {
        Report += TEXT("✓ 动捕链路连通正常。\n");
    }

    if (Samples.Num() > 0) {
        float SumFPS = 0.0f;
        for (const auto& Pt : Samples) SumFPS += Pt.RenderFPS;
        float AvgFPS = SumFPS / static_cast<float>(Samples.Num());

        if (AvgFPS < 25.0f) {
            Report += TEXT("⚠️ [视口渲染过低] 视口渲染帧率低于 25 FPS。可能原因:\n");
            Report += TEXT("   1. 视口未开启 Realtime 模式 (按 Ctrl+R 开启)。\n");
            Report += TEXT("   2. 编辑器焦点切出，触发了后台降频。\n");
        } else if (AvgFPS > 100.0f && (!bHasNodeConfig || !CachedNodeConfig.bEnableInterpolation)) {
            Report += TEXT("💡 [高刷屏平滑建议] 视口渲染帧率超过 100 FPS。建议在动画蓝图节点开启【Enable Frame Interpolation / 帧平滑插值】，消除 60Hz 动捕采样不同步引起的微顿挫。\n");
        }
    }

    Report += TEXT("================================================================================\n");
    Report += TEXT("报告生成完毕。\n");
    Report += TEXT("================================================================================\n");

    return Report;
}
