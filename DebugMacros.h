#pragma once


// Creates a macro
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);

#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 1.f);

#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Black, true);

#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Black, true);\
	}


// Draw sphere for a single frame (for tick function)
#define DRAW_SPHERE_SINGLEFRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE_SINGLEFRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, -1.f);
#define DRAW_POINT_SINGLEFRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Black, false, -1.f);
#define DRAW_VECTOR_SINGLEFRAME(StartLocation, EndLocation) if (GetWorld()) \
{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Black, false, -1.f);\
	}