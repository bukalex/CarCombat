// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileTurret.h"

AMissileTurret::AMissileTurret()
{
	LauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>("Launcher Mesh");
	LauncherMesh->SetupAttachment(BaseMesh);
}