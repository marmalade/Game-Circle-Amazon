/*
 * Internal header for the s3eGameCircle extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EGAMECIRCLE_INTERNAL_H
#define S3EGAMECIRCLE_INTERNAL_H

#include "s3eTypes.h"
#include "s3eGameCircle.h"
#include "s3eGameCircle_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eGameCircleInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eGameCircleInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eGameCircleTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eGameCircleTerminate_platform();
void s3eGameCircleInitialize_platform(bool useAchievements, bool useWhispersync, bool useLeaderboards, bool firsRun);

s3eGameCircleStatus s3eGameCircleGetStatus_platform();

bool s3eGameCircleIsReady_platform();

void s3eGameCircleGetPlayerAlias_platform(const char* developerPayload);

void s3eGameCircleShowAchievementsOverlay_platform();

void s3eGameCircleUpdateAchievement_platform(const char* achievementId, float percentComplete, const char* developerPayload);

void s3eGameCircleResetAchievements_platform();

void s3eGameCircleResetAchievement_platform(const char* achievementId, const char* developerPayload);

void s3eGameCircleSetPopUpLocation_platform(s3eGameCirclePopUpLocation location);

void s3eGameCircleShowLeaderboardOverlay_platform(const char* leaderboardId);

void s3eGameCircleShowLeaderboardsOverlay_platform();

void s3eGameCircleSubmitScore_platform(const char* leaderboardId, int64_t score, const char* developerPayload);

void s3eGameCircleGetLeaderboards_platform(const char* developerPayload);

void s3eGameCircleGetScores_platform(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, int startRank, int count, const char* developerPayload);

void s3eGameCircleGetLocalPlayerScore_platform(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, const char* developerPayload);

bool s3eGameCircleHasNewMultiFileGameData_platform();

void s3eGameCircleUnpackNewMultiFileGameData_platform();

void s3eGameCircleSynchronizeBlob_platform(s3eGameCircleConflictStrategy conflictStrategy);

void s3eGameCirclenchronizeBlobProgress_platform(const char* description, const void* data, int dataLen, s3eGameCircleConflictStrategy conflictStrategy);

void s3eGameCircleSynchronizeMultiFile_platform(s3eGameCircleConflictStrategy conflictStrategy);

void s3eGameCircleSynchronizeMultiFileProgress_platform(const char* description, s3eGameCircleConflictStrategy conflictStrategy);

void s3eGameCircleRequestRevertBlob_platform();

void s3eGameCircleRequestRevertMultiFile_platform();


#endif /* !S3EGAMECIRCLE_INTERNAL_H */