// Copyright Nourda

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"


UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	
public:
	virtual void HighlightActor() = 0; // =0 makes it a pure virtual function, no definition required since its abstract
	virtual void UnHighlightActor() = 0;
	
};
