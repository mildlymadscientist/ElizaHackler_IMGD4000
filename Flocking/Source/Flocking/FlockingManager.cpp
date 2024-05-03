#include "FlockingManager.h"
#include "Agent.h"
#include "Math/Vector.h"

//count of boids
#define AGENT_COUNT 10    

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

    World = world;

    //place boids
    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {

    FVector v1, v2, v3, v4;


    //for all agents
    for (AAgent* agent : Agents) {
     
        //Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
        v1 = rule1(agent);


        //Rule 2: Boids try to keep a small distance away from other objects (including other boids).

        v2 = rule2(agent);


        //Rule 3: Boids try to match velocity with near boids.

        v3 = rule3(agent);

        //Tendency towards a particular place
        v4 = tend_to_place(agent);

   
        agent->Velocity = agent->Velocity + v1 + v2 + v3 + v4;
        //Agents[i]->SetActorLocation(Agents[i]->GetActorLocation() + Agents[i]->Velocity);
            
    }

}

FVector UFlockingManager::rule1(AAgent* agentOrig)
{
    FVector sum = FVector(0.0f, 0.0f, 0.0f);

    for (int j = 0; j < AGENT_COUNT; j++) {

        if (Agents[j] != agentOrig) {

            sum = sum + Agents[j]->GetActorLocation();

        }

        sum = sum / (AGENT_COUNT - 1);

    }
    return (sum - agentOrig->GetActorLocation()) / 100;
}

FVector UFlockingManager::rule2(AAgent* agentOrig)
{

    FVector distance = FVector(0.0f, 0.0f, 0.0f);
    float vector;

    for (int j = 0; j < AGENT_COUNT; j++) {

        if (Agents[j] != agentOrig) {

            vector = FVector::Dist(agentOrig->GetActorLocation(), Agents[j]->GetActorLocation());

            if (abs(vector) < 100) {
                distance = distance - (agentOrig->GetActorLocation() - Agents[j]->GetActorLocation());
            }
        }

    }

    return distance;
}

FVector UFlockingManager::rule3(AAgent* agentOrig)
{

    FVector match = FVector(0.0f, 0.0f, 0.0f);

    for (int j = 0; j < AGENT_COUNT; j++) {
        if (Agents[j] != agentOrig) {
            match = match + Agents[j]->Velocity;
        }


        match = match / (AGENT_COUNT - 1);


    }

    (match - agentOrig->Velocity) / 8;
    return (match - agentOrig->Velocity) / 8;
}

FVector UFlockingManager::tend_to_place(AAgent* agentOrig)
{
    //change place to change center of flock
    FVector place = FVector(1500.0f, 1500.0f, 1500.0f);

    return (place - agentOrig->GetActorLocation()) / 120;
}