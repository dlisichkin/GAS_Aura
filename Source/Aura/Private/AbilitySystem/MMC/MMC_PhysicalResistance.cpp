// Copyright some company name


#include "AbilitySystem/MMC/MMC_PhysicalResistance.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_PhysicalResistance::UMMC_PhysicalResistance()
{
	ResilienceDefinition.AttributeToCapture = UAuraAttributeSet::GetResilienceAttribute();
	ResilienceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResilienceDefinition.bSnapshot = false;

	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;
}

float UMMC_PhysicalResistance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	float Resilience = 0.f;
	GetCapturedAttributeMagnitude(ResilienceDefinition, Spec, EvaluationParameters, Resilience);
	Resilience = FMath::Max<float>(Resilience, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	return 3.f * Vigor + 2.f * Resilience + 10.f * CharacterLevel;
}
