// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGunTurret.h"

AMachineGunTurret::AMachineGunTurret()
{
	JointMesh = CreateDefaultSubobject<UStaticMeshComponent>("Joint Mesh");
	JointMesh->SetupAttachment(BaseMesh);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gun Mesh");
	GunMesh->SetupAttachment(JointMesh);
}