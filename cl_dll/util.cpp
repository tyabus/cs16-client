/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// util.cpp
//
// implementation of class-less helper functions
//

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "hud.h"
#include "cl_util.h"
#include <string.h>

#ifdef _MSC_VER
#include <ctype.h>
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

//vec3_t vec3_origin( 0, 0, 0 );

//double sqrt(double x);

float rsqrt( float number )
{
	int	i;
	float	x, y;

	if( number == 0.0f )
		return 0.0f;

	x = number * 0.5f;
	i = *(int *)&number;	// evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);	// what the fuck?
	y = *(float *)&i;
	y = y * (1.5f - (x * y * y));	// first iteration

	return y;
}
#ifdef _MSC_VER
char *strcasestr(const char *str, const char *pattern)
{
	size_t i;

	if (!*pattern)
		return (char*)str;

	for (; *str; str++) 
	{
		if (toupper((unsigned char)*str) == toupper((unsigned char)*pattern)) 
		{
			for (i = 1;; i++) 
			{
				if (!pattern[i])
					return (char*)str;
					
				if (toupper((unsigned char)str[i]) != toupper((unsigned char)pattern[i]))
					break;
			}
		}
	}
	return NULL;
}
#endif

// defined in pm_math.cpp
float Length(const float *v)
{
	int		i;
	float	length;
	
	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);		// FIXME

	return length;
}
void VectorAngles( const float *forward, float *angles )
{
	float	tmp, yaw, pitch;
	
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = (atan2(forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

float VectorNormalize (float *v)
{
	float	length;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = rsqrt (length);

	if (length)
	{
		v[0] *= length;
		v[1] *= length;
		v[2] *= length;
	}

	return length;

}

void NormalizeAngles(float *angles)
{
	int i;
	// Normalize angles
	for (i = 0; i < 3; ++i)
	{
		if (angles[i] > 180.0)
		{
			angles[i] -= 360.0;
		}
		else if (angles[i] < -180.0)
		{
			angles[i] += 360.0;
		}
	}
}

// defined in pm_math.cpp
void CrossProduct(const float *v1, const float *v2, float *cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void VectorTransform(const vec_t *in1, float(*in2)[4], vec_t *out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

int HUD_GetSpriteIndexByName( const char *sz )
{
	return gHUD.GetSpriteIndex(sz);
}

_HSPRITE HUD_GetSprite( int index )
{
	return gHUD.GetSprite(index);
}

wrect_t HUD_GetSpriteRect( int index )
{
	return gHUD.GetSpriteRect( index );
}

vec3_t g_ColorBlue	= { 0.6, 0.8, 1.0 };
vec3_t g_ColorRed		= { 1.0, 0.25, 0.25 };
vec3_t g_ColorGreen	= { 0.6, 1.0, 0.6 };
vec3_t g_ColorYellow	= { 1.0, 0.7, 0.0 };
vec3_t g_ColorGrey	= { 0.8, 0.8, 0.8 };

float *GetClientColor( int clientIndex )
{
	switch ( g_PlayerExtraInfo[clientIndex].teamnumber )
	{
	case TEAM_TERRORIST:  return g_ColorRed;
	case TEAM_CT:         return g_ColorBlue;
	case TEAM_SPECTATOR:
	case TEAM_UNASSIGNED: return g_ColorYellow;
	case 4:               return g_ColorGreen;
	default:              return g_ColorGrey;
	}
}


