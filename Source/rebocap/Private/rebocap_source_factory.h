#pragma once

#include "CoreMinimal.h"
#include "LiveLinkSourceFactory.h"
#include "rebocap_source_factory.generated.h"

UCLASS()
class URebocapSourceFactory final : public ULiveLinkSourceFactory {
  GENERATED_BODY()

  virtual FText GetSourceDisplayName() const override final;
  virtual FText GetSourceTooltip() const override final;
  virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override final;
  virtual EMenuType GetMenuType() const override final;
  // virtual TSharedPtr<ILiveLinkSource>
  virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString &ConnectionString) const override;
  
 private:
  void OnSourceSelected(uint16_t port, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const;
};
