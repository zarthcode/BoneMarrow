/** 
 * @file Bone.h
 * @brief Defines for various BoneMarrow concepts and data-types
 * @todo - This is a preliminary implementation.
**/

#include "stm32f4xx_hal.h"

/// Skeletal Types
/// @todo Special extended skeletal types for direct communication w/bonus sensors
typedef enum
{
	SKELETALTYPE_Bone7,		// MantisVR EVP4 is in this category.
	SKELETALTYPE_Bone12,	// Reserved
	SKELETALTYPE_Bone17,	// Reserved
	SKELETALTYPE_Bone19,	// This is extreme, but reserved.
	SKELETALTYPE_RESERVED4,	// Reserved
	SKELETALTYPE_RESERVED5,	// Reserved
	SKELETALTYPE_LAST
	
} SkeletalType;

/// Standard bone positions
typedef enum
{
	Bone_Forearm,			// Onboard Sensor placed on the forearm
	Bone_Metacarpal,		// Sensor on the back of the hand.
	Bone_Thumb_Distal,		// Furthest bone on the thumb.
	Bone_Index_Distal,		// Furthest bone on the index finger.
	Bone_Middle_Distal,		// Furthest bone on the middle finger.
	Bone_Ring_Distal,		// Furthest bone on the ring finger.
	Bone_Pinky_Distal,		// Furthest bone on the pinky finger.
	Bone_Last				// Not a bone! Use this for looping.
} Bone7Type;


/// @brief Used to indicate that this skeleton/device represents either a left/right handed glove.
typedef enum
{
	HANDEDNESS_LEFT,
	HANDEDNESS_RIGHT

} HandednessType;

// Skeletal manipulation functions

// Reference Pose
// Apply Quaternion
// GetSkeleton

