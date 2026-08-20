#include "rebocap_source_factory.h"

#include "Internationalization/Text.h"
#include "rebocap_source.h"
#include "rebocap_source_editor.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#define LOCTEXT_NAMESPACE "RebocapSourceFactory"

FText URebocapSourceFactory::GetSourceDisplayName() const { return LOCTEXT("SourceDisplayName", "Rebocap Source"); }

FText URebocapSourceFactory::GetSourceTooltip() const { return LOCTEXT("SourceTooltip", "Creates a connection to a Rebocap Source"); }

TSharedPtr<SWidget> URebocapSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const {
  return SNew(SRebocapSourceEditor)
    .OnRebocapSourceClickConnect(SRebocapSourceEditor::FOnRebocapSourceClickConnect::CreateUObject(
        this, &URebocapSourceFactory::OnSourceSelected, OnLiveLinkSourceCreated));
}

URebocapSourceFactory::EMenuType URebocapSourceFactory::GetMenuType() const { return EMenuType::SubPanel; }

void URebocapSourceFactory::OnSourceSelected(uint16_t port, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const {
  auto source = MakeShared<FRebocapSource>(port);
  FRebocapSource::SetInstance(source);
  FString ConnectionString = FString::Printf(TEXT("Name=\"rebocap\""));
  OnLiveLinkSourceCreated.ExecuteIfBound(source, MoveTemp(ConnectionString));
}

TSharedPtr<ILiveLinkSource> URebocapSourceFactory::CreateSource(const FString& ConnectionString) const {
  // 创建并返回一个新的 LiveLink
  FString Name;
  UE_LOG(LogTemp, Warning, TEXT("connect string:%s"), *ConnectionString);
  if (!FParse::Value(*ConnectionString, TEXT("Name="), Name)) {
    return TSharedPtr<ILiveLinkSource>();
  }
  FIPv4Endpoint DeviceEndPoint;
  if (!FIPv4Endpoint::Parse(ConnectionString, DeviceEndPoint)) {
    UE_LOG(LogTemp, Warning, TEXT("use default address, failed to parse connection address"));
    DeviceEndPoint.Address = FIPv4Address::Any;
    DeviceEndPoint.Port = 7690;
  }

  TSharedPtr<FRebocapSource> newSource = MakeShared<FRebocapSource>(DeviceEndPoint.Port);
  FRebocapSource::SetInstance(newSource);

  return newSource;
}


#undef LOCTEXT_NAMESPACE